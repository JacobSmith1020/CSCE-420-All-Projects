#include "parser.hpp"
#include <vector>
#ifndef NATDED_HPP
#define NATDED_HPP

using namespace std;

Expr* modus_ponens(Expr* ex1, Expr* ex2);

void modus_ponens_test();

Expr* implication_elimination(Expr* ex1);

void implication_elimination_test();

vector<Expr*> and_elimination(Expr* ex1);

void and_elimination_test();

Expr* and_introduction(vector<Expr*> ex1);

void and_introduction_test();

Expr* or_introduction(vector<Expr*> ex1);

void or_introduction_test();

Expr* double_negation_elimination(Expr* ex1);

void double_negation_elimination_test();

Expr* resolution(Expr* ex1, Expr* ex2);

void resolution_test();

Expr* commutativity(Expr* ex1);

void commutativity_test();

#endif