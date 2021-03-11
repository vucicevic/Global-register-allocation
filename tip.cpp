#include "tip.hpp"

Tip::~Tip() {
    
}


//  Id

string Id::tip() const{
    return "id";
}

void Id::ispisi() const{
    cout<<_s;
}

void Id::ispisiRegistre(const map<string,unsigned>& preslikavanje) const{
    cout<<"r"<<preslikavanje.at(_s);
}
 
string Id::vrednost() const{
    return _s;
}


//  Num
    
string Num::tip() const{
    return "num";
}

void Num::ispisi() const{
    cout<<_num;
}

void Num::ispisiRegistre(__attribute__((unused)) const map<string,unsigned>& preslikavanje) const{
    this->ispisi();
}

int Num::vrednost() const{
    return _num;
}
