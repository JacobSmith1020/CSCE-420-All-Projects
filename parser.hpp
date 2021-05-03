#include <vector>
#include <string>
#include <stdexcept>

#ifndef PARSER_HPP
#define PARSER_HPP

using namespace std;

//----------------------------------

enum EXPR_TYPE { ATOM , LIST };

class Expr 
{
public:
  EXPR_TYPE kind;
  string sym;
  vector<Expr*> sub;
  int end;

  Expr(string s);
  Expr(vector<string> tokens,int start);
  Expr(Expr* e); // deep copy
  Expr(vector<Expr*> L); // create new COMPLEX expression from list
  string toString();
};

//----------------------------------

vector<string> tokenize(string s);

Expr* parse(string s);

// check for equality (overload the == operator?)

bool Eq(Expr* a, Expr* b);

vector<Expr*>load_kb(string fname);

void show_kb(vector<Expr*>& KB);

//----------------------------------

// for throwing exceptions...

class SyntaxError : public runtime_error
{
public:
  SyntaxError(string s);
};

class RuleApplicationError : public runtime_error
{
public:
  RuleApplicationError(string s);
};

#endif