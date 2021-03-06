D[0-9]
L[a-zA-Z_]

%{
#include <stdio.h>
#include <string>
#include <vector>
#include "Declarator.hpp"
#include "Function.hpp"
#include "Block.hpp"
#include "Statement.hpp"
#include "Expression.hpp"
#include "Assignment.hpp"
#include "Binary_expression.hpp"
#include "Unary_expression.hpp"
#include "Primary_expression.hpp"
#include "Selection.hpp"
#include "Iteration.hpp"
#include "While.hpp"
#include "For.hpp"
#include "Vectorized_for.hpp"
#include "Jump.hpp"
#include "parser.tab.h"

extern "C"{
int yylex(void);
}

#define YYSTYPE


void count();
int comment ();
int check_type ();
%}

%option yylineno
%%
"/*"		{ comment(); }
"//".*          { count(); }
"float"		{ count(); return(FLOAT); }
"if"		{ count(); return(IF); }
"else"		{ count(); return(ELSE); }
"int"		{ count(); return(INT); }
"return"	{ count(); return(RETURN); }
"void"		{ count(); return(VOID); }
"while"		{ count(); return(WHILE); }
"for"		{ count(); return(FOR); }
"#pragma omp simd" {count(); return(PRAGMA_SIMD);}
"reduction"     { count(); return(REDUCTION); }

{L}({L}|{D})*	{ count(); yylval.str=new std::string(yytext); return(IDENTIFIER); }

{D}+    	{ count(); yylval.int_num = atoi(yytext); return(ICONSTANT); }
{D}+"."{D}*     { count(); yylval.float_num = atof(yytext); return(FCONSTANT);}
"++"		{ count(); return(INC_OP); }
"--"		{ count(); return(DEC_OP); }
"<="		{ count(); return(LE_OP); }
">="		{ count(); return(GE_OP); }
"=="		{ count(); return(EQ_OP); }
"!="		{ count(); return(NE_OP); }
";"		{ count(); return(';'); }
"{"      	{ count(); return('{'); }
"}"      	{ count(); return('}'); }
","		{ count(); return(','); }
":"		{ count(); return(':'); }
"="		{ count(); return('='); }
"("		{ count(); return('('); }
")"		{ count(); return(')'); }
"["     	{ count(); return('['); }
"]"     	{ count(); return(']'); }
"."		{ count(); return('.'); }
"!"		{ count(); return('!'); }
"-"		{ count(); return('-'); }
"+"		{ count(); return('+'); }
"*"		{ count(); return('*'); }
"<"		{ count(); return('<'); }
">"		{ count(); return('>'); }
[ \t\v\n\f]	{ count(); }
.		{ /* ignore bad characters */ }

%%

int yywrap() {
	return 1;
}

int comment() {
	char c, c1;

loop:
	while ((c = yyinput()) != '*' && c != 0);
	if ((c1 = yyinput()) != '/' && c != 0) {
		unput(c1);
		goto loop;
	}
	return 0;
}


int column = 0;

void count() {
	int i;

	for (i = 0; yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;
	}
	//ECHO;
}

