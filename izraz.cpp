#include "izraz.hpp"


Izraz::~Izraz() {
    
}


//  Jednoclan

set<string> Jednoclan::vratiPromenljive() const{
    
    set<string> skup;
    
    if (_t->tip() == "id"){
        Id* id = dynamic_cast<Id*>(_t);
        skup.insert(id->vrednost());
    }
    
    return skup;
}
    
void Jednoclan::ispisi() const{
    _t->ispisi();
}

void Jednoclan::ispisiRegistre(const map<string,unsigned>& preslikavanje) const{
    _t->ispisiRegistre(preslikavanje);
}

Jednoclan::~Jednoclan(){
    delete _t;
}


//  Binarni

set<string> Binarni::vratiPromenljive() const{
    set<string> skup;
    
    if (_t1->tip() == "id") {
        Id* id = dynamic_cast<Id*>(_t1);
        skup.insert(id->vrednost());
    }
    if (_t2->tip() == "id") {
        Id* id = dynamic_cast<Id*>(_t2);
        skup.insert(id->vrednost());
    }
    
    return skup;
}

void Binarni::ispisi() const{
    _t1->ispisi();
    cout<<" "<<_op<<" ";
    _t2->ispisi();
}

void Binarni::ispisiRegistre(const map<string,unsigned>& preslikavanje) const{
    _t1->ispisiRegistre(preslikavanje);
    cout<<" "<<_op<<" ";
    _t2->ispisiRegistre(preslikavanje);
}

Binarni::~Binarni(){
    delete _t1;
    delete _t2;
}
