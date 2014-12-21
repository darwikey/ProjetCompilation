%{
  #include <stdio.h>
  #include <string>
  #include <vector>
  #include <iostream>
  #include "parser.h"
  #include "Declarator.hpp"


  extern "C"{
    int yyparse();
    int yylex ();
    int yyerror (char*);
  }

  using namespace std;

%}
%token <str> IDENTIFIER 
%token <int_num> ICONSTANT
%token <float_num> FCONSTANT
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token INT FLOAT VOID
%token IF ELSE WHILE RETURN FOR
%type <type> type_name 
%type <declarator> declarator parameter_declaration
%type <declarator_list> declarator_list parameter_list


%union {
  std::string* str;
  int int_num;
  float float_num;
  Declarator_type type;
  Declarator* declarator;
  std::vector<Declarator*>* declarator_list;
}
%start program
%%

primary_expression
: IDENTIFIER   //{cout << "id : " << *$1 << endl;}
| ICONSTANT   //{cout << "int const : " << $1 << endl;}
| FCONSTANT   //{cout << "float const : " << $1 << endl;}
| '(' expression ')'
| IDENTIFIER '(' ')'  {cout << "appelle fonction" << endl;}
| IDENTIFIER '(' argument_expression_list ')'   {cout << "appelle fonction parametre" << endl;}
| IDENTIFIER INC_OP {cout << "increment " << *$1 << endl;}
| IDENTIFIER DEC_OP
| IDENTIFIER '[' expression ']'
;

argument_expression_list
: expression
| argument_expression_list ',' expression
;

unary_expression
: primary_expression
| '-' unary_expression
| '!' unary_expression
;

multiplicative_expression
: unary_expression
| multiplicative_expression '*' unary_expression
;

additive_expression
: multiplicative_expression
| additive_expression '+' multiplicative_expression
| additive_expression '-' multiplicative_expression
;

comparison_expression
: additive_expression
| additive_expression '<' additive_expression
| additive_expression '>' additive_expression
| additive_expression LE_OP additive_expression
| additive_expression GE_OP additive_expression
| additive_expression EQ_OP additive_expression
| additive_expression NE_OP additive_expression
;

expression
: IDENTIFIER '=' comparison_expression
| IDENTIFIER '[' expression ']' '=' comparison_expression
| comparison_expression
;

declaration
: type_name declarator_list ';' {
  cout << "new var :\n \t type : "<< (int)$1 <<endl;
  for (Declarator* it: *$2){
    it->type = $1;
    cout << "\t name : " << it->name << endl;
    cout << "\t structure : " << (int)it->structure << endl;
  }}
;

declarator_list
  : declarator {$$ = new vector<Declarator*>(0); $$->push_back($1);} 
  | declarator_list ',' declarator {$$->push_back($3);}
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

parameter_list
: parameter_declaration {$$ = new vector<Declarator*>(); $$->push_back($1);}
| parameter_list ',' parameter_declaration {$$->push_back($3);}
;

parameter_declaration
: type_name declarator {
  for (Declarator* it: *$2){
    it->type = $1;
  }}
;

statement
: compound_statement
| expression_statement 
| selection_statement
| iteration_statement
| jump_statement
;

compound_statement
: '{' '}'
| '{' statement_list '}'
| '{' declaration_list statement_list '}'
;

declaration_list
: declaration
| declaration_list declaration
;

statement_list
: statement
| statement_list statement
;

expression_statement
: ';'
| expression ';'
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
: RETURN ';'
| RETURN expression ';'
;

program
: external_declaration
| program external_declaration
;

external_declaration
: function_definition
| declaration
;

function_definition
: type_name declarator compound_statement
;

%%
#include <stdio.h>
#include <string.h>

extern char yytext[];
extern int column;
extern int yylineno;
extern FILE *yyin;

char *file_name = NULL;

extern "C"{
  int yyerror (char *s) {
    fflush (stdout);
    cerr << file_name << ": " << yylineno << ":" << ": " << s;
    return 0;
  }
}

int main (int argc, char *argv[]) {
    FILE *input = NULL;
    if (argc==2) {
	input = fopen (argv[1], "r");
	file_name = strdup (argv[1]);
	if (input) {
	    yyin = input;
	    yyparse();
	}
	else {
	  cerr << argv[0] << ": Could not open " << argv[1] << endl;
	  return 1;
	}
	free(file_name);
    }
    else {
	cerr << "error: no input file" << endl;
	return 1;
    }
    return 0;
}
