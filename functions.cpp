#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <iterator>
#include <vector>

#include "statement.hpp"

using namespace std;

// union of two sets
template <typename T>
set<T> getUnion(const set<T>& first, const set<T>& second){
    set<T> result = first;
    result.insert(second.begin(), second.end());
    return result;
}

    
// difference of two sets
template<typename T>
set<T> operator-(const set<T>& reference, const set<T>& items_to_remove){
    set<T> result;
    set_difference( reference.begin(), reference.end(),
                    items_to_remove.begin(), items_to_remove.end(),
                    inserter(result, result.end())
                  );
    return result;
}


/*  calculate sets IN and OUT for every stetement using next algorithm : 
 * 
 *      for each node n in CFG (Control-flow graph)
 *          in[n] = /  
 *          out[n] = /
 *      
 *      repeat
 *          for each node n in CFG in reverse topsort order
 *              in'[n] = in[n]
 *              out'[n] = out[n]
 *              out[n] = U in[s], where s in succ[n]
 *              in[n]  = use[n] U (out[n] \ def[n])
 *      until (in'[n]==in[n] and out'[n]==out[n]) for all n
 * 
 * 
 */

void insOuts(vector<set<string>>& in, vector<set<string>>& out, const vector<set<string>>& use, const vector<set<string>>& def, const vector<set<unsigned>>& follow) {
      
    vector<set<string>> inp (use.size());
    vector<set<string>> outp (use.size());
    
    bool done;
    
    do{
        
        done = true;
        
        for(int i = use.size()-1 ; i >= 0 ; i--){
            inp[i] = in[i];
            outp[i] = out[i];
            
            set<string> set_union;
            
            for(auto it = follow[i].begin() ; it!=follow[i].end() ; it++)
                set_union = getUnion(set_union,in[*it]);
        
            out[i] = set_union;
            
            set<string> difference = out[i] - def[i];
            in[i] = getUnion(use[i],difference);
        }
        
        for(int i=use.size()-1 ; i>=0 ; i--)
            if (!(inp[i]==in[i] && outp[i]==out[i])) {
                done = false;
                break;
            }
    
    }
    while(!done);       

}

/*
    using IN and OUT sets we make interference graph 
        
    first we group all IN and OUT set into one dependency set
        
    Example :
        if one entry in dependency set is : {a,b,c,d} 
        and current graph is :
        
        graph['a'] = {e}
        graph['b'] = {}
        graph['c'] = {}
        graph['d'] = {a,c}
        
        we update graph to this :
                                    *it     element         diff
                                     ||       ||             ||
                                     \/       \/             \/
        graph['a'] = U(graph['a'],{a,b,c,d} \ {a}) = U({e},{b,c,d})   = {b,c,d,e}
        graph['b'] = U(graph['b'],{a,b,c,d] \ {b}) = U({},{a,c,d})    = {a,c,d}
        graph['c'] = U(graph['c'],{a,b,c,d} \ {c}) = U({},{a,b,d})    = {a,b,d}
        graph['d'] = U(graph['d'],{a,b,c,d} \ {d}) = U({a,c},{a,b,c}) = {a,b,c}
    */
map<string,set<string>> build (const vector<set<string>>&in, const vector<set<string>>& out){
        
    set<set<string>> dependency_set;
    
    // group all IN and OUT into one set
    for(unsigned i = 0 ; i < in.size() ; i++){
        if (!in[i].empty())
            dependency_set.insert(in[i]);
        if (!out[i].empty())
            dependency_set.insert(out[i]);
    }
    
    map<string,set<string>> graph;
    
    for(auto it = dependency_set.begin() ; it != dependency_set.end() ; it++)
        for(auto element = it->begin() ; element != it->end() ; element++){
            set<string> diff = *it - set<string> {*element};
            graph[*element] = getUnion(graph[*element],diff);
         }
         
    return graph;

}

// simplify and spill
void simplify_spill(map<string,set<string>>& graph, stack<string>& var_stack, const unsigned num_of_regs) {

    // last element will go directly to the stack and will finish with recursion
    if(graph.size() == 1){
        auto it = graph.begin();
        var_stack.push(it->first);
        graph.erase(it->first);
        return;
    }
    
    string candidate;
    bool possible_spill = true;
     
    // candidate is a variable with cardinality < num_regs
    // if there isn't one we possibly have a spill
    for(auto it = graph.begin() ; it != graph.end() ; it++)
        if(it->second.size() < num_of_regs){
            candidate = it->first;
            var_stack.push(candidate);
            possible_spill = false;
            break;
        }
        
         
    // we choose first one to be our candidate
    if(possible_spill){
        auto it = graph.begin();
        candidate = it->first;
        var_stack.push(candidate);
    }
        
    // update graph and do this recursively until the graph has elements
    graph.erase(candidate);
    for(auto it = graph.begin() ; it != graph.end() ; it++)
        graph[it->first].erase(candidate);
        
    simplify_spill(graph,var_stack,num_of_regs);
    

}

// Select - for given stack of variables we try to associate registers to each of them (this is similar to graph coloring problem)
// in case that this is impossible, we return spilled_vars and we need to spill some of them (spill = create Load and Store for it)
void select(map<string,unsigned>& mapping,stack<string>& var_stack,const map<string,set<string>>& graph, const set<unsigned>& registers, set<string>& spilled_vars){
    
    // if stack is empty we finish 
    if(var_stack.empty())
        return;
    
    string candidate = var_stack.top();
        
    // calculating taken registers
    set<unsigned> unavailable_regs;
    
    for(auto it = graph.at(candidate).begin() ; it != graph.at(candidate).end() ; it++)
        if(mapping[*it] != 0)
            unavailable_regs.insert(mapping[*it]);
        
    // if there is available register, candidate gets it..if not we must spill that variable
    set<unsigned> available_regs = registers - unavailable_regs;
    
    if(!available_regs.empty()){
        auto it = available_regs.begin();
        mapping[candidate] = *it;
    }
    else{
        spilled_vars.insert(candidate);
    }
    
    // update stack and call recursively
    var_stack.pop();
    select(mapping,var_stack,graph,registers,spilled_vars);
    
}

// this will delete dead assigns
void deadCode(vector<Stmt*>& statements, vector<set<string>>& in, vector<set<string>>& out) {
    
    // using OUT sets we can determine redundant assigns (which will be deleted)
    set<unsigned> to_remove;
    for(unsigned i = 0 ; i < statements.size() ; i++){
        if(statements[i]->type() == Stmt::Type::ASSIGN){
             Assign* assign = dynamic_cast<Assign*> (statements[i]);
             if(assign->del(out[i]))
                 to_remove.insert(i);
        }
        else if(statements[i]->type()== Stmt::Type::ASSIGN_F){
             AssignF* assignf = dynamic_cast<AssignF*> (statements[i]);
             if(assignf->del(out[i]))
                 to_remove.insert(i);
        }
    }
    
    // we must update the jumps if we deleted something
    if(!to_remove.empty()){
        for(unsigned i = 0 ; i < statements.size() ; i++){
            if(statements[i]->type() == Stmt::Type::IF_GO){
                IfGo* ifgo = dynamic_cast<IfGo*> (statements[i]);
                ifgo->adjustDC(to_remove);
            }
            else if(statements[i]->type() == Stmt::Type::GOTO){
                GoTo* go = dynamic_cast<GoTo*> (statements[i]);
                go->adjustDC(to_remove);
            }
        }
    
        unsigned removed = 0;
        
        for(auto it = to_remove.begin() ; it != to_remove.end() ; it++){
            delete statements[*it-removed];
            statements.erase(statements.begin()+*it-removed);
            in.erase(in.begin()+*it-removed);
            out.erase(out.begin()+*it-removed);
            removed++;
        }
    }
    
}

// function updates jumps after adding Load and Store instructions 
void updateLS(const vector<unsigned>& before, const vector<unsigned>& after, vector<Stmt*> statements){
     
    for(unsigned i = 0 ; i < statements.size() ; i++){
        if(statements[i]->type() == Stmt::Type::IF_GO){
            IfGo* ifgo = dynamic_cast<IfGo*> (statements[i]);
            ifgo->adjustLS(before,after);
        } 
        else if(statements[i]->type() == Stmt::Type::GOTO){
            GoTo* go = dynamic_cast<GoTo*> (statements[i]);
            go->adjustLS(before,after);
        }
    }
}


void printStmts(const string& message, const vector<Stmt*>& statements) {
    
    cout << "\t" << message << endl << endl;
    
    for(unsigned i = 0 ; i < statements.size() ; i++){
        cout << i << "\t";
        statements[i]->print();
    }
    
    cout << endl;
    cout << "----------------------------------------------" << endl;
}

