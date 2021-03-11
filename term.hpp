#ifndef __TERM_HPP__
#define __TERM_HPP__ 1

#include <iostream>
#include <string>
#include <map>

using namespace std;

/*
 *  Abstract base class Term
 *  Derived classes Id and Num will be leaves of AST
 */
class Term {
public:
    
    // Term types
    enum class Type {
        ID,
        NUM
    };
    
    // returns type of Term
    virtual Type type() const = 0;
    
    // prints Term to STDOUT 
    virtual void print() const = 0;
    
    // prints Term after register allocation
    virtual void printRegs(const map<string,unsigned>& mapping) const = 0;
    
    virtual ~Term();
    
};


class Id : public Term {
public:
    Id(const string& id)
    : m_id(id)
    {}
    
    Type type() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
    string value() const;
    
private:
    string m_id;
};


class Num : public Term {
public:
    Num(int num)
    : m_num(num)
    {}
    
    Type type() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
    int value() const;
    
private:
    int m_num;    
};

#endif
