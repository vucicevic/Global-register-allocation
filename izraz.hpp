#ifndef __IZRAZ_HPP__
#define __IZRAZ_HPP__ 1

#include "tip.hpp"
#include <set>


using namespace std;


/*
 *  Klasa Izraz je bazna klasa iz koje će biti izvedene klase Jednoclan i Binarni.
 *  Pošto ne vršimo izračunavanje Izraza, već samo ispisivanje, nisu nam potrebne
 *  klase poput : Zbir, Proizvod, Razlika itd.
 * 
 */


class Izraz {
    
public:
    // metod vraća skup stringova koji sačinjavaju izraz. npr : a + 5 -> {a} , 3 - 2 -> {} , a * b -> {a,b}
    virtual set<string> vratiPromenljive() const = 0;
    // metod ispisuje na STDOUT Izraz 
    virtual void ispisi() const = 0;
    // metod ispisuje na STDOUT Izraz u skladu sa preslikavanjem registara
    virtual void ispisiRegistre(const map<string,unsigned>& preslikavanje) const = 0;
    virtual ~Izraz();
};

class Jednoclan : public Izraz {
public:
    Jednoclan(Tip* t)
    : _t(t)
    {}
    set<string> vratiPromenljive() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    ~Jednoclan();
private:
    Tip* _t;
    Jednoclan(const Jednoclan& j);
    Jednoclan& operator= (const Jednoclan& j);
};


class Binarni : public Izraz {
public:
    Binarni(Tip* t1,string op,Tip* t2)
    : _t1(t1),_op(op), _t2(t2)
    {}
    set<string> vratiPromenljive() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    ~Binarni();
private:
    Tip* _t1;
    string _op;
    Tip* _t2;
    Binarni(const Binarni& r);
    Binarni& operator=(const Binarni& r);
};
#endif
