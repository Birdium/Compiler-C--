%{
    #include <stdio.h>
    #include <stdarg.h>
    #include <string.h>
    #include <stdbool.h>
    #include "ast.h"
    #include "lex.yy.c"
    extern int yylex();
    extern int yyparse();    
    extern void yyerror(const char *);
    Node *root;
    bool syn_error = false;
%}
%define parse.error verbose
%union{
    Node *node;
};

%token<node> IF ELSE WHILE RETURN STRUCT TYPE
%token<node> INT FLOAT ID
%token<node> LC RC SEMI COMMA
%right<node> ASSIGNOP
%left<node> OR
%left<node> AND
%left<node> RELOP
%left<node> PLUS MINUS
%left<node> STAR DIV
%right<node> NOT 
%left<node> LP RP LB RB DOT

%type<node> Program
%type<node> ExtDefList ExtDef ExtDecList VarDec FunDec VarList ParamDec
%type<node> Specifier StructSpecifier
%type<node> OptTag Tag
%type<node> CompSt Stmt Stmt1 StmtList Def DefList Dec DecList Exp Args
%% 

Program: 
        ExtDefList                  {root = newNode(Program_NODE, $1, NULL);}
;

ExtDefList: 
        ExtDef ExtDefList           {$$ = newNode(ExtDefList_NODE, $1, $2, NULL);}
    |                               {$$ = newNode(Null_NODE, NULL);}
;

ExtDef: 
        Specifier ExtDecList SEMI   {$$ = newNode(ExtDef_NODE, $1, $2, $3, NULL);}
    |   Specifier SEMI              {$$ = newNode(ExtDef_NODE, $1, $2, NULL);}
    |   Specifier FunDec CompSt     {$$ = newNode(ExtDef_NODE, $1, $2, $3, NULL);}
    |   Specifier FunDec SEMI       {$$ = newNode(ExtDef_NODE, $1, $2, $3, NULL);}
    |   error SEMI                  {}
    |   Specifier error             {}
;

ExtDecList: 
        VarDec                      {$$ = newNode(ExtDecList_NODE, $1, NULL);}
    |   VarDec COMMA ExtDecList     {$$ = newNode(ExtDecList_NODE, $1, $2, $3, NULL);}
    |   VarDec error ExtDecList     {}
    |   VarDec error                {}
;


Specifier:
        TYPE                        {$$ = newNode(Specifier_NODE, $1, NULL);}
    |   StructSpecifier             {$$ = newNode(Specifier_NODE, $1, NULL);}
;

StructSpecifier:
        STRUCT OptTag LC DefList RC {$$ = newNode(StructSpecifier_NODE, $1, $2, $3, $4, $5, NULL);}
    |   STRUCT Tag                  {$$ = newNode(StructSpecifier_NODE, $1, $2, NULL);}
    |   STRUCT OptTag LC error RC   {}
;

OptTag:
        ID                          {$$ = newNode(OptTag_NODE, $1, NULL);}
    |                               {$$ = newNode(Null_NODE, NULL);}
;

Tag:
        ID                          {$$ = newNode(Tag_NODE, $1, NULL);}
;

VarDec:
        ID                          {$$ = newNode(VarDec_NODE, $1, NULL);}
    |   VarDec LB INT RB            {$$ = newNode(VarDec_NODE, $1, $2, $3, $4, NULL);}
    |   VarDec LB error RB          {}
    /* |   VarDec LB error RB          {yyerror("Missing array length"); yyerrok;} */
    |   VarDec LB error             {}
    /* |   VarDec LB error             {yyerror("Missing character: \"]\""); yyerrok;} */
;

FunDec:
        ID LP VarList RP            {$$ = newNode(FunDec_NODE, $1, $2, $3, $4, NULL);}
    |   ID LP RP                    {$$ = newNode(FunDec_NODE, $1, $2, $3, NULL);}
    |   ID LP error RP              {}
    |   error LP VarList RP         {}
    /* |   ID LP error                 {yyerror("Missing character: \")\""); yyerrok;} */
;

VarList:
        ParamDec COMMA VarList      {$$ = newNode(VarList_NODE, $1, $2, $3, NULL);}
    |   ParamDec                    {$$ = newNode(VarList_NODE, $1, NULL);}
    |   ParamDec error VarList      {}
;

ParamDec:
        Specifier VarDec            {$$ = newNode(ParamDec_NODE, $1, $2, NULL);}
;


CompSt:
        LC DefList StmtList RC      {$$ = newNode(CompSt_NODE, $1, $2, $3, $4, NULL);}
    |   LC error RC                 {}
;

StmtList:
        Stmt StmtList               {$$ = newNode(StmtList_NODE, $1, $2, NULL);}
    |                               {$$ = newNode(Null_NODE, NULL);}
;

Stmt:
        Exp SEMI                    {$$ = newNode(Stmt_NODE, $1, $2, NULL);}
    |   CompSt                      {$$ = newNode(Stmt_NODE, $1, NULL);}
    |   RETURN Exp SEMI             {$$ = newNode(Stmt_NODE, $1, $2, $3, NULL);}
    |   IF LP Exp RP Stmt1 ELSE Stmt{$$ = newNode(Stmt_NODE, $1, $2, $3, $4, $5, $6, $7, NULL);}
    |   IF LP Exp RP Stmt           {$$ = newNode(Stmt_NODE, $1, $2, $3, $4, $5, NULL);}
    |   WHILE LP Exp RP Stmt        {$$ = newNode(Stmt_NODE, $1, $2, $3, $4, $5, NULL);}
    /* |   Exp error                   {printf("114");yyerrok;} */
    |   error SEMI                  {}
    |   Exp error                   {}
;

Stmt1:
        Exp SEMI                     {$$ = newNode(Stmt_NODE, $1, $2, NULL);}
    |   CompSt                       {$$ = newNode(Stmt_NODE, $1, NULL);}
    |   RETURN Exp SEMI              {$$ = newNode(Stmt_NODE, $1, $2, $3, NULL);}
    |   IF LP Exp RP Stmt1 ELSE Stmt1{$$ = newNode(Stmt_NODE, $1, $2, $3, $4, $5, $6, $7, NULL);}
    |   WHILE LP Exp RP Stmt1        {$$ = newNode(Stmt_NODE, $1, $2, $3, $4, $5, NULL);}
    |   error SEMI                   {}
    |   Exp error                    {}
;

DefList: 
        Def DefList                 {$$ = newNode(DefList_NODE, $1, $2, NULL);}
    |                               {$$ = newNode(Null_NODE, NULL);}
;

Def:
        Specifier DecList SEMI      {$$ = newNode(Def_NODE, $1, $2, $3, NULL);}
    |   Specifier error SEMI        {}
;

DecList:
        Dec                         {$$ = newNode(DecList_NODE, $1, NULL);}
    |   Dec COMMA DecList           {$$ = newNode(DecList_NODE, $1, $2, $3, NULL);}
    |   Dec error DecList           {}
    |   Dec error                   {}
;

Dec:
        VarDec                      {$$ = newNode(Dec_NODE, $1, NULL);}
    |   VarDec ASSIGNOP Exp         {$$ = newNode(Dec_NODE, $1, $2, $3, NULL);}
    |   VarDec ASSIGNOP error       {}
;


Exp:
        Exp ASSIGNOP Exp            {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   Exp AND Exp                 {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   Exp OR Exp                  {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   Exp RELOP Exp               {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   Exp PLUS Exp                {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   Exp MINUS Exp               {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   Exp STAR Exp                {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   Exp DIV Exp                 {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   LP Exp RP                   {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   MINUS Exp                   {$$ = newNode(Exp_NODE, $1, $2, NULL);}
    |   NOT Exp                     {$$ = newNode(Exp_NODE, $1, $2, NULL);}
    |   ID LP Args RP               {$$ = newNode(Exp_NODE, $1, $2, $3, $4, NULL);}
    |   ID LP RP                    {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   Exp LB Exp RB               {$$ = newNode(Exp_NODE, $1, $2, $3, $4, NULL);}
    |   Exp DOT ID                  {$$ = newNode(Exp_NODE, $1, $2, $3, NULL);}
    |   ID                          {$$ = newNode(Exp_NODE, $1, NULL);}
    |   INT                         {$$ = newNode(Exp_NODE, $1, NULL);}
    |   FLOAT                       {$$ = newNode(Exp_NODE, $1, NULL);}
    |   Exp ASSIGNOP error          {}
    |   Exp AND error               {}
    |   Exp OR error                {}
    |   Exp RELOP error             {}
    |   Exp PLUS error              {}
    |   Exp MINUS error             {}
    |   Exp STAR error              {}
    |   Exp DIV error               {}
    |   LP error RP                 {}
    |   MINUS error                 {}
    |   NOT error                   {}
    |   ID LP error RP              {}
    |   Exp LB error RB             {}
;

Args:
        Exp COMMA Args              {$$ = newNode(Args_NODE, $1, $2, $3, NULL);}
    |   Exp                         {$$ = newNode(Args_NODE, $1, NULL);}
;

%%

void yyerror(const char *str){
    syn_error = true;
    printf("Error type B at Line %d: %s.\n", yylineno, str);
}
