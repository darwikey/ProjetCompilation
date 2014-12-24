%{
  #include <stdio.h>
  #include <string>
  #include <vector>
  #include <iostream>
  #include <stdexcept>
  #include "parser.h"
  #include "Declarator.hpp"
  #include "Function.hpp"
  #include "Statement.hpp"
  #include "Expression.hpp"
  #include "Assignment.hpp"
  #include "Binary_expression.hpp"
  #include "Unary_expression.hpp"
  #include "Primary_expression.hpp"
  #include "Selection.hpp"
  #include "Iteration.hpp"
  #include "Jump.hpp"
  #include "Main_block.hpp"

  extern "C"{
    int yyparse();
    int yylex ();
    int yyerror (char*);
  }

  using namespace std;

  //**** variables globales ****
  // bloc principale contenant tous les autres blocs
  Main_block main_block;

%}
%token <str> IDENTIFIER 
%token <int_num> ICONSTANT
%token <float_num> FCONSTANT
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token INT FLOAT VOID
%token IF ELSE WHILE RETURN FOR

%type <type> type_name 
%type <declarator> declarator parameter_declaration
%type <function> function_definition
%type <declarator_list> declarator_list parameter_list declaration declaration_list
%type <block> compound_statement
%type <statement> statement
%type <statement_list> statement_list
%type <expression> expression_statement expression comparison_expression additive_expression multiplicative_expression unary_expression primary_expression
%type <selection> selection_statement
%type <iteration> iteration_statement
%type <jump> jump_statement
%type <function_parameter> argument_expression_list

%union {
  std::string* str;
  int int_num;
  float float_num;
  Declarator_type type;
  Declarator* declarator;
  Function* function;
  std::vector<Declarator*>* declarator_list;
  Block* block;
  Statement* statement;
  std::vector<Statement*>* statement_list;
  Expression* expression;
  Selection* selection;
  Iteration* iteration;
  Jump* jump;
  std::vector<Expression*>* function_parameter;
}

%start program
%%

primary_expression
: IDENTIFIER {
  $$ = new Primary_expression(Primary_type::VARIABLE, *$1);
}
| ICONSTANT {
  $$ = new Primary_expression((int)$1);
}
| FCONSTANT {
  $$ = new Primary_expression((float)$1);
} 
| '(' expression ')' {
  $$ = new Primary_expression(Primary_type::EXPRESSION, "", $2);
}
| IDENTIFIER '(' ')' {
  $$ = new Primary_expression(Primary_type::FUNCTION, vector<Expression*>());
}
| IDENTIFIER '(' argument_expression_list ')' {
  $$ = new Primary_expression(Primary_type::FUNCTION, *$3);
}
| IDENTIFIER INC_OP {
  $$ = new Primary_expression(Primary_type::INCREMENTED_VARIABLE, *$1);
}
| IDENTIFIER DEC_OP {
  $$ = new Primary_expression(Primary_type::DECREMENTED_VARIABLE, *$1);
}
| IDENTIFIER '[' expression ']' {
  $$ = new Primary_expression(Primary_type::ARRAY, *$1, $3);
}
;

argument_expression_list
: expression {
  $$ = new vector<Expression*>(); 
  $$->push_back($1);
}
| argument_expression_list ',' expression {
  $$ = $1;
  $$->push_back($3);
}
;

unary_expression
: primary_expression {$$ = $1;}
| '-' unary_expression {$$ = new Unary_expression(Unary_type::NEGATIVE, $2);}
| '!' unary_expression {$$ = new Unary_expression(Unary_type::NOT, $2);}
;

multiplicative_expression
: unary_expression {$$ = $1;}
| multiplicative_expression '*' unary_expression {$$ = new Binary_expression(Binary_type::MULTIPLICATION, $1, $3);}
;

additive_expression
: multiplicative_expression {$$ = $1;}
| additive_expression '+' multiplicative_expression {$$ = new Binary_expression(Binary_type::ADDITION, $1, $3);}
| additive_expression '-' multiplicative_expression {$$ = new Binary_expression(Binary_type::SUBTRACTION, $1, $3);}
;

comparison_expression
: additive_expression {$$ = $1;}
| additive_expression '<' additive_expression {$$ = new Binary_expression(Binary_type::LOWER, $1, $3);}
| additive_expression '>' additive_expression {$$ = new Binary_expression(Binary_type::GREATER, $1, $3);}
| additive_expression LE_OP additive_expression {$$ = new Binary_expression(Binary_type::LOWER_EQUAL, $1, $3);}
| additive_expression GE_OP additive_expression {$$ = new Binary_expression(Binary_type::GREATER_EQUAL, $1, $3);}
| additive_expression EQ_OP additive_expression {$$ = new Binary_expression(Binary_type::EQUAL, $1, $3);}
| additive_expression NE_OP additive_expression {$$ = new Binary_expression(Binary_type::NOT_EQUAL, $1, $3);}
;

expression
: IDENTIFIER '=' comparison_expression {$$ = new Assignment(*$1, $3);}
| IDENTIFIER '[' expression ']' '=' comparison_expression {$$ = new Assignment(*$1, $3, $6);}
| comparison_expression {$$ = $1;}
;

declaration
: type_name declarator_list ';' {
  $$ = $2;
  for (Declarator* it: *$2){
    it->type = $1;
    cout << "new declarator : " << *it << endl;
  }}
;

declarator_list
: declarator {$$ = new vector<Declarator*>(); $$->push_back($1);} 
| declarator_list ',' declarator {$$ = $1; $$->push_back($3);}
;

type_name
  : VOID {$$ = Declarator_type::VOID;}
  | INT  {$$ = Declarator_type::INT;}
  | FLOAT {$$ = Declarator_type::FLOAT;}
;

declarator
// variable int ou float
: IDENTIFIER {$$ = new Declarator(*$1, Declarator_structure::VARIABLE);}
//pointeur
| '*' IDENTIFIER {$$ = new Declarator(*$2, Declarator_structure::POINTER);}
//tableau
| IDENTIFIER '[' ICONSTANT ']' {$$ = new Declarator(*$1, Declarator_structure::ARRAY);}
// prototype de fonction
| declarator '(' parameter_list ')' {$$ = $1; $$->is_function = true; $$->parameter_list = *$3;}
| declarator '(' ')' {$$ = $1; $$->is_function = true;}
;

// liste de parametre pour un prototype de function
parameter_list
: parameter_declaration {$$ = new vector<Declarator*>(); $$->push_back($1);}
| parameter_list ',' parameter_declaration {$$ = $1; $$->push_back($3);}
;

// parametre pour un prototype de function
parameter_declaration
: type_name declarator {$$ = $2; $$->type = $1;}
;

statement
: compound_statement {$$ = $1;}
| expression_statement {$$ = $1;}  
| selection_statement {$$ = $1;}
| iteration_statement {$$ = $1;}
| jump_statement {$$ = $1;}
;

compound_statement
: '{' '}' {$$ = new Block();}
| '{' statement_list '}' {
  $$ = new Block();
  $$->add_statement(*$2);
}
| '{' declaration_list statement_list '}' {
  $$ = new Block();
  $$->add_declaration(*$2);
  $$->add_statement(*$3);
  cout<<"new block";
}
;

declaration_list
: declaration {$$ = $1;}
| declaration_list declaration {
  $$ = $1;
  $$->insert($$->end(), $2->begin(), $2->end()); // fusionne les deux listes
 }
;

statement_list
: statement {$$ = new vector<Statement*>(); $$->push_back($1);}
| statement_list statement {$$ = $1; $$->push_back($2);}
;

expression_statement
: ';' {$$ = new Expression();}
| expression ';' {$$ = $1;}
;

selection_statement
: IF '(' expression ')' statement
| IF '(' expression ')' statement ELSE statement
;

iteration_statement
: WHILE '(' expression ')' statement
| FOR '(' expression_statement expression_statement expression ')' statement
;

jump_statement
: RETURN ';' {$$ = new Jump();}
| RETURN expression ';'{$$ = new Jump($2);}
;

program
: external_declaration
| program external_declaration
;

// implementation des fonctions
external_declaration
: function_definition {main_block.add_function($1);}
| declaration {main_block.add_declaration(*$1);}
;

function_definition
: type_name declarator compound_statement {
  $2->type = $1;
  $$ = new Function($2, $3);
  cout << *$$ << endl;
}
;

%%
#include <stdio.h>
#include <string.h>
#include <fstream>

extern char yytext[];
extern int column;
extern int yylineno;
extern FILE *yyin;

string file_name;

extern "C"{
  int yyerror (char *s) {
    fflush (stdout);
    cerr << file_name << ":l" << yylineno << ": " << s << endl;
    return 0;
  }
}


int main (int argc, char *argv[]) {
  FILE *input = NULL;
  if (argc==2) {
    input = fopen (argv[1], "r");
    file_name = argv[1];

    if (input) {
      yyin = input;
      
      try{
	yyparse();

	ofstream output ("out.s", ios::out | ios::trunc);
	if (output){
	  output << main_block.get_code(vector<Block*>(), nullptr);

	  output.close();
	}
	else{
	  cerr << "unable to open output file" << endl;
	  return 1;
	}
      }
      catch(const exception& e){
	cerr << "error : " << e.what() << endl;
	return 1;
      }
    }
    else {
      cerr << argv[0] << ": Could not open " << argv[1] << endl;
      return 1;
    }
    
  }
  else {
    cerr << "error: no input file" << endl;
    return 1;
  }
    return 0;
}
