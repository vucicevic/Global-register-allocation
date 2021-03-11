#include "term.hpp"

Term::~Term() {
    
}


/* 
 *  Id
 */

Term::Type Id::type() const{
    return Type::ID;
}

void Id::print() const{
    cout << m_id;
}

void Id::printRegs(const map<string,unsigned>& mapping) const{
    cout << "r" << mapping.at(m_id);
}
 
string Id::value() const{
    return m_id;
}


/* 
 *  Num
 */
    
Term::Type Num::type() const{
    return Type::NUM;
}

void Num::print() const{
    cout << m_num;
}

void Num::printRegs(__attribute__((unused)) const map<string,unsigned>& mapping) const{
    print();
}

int Num::value() const{
    return m_num;
}
