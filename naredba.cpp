#include "naredba.hpp"
#include <algorithm>

template <typename T>
set<T> getUnion(const set<T>& a, const set<T>& b){
         set<T> result = a;
         result.insert(b.begin(), b.end());
         return result;
    
}


Naredba::~Naredba(){
    
}

//  Dodela

set<string> Dodela::use() const{
    return _i->vratiPromenljive();
}

set<string> Dodela::def() const{
    set<string> skup;
    skup.insert(_id);
    return skup;
}

set<unsigned> Dodela::next(const unsigned& index, const unsigned& broj_naredbi) const{
    set<unsigned> skup;
    if(index!=broj_naredbi)
        skup.insert(index+1);
    return skup;
}

string Dodela::tip() const{
    return "dodela";
}

void Dodela::ispisi() const{
    cout<< _id <<" := ";
    _i->ispisi();
    cout<<endl;
}

void Dodela::ispisiRegistre(const map<string,unsigned>& preslikavanje) const{
    cout<<"r"<<preslikavanje.at(_id) <<" := ";
    _i->ispisiRegistre(preslikavanje);
    cout<<endl;
}

bool Dodela::obrisi(const set<string>& out) const{
    
    return out.find(_id)==out.end();
}

Dodela::~Dodela(){
    delete _i;
}


//  DodelaF

set<string> DodelaF::use() const{
    set<string> skup;
    if  (_t->tip()=="id"){
        Id* id = dynamic_cast<Id*>(_t);
        skup.insert(id->vrednost());
    }
    return skup;
}

set<string> DodelaF::def() const{
    set<string> skup;
    skup.insert(_id);
    return skup;
}

set<unsigned> DodelaF::next(const unsigned& index, const unsigned& broj_naredbi) const{
    set<unsigned> skup;
    if(index!=broj_naredbi)
        skup.insert(index+1);
    return skup;
}

string DodelaF::tip() const{
    return "dodelaf";
}

void DodelaF::ispisi() const{
    cout<<_id<<" := "<<_fname<<"(";
    _t->ispisi();
    cout<<")"<<endl;
}

void DodelaF::ispisiRegistre(const map<string,unsigned>& preslikavanje) const{
    cout<<"r"<<preslikavanje.at(_id)<<" := "<<_fname<<"(";
    _t->ispisiRegistre(preslikavanje);
    cout<<")"<<endl;
}

bool DodelaF::obrisi(const set<string>& out) const{
   
    return out.find(_id)==out.end();
}

DodelaF::~DodelaF(){
    delete _t;
}


//  IfGo

set<string> IfGo::use() const{
    return _uslov->vratiPromenljive();
}

set<string> IfGo::def() const{
    set<string> skup;
    return skup;
}

set<unsigned> IfGo::next(const unsigned& index, const unsigned& broj_naredbi) const{
    set<unsigned> skup;
    if(index!=broj_naredbi)
        skup.insert(index+1);
    skup.insert(_go);
    return skup;
}

string IfGo::tip() const{
    return "ifgo";
}

void IfGo::ispisi() const{
    cout<<"ifFalse ";
    _uslov->ispisi();
    cout<<" goto "<<_go<<endl;
}

void IfGo::ispisiRegistre(const map<string,unsigned>& preslikavanje) const{
    cout<<"ifFalse ";
    _uslov->ispisiRegistre(preslikavanje);
    cout<<" goto "<<_go<<endl;
}

void IfGo::podesiGo (const set<unsigned>& skup){
    
    unsigned smanji = count_if(
         skup.begin()
        ,skup.end()
        , [&](unsigned elem){ return elem < _go; } );
    _go-= smanji;
}

void IfGo::podesiGo2 (const vector<bool>& pre, const vector<bool>& posle){
    unsigned br=0;
    for(unsigned i=0;i<_go;i++){
        if(pre[i]) br++;
        if(posle[i]) br++;
    }
    _go+=br;
}

IfGo::~IfGo(){
    delete _uslov;
}


//  GoTo

set<string> GoTo::use() const{
    set<string> skup;
    return skup;
}

set<string> GoTo::def() const{
    set<string> skup;
    return skup;
}

set<unsigned> GoTo::next(__attribute__((unused)) const unsigned& index, __attribute__((unused)) const unsigned& broj_naredbi) const{
    set<unsigned> skup;
    skup.insert(_go);
    return skup;
}

string GoTo::tip() const{
    return "goto";
}

void GoTo::ispisi() const{
    cout<<"goto "<<_go<<endl;
}

void GoTo::ispisiRegistre(__attribute__((unused)) const map<string,unsigned>& preslikavanje) const{
    this->ispisi();
}

void GoTo::podesiGo (const set<unsigned>& skup){
    unsigned smanji = count_if(
         skup.begin()
        ,skup.end()
        , [&](unsigned elem){ return elem < _go; } );
    _go-= smanji;
    
}

void GoTo::podesiGo2 (const vector<bool>& pre, const vector<bool>& posle){
    unsigned br=0;
    for(unsigned i=0;i<_go;i++){
        if(pre[i]) br++;
        if(posle[i]) br++;
    }
    _go+=br;
}


//  Return 

set<string> Return::use() const{
    set<string> skup;
    skup.insert(_id);
    return skup;
}

set<string> Return::def() const{
    set<string> skup;
    return skup;
}

set<unsigned> Return::next(const unsigned& index, const unsigned& broj_naredbi) const{
    set<unsigned> skup;
    if(index!=broj_naredbi)
        skup.insert(index+1);
    return skup;
}

string Return::tip() const{
    return "return";
}

void Return::ispisi() const{
    cout<<"return "<<_id<<endl;
}

void Return::ispisiRegistre(const map<string,unsigned>& preslikavanje) const{
    cout<<"return "<<"r"<<preslikavanje.at(_id)<<endl;
}


// Load

set<string> Load::use() const{
    set<string> skup;
    return skup;
}
    
set<string> Load::def() const{
    set<string> skup;
    skup.insert(_id);
    return skup;
}

set<unsigned> Load::next(const unsigned& index, const unsigned& broj_naredbi) const{
    set<unsigned> skup;
    if(index!=broj_naredbi)
        skup.insert(index+1);
    return skup;
}
    
string Load::tip() const{
    return "load";
}
    
void Load::ispisi() const{
    cout<<_id<<" := "<<"M["<<_id<<"_loc]"<<endl;
}
    
void Load::ispisiRegistre(const map<string,unsigned>& preslikavanje) const{
    cout<<"r"<<preslikavanje.at(_id)<<" := "<<"M["<<_id<<"_loc]"<<endl;
}


// Store

set<string> Store::use() const{
    set<string> skup;
    skup.insert(_id);
    return skup;
}
    
set<string> Store::def() const{
    set<string> skup;
    return skup;
}

set<unsigned> Store::next(const unsigned& index, const unsigned& broj_naredbi) const{
    set<unsigned> skup;
    if(index!=broj_naredbi)
        skup.insert(index+1);
    return skup;
}
    
string Store::tip() const{
    return "store";
}
    
void Store::ispisi() const{
    cout<<"M["<<_id<<"_loc] := "<<_id<<endl;
}
    
void Store::ispisiRegistre(const map<string,unsigned>& preslikavanje) const{
    cout<<"M["<<_id<<"_loc] := r"<< preslikavanje.at(_id)<<endl;
}
