#include "expression.hpp"

Expression::~Expression() {
    
}

/* 
 *  Unary
 */

set<string> Unary::returnVars() const{
    set<string> var_set;
    
    if (m_operand->type() == Term::Type::ID){
        Id* id = dynamic_cast<Id*> (m_operand);
        var_set.insert(id->value());
    }
    
    return var_set;
}
    
void Unary::print() const{
    m_operand->print();
}

void Unary::printRegs(const map<string,unsigned>& mapping) const{
    m_operand->printRegs(mapping);
}

Unary::~Unary(){
    delete m_operand;
}


/* 
 *  Binary
 */

set<string> Binary::returnVars() const{
    set<string> var_set;
    
    for(auto item : {m_left, m_right}){
        if (item->type() == Term::Type::ID) {
            Id* id = dynamic_cast<Id*> (item);
            var_set.insert(id->value());
        }
    }
    
    return var_set;
}

void Binary::print() const{
    m_left->print();
    cout << " " << m_operation << " ";
    m_right->print();
}

void Binary::printRegs(const map<string,unsigned>& mapping) const{
    m_left->printRegs(mapping);
    cout << " " << m_operation << " ";
    m_right->printRegs(mapping);
}

Binary::~Binary(){
    delete m_left;
    delete m_right;
}
