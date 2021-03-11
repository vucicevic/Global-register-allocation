#ifndef __NAREDBA_HPP__
#define __NAREDBA_HPP__ 1

#include "izraz.hpp"
#include <vector>

using namespace std;

/*
 * Klasa Naredba je bazna klasa iz koje će biti izvedene klase:
 * Dodela  ( a := b + c, b := d, e := 3 )    , 
 * DodelaF ( a := sqrt(5), b := kvadrat(c) ) , 
 * IfGo,   ( ifFalse a < b goto 3 )          ,
 * GoTo    ( goto 5, goto 7 )                ,
 * Return  ( return t1, return b )           .
 * Load    ( a := M[a_loc]       )           ,
 * Store   ( M[a_loc] := a       )
 */

class Naredba {
    
public:
    // metod određuje skup USE
    virtual set<string> use() const = 0;
    // metod određuje skup DEF
    virtual set<string> def() const = 0;
    // metod određuje skup sledbenika
    virtual set<unsigned> next(const unsigned& index, const unsigned& broj_naredbi) const = 0;
    // metod vraća tip : "dodela", "dodelaf", "ifgo", "goto" , "return", "load", "store"
    virtual string tip() const = 0;
    // metod na STDOUT ispisuje Naredbu
    virtual void ispisi() const = 0;
    // metod na STDOUT ispisuje Naredbu u skladu sa preslikavanjem registara
    virtual void ispisiRegistre(const map<string,unsigned>& preslikavanje) const = 0;
    virtual ~Naredba();
};


class Dodela : public Naredba {
public:
    Dodela(string id, Izraz* i)
    : _id(id), _i(i)
    {}
    
    set<string> use() const;
    set<string> def() const;
    set<unsigned> next(const unsigned& index, const unsigned& broj_naredbi) const;
    string tip() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    // metod određuje da li je naredba suvišna
    bool obrisi(const set<string>& out) const;
    ~Dodela();
    
private:
    string _id;
    Izraz* _i;
    Dodela(const Dodela& d);
    Dodela& operator=(const Dodela& d);
};


class DodelaF : public Naredba {
public:
    DodelaF(string id, string fname, Tip* t)
    : _id(id), _fname(fname), _t(t)
    {}
    
    set<string> use() const;
    set<string> def() const;
    set<unsigned> next(const unsigned& index, const unsigned& broj_naredbi) const;
    string tip() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    // metod određuje da li je naredba suvišna
    bool obrisi(const set<string>& out) const;
    ~DodelaF();
    
private:
    string _id;
    string _fname;
    Tip* _t;
    DodelaF(const DodelaF& d);
    DodelaF& operator=(const DodelaF& d);
};


class IfGo : public Naredba {
public:
    IfGo(Izraz* uslov, int go)
    : _uslov(uslov), _go(go)
    {}
    
    set<string> use() const;
    set<string> def() const;
    set<unsigned> next(const unsigned& index, const unsigned& broj_naredbi) const;
    string tip() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    // metod ažurira skokove nakon što izbacimo mrtav kod
    void podesiGo (const set<unsigned>& skup);
    // metod ažurira skokove nakod dodavanja naredbi Load i Store
    void podesiGo2 (const vector<bool>& pre, const vector<bool>& posle);
    ~IfGo();
    
private:
    Izraz* _uslov;
    int _go;
    IfGo(const IfGo& d);
    IfGo& operator=(const IfGo& d);
};

class GoTo : public Naredba {
public:
    GoTo(int go)
    : _go(go)
    {}
    
    set<string> use() const;
    set<string> def() const;
    set<unsigned> next(const unsigned& index, const unsigned& broj_naredbi) const;
    string tip() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    // metod ažurira skokove nakon što izbacimo mrtav kod
    void podesiGo (const set<unsigned>& skup);
    // metod ažurira skokove nakod dodavanja naredbi Load i Store
    void podesiGo2 (const vector<bool>& pre, const vector<bool>& posle);

private:
    int _go;
};

class Return : public Naredba {
public:
    Return(string id)
    : _id(id)
    {}
    
    set<string> use() const;
    set<string> def() const;
    set<unsigned> next(const unsigned& index, const unsigned& broj_naredbi) const;
    string tip() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    
private:
    string _id;
};

class Load : public Naredba {
public:
    Load(string id)
    : _id(id)
    {}
    
    set<string> use() const;
    set<string> def() const;
    set<unsigned> next(const unsigned& index, const unsigned& broj_naredbi) const;
    string tip() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    
private:
    string _id;
};

class Store : public Naredba {
public:
    Store(string id)
    : _id(id)
    {}
    
    set<string> use() const;
    set<string> def() const;
    set<unsigned> next(const unsigned& index, const unsigned& broj_naredbi) const;
    string tip() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    
private:
    string _id;
};

#endif
