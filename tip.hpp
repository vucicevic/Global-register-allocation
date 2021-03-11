#ifndef __TIP_HPP__
#define __TIP_HPP__ 1

#include <iostream>
#include <string>
#include <map>

using namespace std;


/*
 *  Klasa Tip je bazna klasa iz koje će biti izvedene klase Id i Num koje će nam predstavljati
 *  listove naseg AST-a.
 * 
 */


class Tip {
    
public:
    // metod vraća "id" ili "num"
    virtual string tip() const = 0;
    // metod ispisuje na STDOUT Tip 
    virtual void ispisi() const = 0;
    // metod ispisuje na STDOUT Tip u skladu sa preslikavanjem registara
    virtual void ispisiRegistre(const map<string,unsigned>& preslikavanje) const = 0;
    virtual ~Tip();
    
    
};

class Id : public Tip {
  
public:
    string tip() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    string vrednost() const;
    Id(string s)
    : _s(s)
    {}
private:
    string _s;
    
};

class Num : public Tip {
  
public:
    string tip() const;
    void ispisi() const;
    void ispisiRegistre(const map<string,unsigned>& preslikavanje) const;
    int vrednost() const;
    Num(int num)
    : _num(num)
    {}
private:
    int _num;
    
};



#endif
