#include <iostream>
#include <vector>

#include "NatDed.hpp"

Expr* modus_ponens(Expr* ex1, Expr* ex2) {
    if((ex1->kind == ATOM) && (ex2->kind == LIST)) {
        if(ex1->sym == ex2->sub[1]->sym) {
            if(ex2->sub[2]->kind == LIST) {;
                Expr* implication = parse("(not " + ex2->sub[2]->sub[1]->sym + ")");
                return implication;
            }
            else {
                Expr* implication = parse(ex2->sub[2]->sym);
                return implication;
            }
        }
    }
    else if((ex1->kind == LIST) && (ex2->kind == ATOM)) {
        if(ex2->sym == ex1->sub[1]->sym) {
            if(ex1->sub[2]->kind == LIST) {;
                Expr* implication = parse("(not " + ex1->sub[2]->sub[1]->sym + ")");
                return implication;
            }
            else {
                Expr* implication = parse(ex1->sub[2]->sym);
                return implication;
            }
        }
    }
    else if ((ex1->kind == LIST) && (ex2->kind == LIST) && (ex2->sub[0]->sym == "not")){
        if(Eq(ex1->sub[1], ex2)) {
            Expr* implication = parse(ex1->sub[2]->sym);
            return implication;
        }
    }
    return ex1;
}

void modus_ponens_test() {
    Expr* s1 = parse("(implies P Q)");
    Expr* s2 = parse("P");
    Expr* s3 = modus_ponens(s2, s1);
    cout << "Modus ponens" << endl << "Given: (implies P Q), P" << endl << "Derived: ";
    cout << s3->toString() << endl << endl;
}

Expr* implication_elimination(Expr* ex1) {
    if(ex1->sub[0]->sym == "implies") {
        string s = "(or (not " + ex1->sub[1]->toString() + ") " + ex1->sub[2]->toString() + ")";
        Expr* implication = parse(s);
        return implication;
    }
    else if(ex1->sub[0]->sym == "or") {
        string s = "(implies (not " + ex1->sub[1]->toString() + ") " + ex1->sub[2]->toString() + ")";
        Expr* implication = parse(s);
        return implication;
    }
    return ex1;
}

void implication_elimination_test() {
    Expr* s1 = parse("(implies A (not B))");
    Expr* s2 = implication_elimination(s1);
    cout << "Implication elimination" << endl << "Given: (implies A (not B))" << endl << "Derived: ";
    cout << s2->toString() << endl << endl;
}

vector<Expr*> and_elimination(Expr* ex1) {
    vector<Expr*> implication;
    if(ex1->sub[0]->sym == "and") {
        for(int i = 1; i < ex1->sub.size(); i++) {
            implication.push_back(parse(ex1->sub[i]->sym));
        }
    }
    return implication;
}

void and_elimination_test() {
    Expr* s1 = parse("(and A B C)");
    vector<Expr*> s2 = and_elimination(s1);
    cout << "And elimination" << endl << "Given: (and A B C)" << endl << "Derived: ";
    for(int i = 0; i < s2.size(); i++) {
        cout << s2[i]->toString() << " ";
    }
    cout << endl << endl;
}

Expr* and_introduction(vector<Expr*> ex1) {
    string implication_s = "(and ";
    for(int i = 0; i < ex1.size(); i++) {
        implication_s += ex1[i]->sym;
        if(i != ex1.size() - 1) {
            implication_s += " ";
        }
    }
    implication_s += ")";
    Expr* implication = parse(implication_s);
    return implication;
}

void and_introduction_test() {
    vector<Expr*> s1 = {parse("A"), parse("B"), parse("C"), parse("D"), parse("E")};
    Expr* s2 = and_introduction(s1);
    cout << "And introduction" << endl << "Given: A, B, C, D, E" << endl << "Derived: ";
    cout << s2->toString() << endl << endl;
}

Expr* or_introduction(vector<Expr*> ex1) {
    string implication_s = "(or ";
    for(int i = 0; i < ex1.size(); i++) {
        implication_s += ex1[i]->sym;
        if(i != ex1.size() - 1) {
            implication_s += " ";
        }
    }
    implication_s += ")";
    Expr* implication = parse(implication_s);
    return implication;
}

void or_introduction_test() {
    vector<Expr*> s1 = {parse("A"), parse("B"), parse("C"), parse("D"), parse("E")};
    Expr* s2 = or_introduction(s1);
    cout << "Or introduction" << endl << "Given: A, B, C, D, E" << endl << "Derived: ";
    cout << s2->toString() << endl << endl;
}

Expr* double_negation_elimination(Expr* ex1) {
    if((ex1->sub[0]->sym == "not") && (ex1->sub[1]->sub[0]->sym == "not")) {
        Expr* implication = parse(ex1->sub[1]->sub[1]->sym);
        return implication;
    }
    return ex1;
}

void double_negation_elimination_test() {
    Expr* s1 = parse("(not (not A))");
    Expr* s2 = double_negation_elimination(s1);
    cout << "Double negation elimination" << endl << "Given: (not (not A))" << endl << "Derived: ";
    cout << s2->toString() << endl << endl;
}

Expr* resolution(Expr* ex1, Expr* ex2) {
    if((ex1->sub[0]->sym == "or") && (ex2->sub[0]->sym == "or")) {//both expressions are or
        if(ex1->sub[1]->kind == LIST) {
            if((ex1->sub[1]->sub[0]->sym == "not") && (ex1->sub[1]->sub[1]->sym == ex2->sub[1]->sym)) {
                Expr* implication = parse("(or " + ex1->sub[2]->toString() + " " + ex2->sub[2]->toString() + ")");
                return implication;
            }
        }
    }
    return ex1;
}

void resolution_test() {
    Expr* s1 = parse("(or (not A) B)");
    Expr* s2 = parse("(or A C)");
    Expr* s3 = resolution(s1, s2);
    cout << "Resolution" << endl << "Given: (or (not A) B), (or A C)" << endl << "Derived: ";
    cout << s3->toString() << endl << endl;
}

Expr* commutativity(Expr* ex1) {
    string s = "(" + ex1->sub[0]->sym + " " + ex1->sub[2]->toString() + " " + ex1->sub[1]->toString() + ")";
    Expr* implication = parse(s);
    return implication;
}

void commutativity_test() {
    Expr* s1 = parse("(and (not A) B)");
    Expr* s2 = commutativity(s1);
    cout << "Commutativity" << endl << "Given: (and (not A) B)" << endl << "Derived: ";
    cout << s2->toString() << endl << endl;
}