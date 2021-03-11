#include "statement.hpp"
#include <algorithm>
#include <numeric>

template <typename T>
set<T> getUnion(const set<T>& first, const set<T>& second){
         set<T> result = first;
         result.insert(second.begin(), second.end());
         return result;
}


Stmt::~Stmt(){
    
}

/* 
 *  Assign
 */

set<string> Assign::use() const{
    return m_expression->returnVars();
}

set<string> Assign::def() const{
    return set<string> {m_id};
}

set<unsigned> Assign::follow(unsigned index, const unsigned num_of_stmts) const{
    if(index != num_of_stmts)
        return set<unsigned> { ++index };
    else
        return set<unsigned> {};
}

Stmt::Type Assign::type() const{
    return Type::ASSIGN;
}

void Assign::print() const{
    cout << m_id << " := ";
    m_expression->print();
    cout << endl;
}

void Assign::printRegs(const map<string,unsigned>& mapping) const{
    cout << "r" << mapping.at(m_id) << " := ";
    m_expression->printRegs(mapping);
    cout << endl;
}

bool Assign::del(const set<string>& out) const{    
    return out.find(m_id) == out.end();
}

Assign::~Assign(){
    delete m_expression;
}


/* 
 *  AssignF
 */

set<string> AssignF::use() const{
    set<string> vars;
    if  (m_arg->type() == Term::Type::ID){
        Id* id = dynamic_cast<Id*> (m_arg);
        vars.insert(id->value());
    }
    return vars;
}

set<string> AssignF::def() const{
    return set<string> {m_id};
}

set<unsigned> AssignF::follow(unsigned index, const unsigned num_of_stmts) const{
    if(index != num_of_stmts)
        return set<unsigned> { ++index };
    else
        return set<unsigned> {};
}

Stmt::Type AssignF::type() const{
    return Type::ASSIGN_F;
}

void AssignF::print() const{
    cout << m_id << " := " << m_fname << "(";
    m_arg->print();
    cout << ")" <<endl;
}

void AssignF::printRegs(const map<string,unsigned>& mapping) const{
    cout << "r" << mapping.at(m_id) << " := " << m_fname << "(";
    m_arg->printRegs(mapping);
    cout << ")" << endl;
}

bool AssignF::del(const set<string>& out) const{
    return out.find(m_id) == out.end();
}

AssignF::~AssignF(){
    delete m_arg;
}


/* 
 *  IfGo
 */

set<string> IfGo::use() const{
    return m_condition->returnVars();
}

set<string> IfGo::def() const{
    return set<string> {};
}

set<unsigned> IfGo::follow(unsigned index, const unsigned num_of_stmts) const{
    set<unsigned> next {m_go};
    
    if(index != num_of_stmts)
        next.insert(++index);
    return next;
}

Stmt::Type IfGo::type() const{
    return Type::IF_GO;
}

void IfGo::print() const{
    cout << "ifFalse ";
    m_condition->print();
    cout << " goto " << m_go << endl;
}

void IfGo::printRegs(const map<string,unsigned>& mapping) const{
    cout << "ifFalse ";
    m_condition->printRegs(mapping);
    cout << " goto " << m_go << endl;
}

void IfGo::adjustDC (const set<unsigned>& to_remove){
    m_go -= count_if(
                    to_remove.begin(),
                    to_remove.end(),
                    [&](unsigned elem){ return elem < m_go; }
                    );
}

void IfGo::adjustLS (const vector<unsigned>& before, const vector<unsigned>& after){
    m_go += accumulate(before.begin(),before.begin() + m_go, 0) +
            accumulate(after.begin(),after.begin() + m_go, 0);
}

IfGo::~IfGo(){
    delete m_condition;
}


/* 
 *  GoTo
 */

set<string> GoTo::use() const{
    return set<string> {};
}

set<string> GoTo::def() const{
    return set<string> {};
}

set<unsigned> GoTo::follow(__attribute__((unused))unsigned index, __attribute__((unused)) const unsigned num_of_stmts) const{
    return set<unsigned> {m_go};
}

Stmt::Type GoTo::type() const{
    return Type::GOTO;
}

void GoTo::print() const{
    cout << "goto " << m_go << endl;
}

void GoTo::printRegs(__attribute__((unused)) const map<string,unsigned>& mapping) const{
    print();
}

void GoTo::adjustDC (const set<unsigned>& to_remove){
    m_go -= count_if(
                    to_remove.begin(),
                    to_remove.end(),
                    [&](unsigned elem){ return elem < m_go; } 
                    );
}

void GoTo::adjustLS (const vector<unsigned>& before, const vector<unsigned>& after){
    m_go += accumulate(before.begin(),before.begin() + m_go, 0) +
            accumulate(after.begin(),after.begin() + m_go, 0);
}


/* 
 *  Return
 */

set<string> Return::use() const{
    return set<string> {m_id};
}

set<string> Return::def() const{
    return set<string> {};
}

set<unsigned> Return::follow(unsigned index, const unsigned num_of_stmts) const{
    if(index != num_of_stmts)
        return set<unsigned> { ++index };
    else
        return set<unsigned> {};
}

Stmt::Type Return::type() const{
    return Type::RETURN;
}

void Return::print() const{
    cout << "return " << m_id << endl;
}

void Return::printRegs(const map<string,unsigned>& mapping) const{
    cout << "return " << "r" << mapping.at(m_id) << endl;
}


/* 
 *  Load
 */

set<string> Load::use() const{
    return set<string> {};
}
    
set<string> Load::def() const{
    return set<string> {m_id};
}

set<unsigned> Load::follow(unsigned index, const unsigned num_of_stmts) const{
    if(index != num_of_stmts)
        return set<unsigned> { ++index };
    else
        return set<unsigned> {};
}
    
Stmt::Type Load::type() const{
    return Type::LOAD;
}
    
void Load::print() const{
    cout << m_id << " := ";
    cout << "M[" << m_id << "_loc]" << endl;
}
    
void Load::printRegs(const map<string,unsigned>& mapping) const{
    cout << "r" << mapping.at(m_id) << " := ";
    cout << "M[" << m_id << "_loc]" << endl;
}


/* 
 *  Store
 */

set<string> Store::use() const{
    return set<string> {m_id};
}
    
set<string> Store::def() const{
    return set<string> {};
}

set<unsigned> Store::follow(unsigned index, const unsigned num_of_stmts) const{
    if(index != num_of_stmts)
        return set<unsigned> { ++index };
    else
        return set<unsigned> {};
}
    
Stmt::Type Store::type() const{
    return Type::STORE;
}
    
void Store::print() const{
    cout << "M[" << m_id << "_loc] := " << m_id << endl;
}
    
void Store::printRegs(const map<string,unsigned>& mapping) const{
    cout << "M[" << m_id << "_loc] := r" << mapping.at(m_id) << endl;
}
