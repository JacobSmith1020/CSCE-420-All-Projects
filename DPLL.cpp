#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "parser.hpp"

using namespace std;

int dpll_counter = 0;
unordered_map<string,bool> best_model;
int best_sat = 0;
//determine if a symbol exists within a cnf clause
// bool determine_if_in_clause(Expr* clause, string symbol) {
//     if(clause->kind == ATOM) {
//         if(clause->sym == symbol) {
//             return true;
//         }
//         else {
//             return false;
//         }
//     }
//     else if(clause->kind == LIST) {
//         for(int i = 0; i < clause->sub.size(); i++) {
//             if(clause->sub[i]->kind == LIST) {
//                 for(int j = 0; j < clause->sub[i]->sub.size(); j++) {
//                     if(clause->sub[i]->sub[j]->sym == symbol) {
//                         return true;
//                     }
//                 }
//             }
//             else if(clause->sub[i]->sym == symbol) {
//                 return true;
//             }
//         }
//         return false;
//     }
// }

//determine if a cnf clause is false, true or undetermined (0, 1, 2)
vector<int> determine_clause_truth(Expr* clause, unordered_map<string,bool> model) {
    bool undetermined = false;
    bool matched = false;
    string clause_variable = "";
    if(clause->kind == ATOM) {
        if(model.count(clause->sym) > 0) {
            if(model.at(clause->sym) == true) {
                return vector<int> {1};
            }
            else {
                return vector<int> {0};
            }
        }
    }
    else if(clause->kind == LIST) {
        for(int i = 0; i < clause->sub.size(); i++) {
            for(auto j: model) {
                if(clause->sub[i]->kind == LIST) {//not operation in clause
                    if((clause->sub[i]->sub[1]->sym == j.first) && (j.second == false)) {
                        return vector<int> {1};
                    }
                }
                else if((clause->sub[i]->sym == j.first) && (j.second == true)) {
                    return vector<int> {1};
                }
            }
        }
        for(int a = 1; a < clause->sub.size(); a++) {
            for(auto b: model) {
                if(clause->sub[a]->kind == LIST) {//not operation
                    clause_variable = clause->sub[a]->sub[1]->sym;
                    if(clause_variable == b.first) {
                        matched = true;
                    }
                }
                else {
                    clause_variable = clause->sub[a]->sym;
                    if(clause_variable == b.first) {
                        matched = true;
                    }
                }
            }
            if(matched == false) {
                undetermined = true;
            }
            matched = false;
        }
        if(undetermined == true) {
            return vector<int> {2};
        }
        else {
            return vector<int> {0};
        }
    }
}

bool dpll(vector<Expr*> clauses, unordered_map<string, bool> model, vector<string> symbols) {
    bool clauses_all_true = false;
    int clauses_true = 0;
    bool some_clause_false = false;
    bool undetermined = false;

    for(int i = 0; i < clauses.size(); i++) {
        if(determine_clause_truth(clauses[i], model) == vector<int> {1}) {//if all clauses are true in model return true
            clauses_all_true = true;
            clauses_true++;
        }
        else if(determine_clause_truth(clauses[i], model) == vector<int> {0}) {//if one clause is false in model return false
            clauses_all_true = false;
            some_clause_false = true;
            break;
        }
        else if(determine_clause_truth(clauses[i], model) == vector<int> {2}) {//some clauses are true and some are unable to be evaluated yet
            clauses_all_true = false;
            some_clause_false = false;
            undetermined = true;
        }
    }
    if(clauses_true > best_sat) {
        best_model = model;
        best_sat = clauses_true;
    }
    cout << "Model: " << endl;
    for(auto a: model) {
        cout << a.first << " = " << a.second << endl;
    }
    cout << "Number of clauses satisfied: " << clauses_true << endl << endl;
    if(some_clause_false == true) {
        return false;
    }
    else if(undetermined == true) {}
    else if(clauses_all_true == true) {
        return true;
    }
    
    //standard base recursion section
    if(symbols.size() == 0) {
        return false;
    }
    string p = symbols.back(); //assign p to the first symbol in the symbols vector
    symbols.pop_back(); //pop element assigned to p
    vector<string> rest = symbols; //assign rest vector to the remaining elements in symbol
    unordered_map<string, bool> model1 = model;
    model1[p] = true;
    unordered_map<string, bool> model2 = model;
    model2[p] = false;

    dpll_counter += 2;
    return (dpll(clauses, model1, rest) || dpll(clauses, model2, rest));
}

int main(int argc, char **argv) {
    string cnf_filename = "";
    bool uch = true;

    //parse command line arguments
    if(argc == 2) {
        cnf_filename = argv[1];
    }
    else if(argc == 3) {
        cnf_filename = argv[1];
        uch = false;
    }
    else {
        cout << "Incorrect number of arguments (should be <filename> [-unit])." << endl;
        exit(-1);
    }
    
    //load the cnf knowledge base
    vector<Expr*> kb = load_kb(cnf_filename);
    unordered_map<string, bool> MODEL;
    vector<string> symbols = {"NSWB", "NSWG", "NSWR", "NTB", "NTG", "NTR", "QB", "QG", "QR", "SAB", "SAG", "SAR", "TB", "TG", "TR", "VB", "VG", "VR", "WAB", "WAG", "WAR"};

    dpll_counter++;
    bool model_found = dpll(kb, MODEL, symbols);
    cout << endl << "Best model found: " << endl;
    for(auto a: best_model) {
        cout << a.first << " = " << a.second << endl;
    }
    cout << "Number of clauses satisfied: " << best_sat << endl;
    cout << "Number of times DPLL was called: " << dpll_counter << endl;
    cin.get();
    return 0;
}