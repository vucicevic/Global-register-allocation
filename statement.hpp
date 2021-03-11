#ifndef __STATEMENT_HPP__
#define __STATEMENT_HPP__ 1

#include "expression.hpp"
#include <vector>

using namespace std;

/*
 * Abstract base class Stmt
 * Derived classes :
 *      Assign  ( a := b + c, b := d, e := 3 )    
 *      AssignF ( a := sqrt(5), b := kvadrat(c) )  
 *      IfGo,   ( ifFalse a < b goto 3 )          
 *      GoTo    ( goto 5, goto 7 )                
 *      Return  ( return t1, return b )           
 *      Load    ( a := M[a_loc]       )           
 *      Store   ( M[a_loc] := a       )
 */

class Stmt {
public:
    
    // Statement types
    enum class Type{
        ASSIGN,
        ASSIGN_F,
        IF_GO,
        GOTO,
        RETURN,
        LOAD,
        STORE
    };

    // returns USE set
    virtual set<string> use() const = 0;
    
    // returns DEF set
    virtual set<string> def() const = 0;
    
    // returns follow set
    virtual set<unsigned> follow(unsigned index, const unsigned num_of_stmts) const = 0;
    
    // returns statement type
    virtual Type type() const = 0;
    
    // prints Stmt to STDOUT
    virtual void print() const = 0;
    
    // prints Stmt after register allocation
    virtual void printRegs(const map<string,unsigned>& mapping) const = 0;
    
    virtual ~Stmt();
    
};


class Assign : public Stmt {
public:
    Assign(const string& id, Expression* expression)
    : m_id(id), 
      m_expression(expression)
    {}
    ~Assign();
    
    set<string> use() const override;
    set<string> def() const override;
    set<unsigned> follow(unsigned index, const unsigned num_of_stmts) const override;
    Type type() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
    // returns true if statement can be deleted (dead code)
    bool del(const set<string>& out) const;
    
private:
    string m_id;
    Expression* m_expression;
    
    Assign(const Assign& d) = delete;
    Assign& operator=(const Assign& d) = delete;
};


class AssignF : public Stmt {
public:
    AssignF(const string& id, const string& fname, Term* arg)
    : m_id(id), 
      m_fname(fname), 
      m_arg(arg)
    {}
    ~AssignF();
    
    set<string> use() const override;
    set<string> def() const override;
    set<unsigned> follow(unsigned index, const unsigned num_of_stmts) const override;
    Type type() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
    // returns true if statement can be deleted (dead code)
    bool del(const set<string>& out) const;
    
private:
    string m_id;
    string m_fname;
    Term* m_arg;
    
    AssignF(const AssignF& d) = delete;
    AssignF& operator=(const AssignF& d) = delete;
};


class IfGo : public Stmt {
public:
    IfGo(Expression* condition, int go)
    : m_condition(condition), 
      m_go(go)
    {}
    ~IfGo();
    
    set<string> use() const override;
    set<string> def() const override;
    set<unsigned> follow(unsigned index, const unsigned num_of_stmts) const override;
    Type type() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
    // updates jumps after removing dead code
    void adjustDC (const set<unsigned>& to_remove);
    
    // updates jumps after adding Load and Store
    void adjustLS (const vector<unsigned>& before, const vector<unsigned>& after);
    
private:
    Expression* m_condition;
    unsigned m_go;
    
    IfGo(const IfGo& d) = delete;
    IfGo& operator=(const IfGo& d) = delete;
};

class GoTo : public Stmt {
public:
    GoTo(int go)
    : m_go(go)
    {}
    
    set<string> use() const override;
    set<string> def() const override;
    set<unsigned> follow(unsigned index, const unsigned num_of_stmts) const override;
    Type type() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
    // updates jumps after removing dead code
    void adjustDC (const set<unsigned>& to_remove);
    
    // updates jumps after adding Load and Store
    void adjustLS (const vector<unsigned>& before, const vector<unsigned>& after);

private:
    unsigned m_go;
};

class Return : public Stmt {
public:
    Return(const string& id)
    : m_id(id)
    {}
    
    set<string> use() const override;
    set<string> def() const override;
    set<unsigned> follow(unsigned index, const unsigned num_of_stmts) const override;
    Type type() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
private:
    string m_id;
};

class Load : public Stmt {
public:
    Load(const string& id)
    : m_id(id)
    {}
    
    set<string> use() const override;
    set<string> def() const override;
    set<unsigned> follow(unsigned index, const unsigned num_of_stmts) const override;
    Type type() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
private:
    string m_id;
};

class Store : public Stmt {
public:
    Store(const string& id)
    : m_id(id)
    {}
    
    set<string> use() const override;
    set<string> def() const override;
    set<unsigned> follow(unsigned index, const unsigned num_of_stmts) const override;
    Type type() const override;
    void print() const override;
    void printRegs(const map<string,unsigned>& mapping) const override;
    
private:
    string m_id;
};

#endif
