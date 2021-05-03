#include <iostream>
#include <vector>
#include <string>
#include "parser.hpp"

// check if two expressions are resolvable
bool resolvable(Expr* ex1, Expr* ex2) {
    string ex1_string = "";
    bool ex1_not = false;
    for(int i = 0; i < ex1->sub.size(); i++) {
        if(ex1->sub[i]->kind == LIST) {// not variable in expression
            ex1_string = ex1->sub[i]->sub[1]->sym;
            ex1_not = true;
        }
        else{
            ex1_string = ex1->sub[i]->sym;
            ex1_not = false;
        }
        for(int j = 0; j < ex2->sub.size(); j++) {
            if(ex2->sub[j]->kind == LIST) {// not variable in expression
                if((ex1_string == ex2->sub[j]->sub[1]->sym) && (ex1_not == false)) {
                    return true;
                }
            }
            else {
                if((ex1_string == ex2->sub[j]->sym) && (ex1_not == true)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// resolution rule of inference function
Expr* resolution(Expr* ex1, Expr* ex2) {
    string ex1_string = "";
    bool ex1_not = false;
    vector<string> remaining_1;
    vector<string> remaining_2;
    string remaining_final = "(or ";
    vector<int> pop_index;
    for(int i = 1; i < ex1->sub.size(); i++) {
        if(ex1->sub[i]->kind == LIST) {// not variable in expression
            if((ex1->sub[i]->sub[0]->sym == "not") && (ex1->sub[i]->sub[1]->kind == LIST)) { // (or (not C1W) (not (or C2W C3W)))
                for(int x = 1; x < ex1->sub[i]->sub[1]->sub.size(); x++) {
                    remaining_1.push_back("(not " + ex1->sub[i]->sub[1]->sub[x]->sym + ")");
                }
            }
            else if(ex1->sub[i]->sub[0]->sym == "not") {
                ex1_string = ex1->sub[i]->sub[1]->sym;
                ex1_not = true;
                remaining_1.push_back("(not " + ex1_string + ")");
            }
            else {
                for(int x = 1; x < ex1->sub[i]->sub.size(); x++) {
                    remaining_1.push_back(ex1->sub[i]->sub[x]->sym);
                }
                ex1_not = false;
            }
        }
        else{
            ex1_string = ex1->sub[i]->sym;
            ex1_not = false;
            remaining_1.push_back(ex1_string);
        }
        for(int j = 1; j < ex2->sub.size(); j++) {
            if(ex2->sub[j]->kind == LIST) {// not variable in expression
                if(i == 1) {
                    if((ex2->sub[j]->sub[0]->sym == "not") && (ex2->sub[j]->sub[1]->kind == LIST)) { // (or (not C1W) (not (or C2W C3W)))
                        for(int x = 1; x < ex2->sub[j]->sub[1]->sub.size(); x++) {
                            remaining_2.push_back("(not " + ex2->sub[j]->sub[1]->sub[x]->sym + ")");
                        }
                    }
                    else if(ex2->sub[j]->sub[0]->sym == "not") {
                        remaining_2.push_back("(not " + ex2->sub[j]->sub[1]->sym + ")");
                    }
                    else {
                        for(int x = 1; x < ex2->sub[j]->sub.size(); x++) {
                            remaining_2.push_back(ex2->sub[j]->sub[x]->sym);
                        }
                    }
                }
                if((ex1_string == ex2->sub[j]->sub[1]->sym) && (ex1_not == false)) {
                    if(!remaining_1.empty()) {
                        remaining_1.pop_back();
                        pop_index.push_back(j - 1);
                        //remaining_2.pop_back();
                    }
                }
            }
            else {
                if(i == 1) {
                    remaining_2.push_back(ex2->sub[j]->sym);
                }
                if((ex1_string == ex2->sub[j]->sym) && (ex1_not == true)) {
                    if(!remaining_1.empty()) {
                        remaining_1.pop_back();
                        pop_index.push_back(j - 1);
                        //remaining_2.pop_back();
                    }
                }
            }
        }
    }  

    //pop remaining variables in ex2 that are being resolved
    for(int a = 0; a < pop_index.size(); a++) {
        if(pop_index[a] >= 0) {
            remaining_2.erase(remaining_2.begin() + pop_index[a]);
        }
        if(a != pop_index.size() - 1) {
            pop_index[a + 1] -= 1;
        }
    }

    //assemble new variables
    for(int k = 0; k < remaining_1.size(); k++) {
        if((k == remaining_1.size() - 1) && (remaining_2.size() == 0)) {
            remaining_final += remaining_1[k] + ")";
        }
        else {
            remaining_final += remaining_1[k] + " ";
        }
    }
    for(int l = 0; l < remaining_2.size(); l++) {
        if(l == remaining_2.size() - 1) {
            remaining_final += remaining_2[l] + ")";
        }
        else {
            remaining_final += remaining_2[l] + " ";
        }
    }

    if(remaining_1.empty() && remaining_2.empty()) {
        return nullptr;
    }
    //cout << remaining_final << endl;
    return parse(remaining_final);
}

// main resolution refutation proof function
bool resolution_refutation(vector<Expr*> kb, Expr* clause) {
    if(clause->kind == LIST) {
        kb.push_back(clause);
    }
    else {
        kb.push_back(parse("(not " + clause->toString() + ")"));
    }
    vector<Expr*> q;
    Expr* resolvents;
    bool subset = false;
    int iterations = 0;
    while(iterations <= 10000) {
        iterations++;
        for(int i = 0; i < kb.size(); i++) {
            for(int j = i + 1; j < kb.size(); j++) {
                if(resolvable(kb[i], kb[j])) {
                    cout << "iterations = " << iterations << " " << "clauses = " << kb.size() << endl;
                    cout << "resolving clauses " << kb[i]->toString() << " and " << kb[j]->toString() << endl;
                    resolvents = resolution(kb[i], kb[j]);
                    cout << "resolvent = " << resolvents->toString() << endl;
                    q.push_back(resolvents);
                }
                if(resolvents == nullptr) { // if resolvents contains the empty clause return true
                    return true;
                }
            }
        }
        //if(q is a subset of kb) return false
        for(int m = 0; m < q.size(); m++) {
            for(int n = 0; n < kb.size(); n++) {
                if(q[m]->toString() == kb[n]->toString()) {
                    subset = true;
                    break;
                }
            }
            if(subset == false) { // an element in q is not contained within kb; not a subset
                break;
            }
            else if((m == q.size() - 1) && (subset == true)) {
                return false;
            }
        }
        for(int k = 0; k < q.size(); k++) {
            kb.push_back(q[k]);
        }
        q.clear();
    }
    return false; // this should only fire if max iterations is reached
}

int main(int argc, char **argv) {
    string cnf_filename = "";
    string clause = "";

    //parse command line arguments
    if(argc == 3) {
        cnf_filename = argv[1];
        clause = argv[2];
    }
    else {
        cout << "Incorrect number of arguments (should be <filename> <clause>)." << endl;
        exit(-1);
    }
    vector<Expr*> kb = load_kb(cnf_filename);
    //Expr* res = resolution(parse("(or (not P) Q)"), parse("(or (not Q))"));
    //Expr* res = resolution(parse("(or (not O3Y) (not (or C3Y C3B)))"), parse("(or O3Y L3B)"));

    bool res_ref = resolution_refutation(kb, parse(clause));
    cout << res_ref << endl;

    //cin.get();
    return 0;
}