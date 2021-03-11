#ifndef __EXPRESSION_HPP__
#define __EXPRESSION_HPP__ 1

#include "term.hpp"
#include <set>


using namespace std;


/*
 *  Abstract base class Expression
 *  Derived classes will be Unary and Binary
 *  Note : Since we don't need to calculate the expression will have just these two,
 *  insted of Addition, Substraction, Product etc.
 */


class Expression {
public:
    
    // returns set of variables that contains in expression. example : a + 5 -> {a} , 3 - 2 -> {} , a * b -> {a,b}
    virtual set<string> returnVars() const = 0;
    
    // prints Expression to STDOUT
    virtual void print() const = 0;
    
    // prints Expression after register allocation
    virtual void printRegs(const map<string,unsigned>& mapping) const = 0;
    
    virtual ~Expression();
    
};


class Unary : public Expression {
public:
    Unary(Term* operand)
    : m_operand(operand)
    {}
    ~Unary();
    
    set<string> returnVars() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
private:
    Term* m_operand;
    
    Unary(const Unary& j) = delete;
    Unary& operator= (const Unary& j) = delete;
};


class Binary : public Expression {
public:
    Binary(Term* left,const string& operation,Term* right)
    : m_left(left),
      m_operation(operation), 
      m_right(right)
    {}
    ~Binary();
    
    set<string> returnVars() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
private:
    Term* m_left;
    string m_operation;
    Term* m_right;
    
    Binary(const Binary& r) = delete;
    Binary& operator=(const Binary& r) = delete;
};

#endif
