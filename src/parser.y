%{
  #include <stdio.h>
  #include <string>
  #include <iostream>
  extern int yylineno;

  extern "C"{
    int yyparse();
    int yylex ();
    int yyerror (char*);
  }


  typedef enum{
    TYPE_INT, TYPE_FLOAT, TYPE_FUNCTION
  }declarator_type;
%}

%token <str> IDENTIFIER 
%token <int_num> ICONSTANT
%token <float_num> FCONSTANT
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token INT FLOAT VOID
%token IF ELSE WHILE RETURN FOR
%union {
  std::string *str;
  int int_num;
  float float_num;
  //decl_type;
}
%start program
%%

primary_expression
: IDENTIFIER   {std::cout << "id : " << *$1 << std::endl;}
| ICONSTANT   {printf("int const : %d\n", $1);}
| FCONSTANT   {printf("float const : %f\n", $1);}
| '(' expression ')'
| IDENTIFIER '(' ')'  {printf("appelle fonction\n");}
| IDENTIFIER '(' argument_expression_list ')'   {printf("appelle fonction parametre\n");}
| IDENTIFIER INC_OP {std::cout << "increment " << *$1 << std::endl;}
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
: type_name declarator_list ';'
;

declarator_list
: declarator
| declarator_list ',' declarator 
;

type_name
: VOID 
| INT  
| FLOAT
;

declarator
: IDENTIFIER  
| '*' IDENTIFIER 
| IDENTIFIER '[' ICONSTANT ']'
| declarator '(' parameter_list ')'
| declarator '(' ')' 
;

parameter_list
: parameter_declaration
| parameter_list ',' parameter_declaration
;

parameter_declaration
: type_name declarator
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
    fprintf (stderr, "%s:%d:%d: %s\n", file_name, yylineno, column, s);
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
	  fprintf (stderr, "%s: Could not open %s\n", *argv, argv[1]);
	    return 1;
	}
	free(file_name);
    }
    else {
	fprintf (stderr, "%s: error: no input file\n", *argv);
	return 1;
    }
    return 0;
}
