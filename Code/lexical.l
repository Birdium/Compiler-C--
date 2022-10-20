%option yylineno

%{
    // #include <stdio.h>
    #include "syntax.tab.h"
    #include "ast.h"
    #include <stdbool.h>
    bool lex_error = false;
    // void IdentAssign(stmt *a, std::string *str){
    //     expr *tmp = new expr;
    //     tmp->type=Ident;
    //     tmp->firstArg.var=str;
    //     a->firstStm.exp=tmp;
    // }
    // void IdentAssign(expr *a, std::string *str){
    //     expr *tmp = new expr;
    //     tmp->type=Ident;
    //     tmp->firstArg.var=str;
    //     a->firstArg.exp=tmp;
    // }
%}


SPACE [ \t\n]+
DEC [1-9][0-9]*
HEX 0(x|X)[0-9a-fA-F]+
OCT 0[0-7]*
FLOAT [0-9]+\.[0-9]+
FLOAT_SCI ([0-9]+\.[0-9]*|[0-9]*\.[0-9]+)[Ee][+-]?[0-9]+
ID [a-zA-Z_][a-zA-Z0-9_]*

BLOCKCOMMENT_START \/\*
BLOCKCOMMENT_END "*"+"/"
COMMENT \/\/.*$

%x comment

%%


{SPACE} 
{COMMENT}                       
{BLOCKCOMMENT_START}            {BEGIN(comment);}
<comment>[^*\n]*                
<comment>\*+[^*/\n]*           
<comment>\n            
<comment>{BLOCKCOMMENT_END}     {BEGIN(INITIAL);}

";"                             {yylval.node = newNode(SEMI_NODE, yytext); return SEMI;}
","                             {yylval.node = newNode(COMMA_NODE, yytext); return COMMA;}
"="                             {yylval.node = newNode(ASSIGNOP_NODE, yytext); return ASSIGNOP;}
"<"                             {yylval.node = newNode(RELOP_NODE, yytext); return RELOP;}
">"                             {yylval.node = newNode(RELOP_NODE, yytext); return RELOP;}
"<="                            {yylval.node = newNode(RELOP_NODE, yytext); return RELOP;}
">="                            {yylval.node = newNode(RELOP_NODE, yytext); return RELOP;}
"=="                            {yylval.node = newNode(RELOP_NODE, yytext); return RELOP;}
"!="                            {yylval.node = newNode(RELOP_NODE, yytext); return RELOP;}

"+"                             {yylval.node = newNode(PLUS_NODE, yytext); return PLUS;}
"-"                             {yylval.node = newNode(MINUS_NODE, yytext); return MINUS;}
"*"                             {yylval.node = newNode(STAR_NODE, yytext); return STAR;}
"/"                             {yylval.node = newNode(DIV_NODE, yytext); return DIV;}
"&&"                            {yylval.node = newNode(AND_NODE, yytext); return AND;}
"||"                            {yylval.node = newNode(OR_NODE, yytext); return OR;}
"."                             {yylval.node = newNode(DOT_NODE, yytext); return DOT;}
"!"                             {yylval.node = newNode(NOT_NODE, yytext); return NOT;}

"int"                           {yylval.node = newNode(TYPE_NODE, yytext); return TYPE;}
"float"                         {yylval.node = newNode(TYPE_NODE, yytext); return TYPE;}
"struct"                        {yylval.node = newNode(STRUCT_NODE, yytext); return STRUCT;}
"return"                        {yylval.node = newNode(RETURN_NODE, yytext); return RETURN;}
"if"                            {yylval.node = newNode(IF_NODE, yytext); return IF;}
"else"                          {yylval.node = newNode(ELSE_NODE, yytext); return ELSE;}
"while"                         {yylval.node = newNode(WHILE_NODE, yytext); return WHILE;}

{ID}                            {yylval.node = newNode(ID_NODE, yytext); return ID;}   
{DEC}                           {yylval.node = newNode(DEC_NODE, yytext); return INT;}
{HEX}                           {yylval.node = newNode(HEX_NODE, yytext); return INT;}
{OCT}                           {yylval.node = newNode(OCT_NODE, yytext); return INT;}
{FLOAT}                         {yylval.node = newNode(FLOAT_NODE, yytext); return FLOAT;}
{FLOAT_SCI}                     {yylval.node = newNode(FLOAT_NODE, yytext); return FLOAT;}

"("                             {yylval.node = newNode(LP_NODE, yytext); return LP;}
")"                             {yylval.node = newNode(RP_NODE, yytext); return RP;}
"["                             {yylval.node = newNode(LB_NODE, yytext); return LB;}
"]"                             {yylval.node = newNode(RB_NODE, yytext); return RB;}
"{"                             {yylval.node = newNode(LC_NODE, yytext); return LC;}
"}"                             {yylval.node = newNode(RC_NODE, yytext); return RC;}

. 		                        {lex_error = true; printf("Error type A at line %d: Mysterious character \"%s\"\n", yylineno, yytext);}



%%

/* int main(){
    int tokentype;
    while (tokentype = yylex()) {
        printf("tokentype: %d", tokentype);
        if (tokentype == DEC || tokentype == HEX || tokentype == OCT ) {
            printf(", value = %d\n", yylval);
        } else if (tokentype == FLOATNUM) {
            printf(", value = %f\n", yylval);
        } else if (tokentype == IDENT) {
            printf(", value = %s\n", yylval);
        } else {
            printf("\n");
        }
    }
} */

