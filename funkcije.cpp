#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <iterator>
#include <vector>

#include "naredba.hpp"

using namespace std;

// pravimo uniju dva skupa
template <typename T>
set<T> getUnion(const set<T>& a, const set<T>& b){
    set<T> result = a;
    result.insert(b.begin(), b.end());
    return result;
}

    
// pravimo razliku dva skupa
template<typename T>
set<T> operator-(const set<T>& reference, const set<T>& items_to_remove){
    set<T> result;
    set_difference(
    reference.begin(), reference.end(),
    items_to_remove.begin(), items_to_remove.end(),
    inserter(result, result.end()));
    return result;
}


/*  pravimo skupove IN i OUT za svaku naredbu po sledećem algoritmu : 
 * 
 *      for each node n in CFG
 *          in[n] = /  
 *          out[n] = /
 *      
 *      repeat
 *          for each node n in CFG in reverse topsort order
 *              in'[n] = in[n]
 *              out'[n] = out[n]
 *              out[n] = U in[s], where s in succ[n]
 *              in[n]  = use[n] U (out[n] \ def[n])
 *      until (in'[n]==in[n] and out'[n]==out[n]) for all n
 * 
 * 
 */

void insOuts(vector<set<string>>& in, vector<set<string>>& out, const vector<set<string>>& use, const vector<set<string>>& def, const vector<set<unsigned>>& sledbenici) {
        
    vector<set<string>> inp (use.size());
    vector<set<string>> outp (use.size());
    
    bool radi = false;
    
    do{
    
        for(int i=use.size()-1;i>=0;i--){
        
            inp[i]=in[i];
            outp[i]=out[i];
            set<string>unija;
            for(auto it=sledbenici[i].begin();it!=sledbenici[i].end();it++)
                unija = getUnion(unija,in[*it]);
        
            out[i]=unija;
            set<string> razlika = out[i] - def[i];
            in[i]=getUnion(use[i],razlika);
        }
        
        for(int i=use.size()-1;i>=0;i--)
            if(inp[i]==in[i] && out[i]==out[i])
                radi=true;
            else{
                radi=false;
                break;
            }
    
    }
    while(!radi);       

    inp.erase(inp.begin(),inp.end());
    outp.erase(outp.begin(),outp.end());
}

// od skupova IN i OUT pravimo graf interferencije 
map<string,set<string>> build (const vector<set<string>>&in, const vector<set<string>>& out){
        
    set<set<string>> skup_zavisnosti;
    // grupišemo sve skupove u jedan ne uzimajući u obzir prazne
    for(unsigned i=0;i<in.size();i++){
        if(in[i].size()>0)
            skup_zavisnosti.insert(in[i]);
        if(out[i].size()>0)
            skup_zavisnosti.insert(out[i]);
    }
    
    
    //  pravimo graf interferencije na osnovu prethodno kreiranog skupa zavisnosti
    map<string,set<string>> mapa;
    for(auto it=skup_zavisnosti.begin();it!=skup_zavisnosti.end();it++)
        for(auto it1=it->begin();it1!=it->end();it1++){
            set<string> drugi;
            drugi.insert(*it1);
            set<string> razlika = *it - drugi;
            mapa[*it1] = getUnion(mapa[*it1],razlika);
         }
         
    skup_zavisnosti.erase(skup_zavisnosti.begin(),skup_zavisnosti.end());
    
    return mapa;

}

// radimo korak simplify i spill
void simplify_spill(map<string,set<string>>& mapa, stack<string>& stek, const unsigned& broj_registara) {

    // poslednji element samo stavimo na stek i završimo sa rekurzivnim pozivom
    if(mapa.size()==1){
        auto it=mapa.begin();
        stek.push(it->first);
        mapa.erase(it->first);
        return;
    }
    
    string kandidat;
    bool moguci_spill = true;
        
    // ako postoji element sa strogo manje suseda biramo prvog takvog
    for(auto it=mapa.begin();it!=mapa.end();it++)
        if(it->second.size()<broj_registara){
            kandidat = it->first;
            stek.push(kandidat);
            moguci_spill = false;
            break;
        }
        
         
    // ako je došlo do spill-a biramo prvi čvor iz mape i stavljamo ga na stek
    if(moguci_spill){
        auto it = mapa.begin();
        kandidat= it->first;
        stek.push(kandidat);
    }
        
    // ažuriramo graf interferencije i pozivamo rekurzivno
    mapa.erase(kandidat);
    for(auto it=mapa.begin();it!=mapa.end();it++)
        mapa[it->first].erase(kandidat);
        
    simplify_spill(mapa,stek,broj_registara);
    

}

// u koraku select dodeljujemo registre čvorovima
void select(map<string,unsigned>& preslikavanje,stack<string>& stek,const map<string,set<string>>& mapa, const set<unsigned>& registri, set<string>& prosute_promenljive){
    
    // ako je stek prazan završili smo rekurzivni poziv
    if(0==stek.size())
        return;
    
    string kandidat = stek.top();
        
    // skup u kojem će biti svi registri koji su zauzeti od strane susednih čvorova
    set<unsigned> susedni_registri;
    // skup u kojem će biti svi potencijalno slobodni registri
    set<unsigned> potencijalni_registri;
        
    // određujemo susedne registre
    for(auto it=mapa.at(kandidat).begin();it!=mapa.at(kandidat).end();it++)
        if(preslikavanje[*it]!=0)
            susedni_registri.insert(preslikavanje[*it]);
        
    // određujemo potencijalne registre
    potencijalni_registri = registri - susedni_registri;
    
    /* 
        ako imamo registar dodelimo ga promenljivoj u suprotnom je promenljiva stvarno prosuta
        i treba izmeniti program
     */ 
    if(potencijalni_registri.size()!=0){
        auto it = potencijalni_registri.begin();
        preslikavanje[kandidat] = *it;
    }
    else
        prosute_promenljive.insert(kandidat);
    
    
    // ažuriramo stek i pozivamo rekurzivno
    stek.pop();
    select(preslikavanje,stek,mapa,registri,prosute_promenljive);
    
}

// brišemo mrtve dodele
void mrtavKod(vector<Naredba*>& naredbe, vector<set<string>>& out, vector<set<string>>& in) {
    
    
    // na osnovu OUT skupova brišemo dodele koje nisu potrebne
    set<unsigned> za_izbaciti;
    for(unsigned i=0;i<naredbe.size();i++){
        if(naredbe[i]->tip()=="dodela"){
             Dodela* dodela = dynamic_cast<Dodela*>(naredbe[i]);
             if(dodela->obrisi(out[i]))
                 za_izbaciti.insert(i);
            
        }
        
        if(naredbe[i]->tip()=="dodelaf"){
             DodelaF* dodela = dynamic_cast<DodelaF*>(naredbe[i]);
             if(dodela->obrisi(out[i]))
                 za_izbaciti.insert(i);
        }
    }
    
    // ažuriramo skokove da bismo održali ekvivalentnost sa polaznim kodom
    for(unsigned i=0;i<naredbe.size();i++){
        if(naredbe[i]->tip()=="ifgo"){
            IfGo* ifgo = dynamic_cast<IfGo*>(naredbe[i]);
            ifgo->podesiGo(za_izbaciti);
        }
    
        if(naredbe[i]->tip()=="goto"){
            GoTo* go = dynamic_cast<GoTo*>(naredbe[i]);
            go->podesiGo(za_izbaciti);
        }
    }
    
    unsigned izbaceno = 0;
    for(auto it=za_izbaciti.begin();it!=za_izbaciti.end();it++){
        naredbe.erase(naredbe.begin()+*it-izbaceno);
        in.erase(in.begin()+*it-izbaceno);
        out.erase(out.begin()+*it-izbaceno);
        izbaceno++;
    }
    
    za_izbaciti.erase(za_izbaciti.begin(),za_izbaciti.end());
    
}

// ažuriramo skokove posle dodavanje load i store instrukcija
void azuriraj(const vector<bool>& pre, const vector<bool>& posle, vector<Naredba*> naredbe){
     
    for(unsigned i=0;i<naredbe.size();i++){
        if(naredbe[i]->tip()=="ifgo"){
            IfGo* ifgo = dynamic_cast<IfGo*>(naredbe[i]);
            ifgo->podesiGo2(pre,posle);
        }
    
        if(naredbe[i]->tip()=="goto"){
            GoTo* go = dynamic_cast<GoTo*>(naredbe[i]);
            go->podesiGo2(pre,posle);
        }
    }
    
    
}

