%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "calc3.h"

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(int i);
nodeType *con(int value);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);

void yyerror(char *s);
int sym[26];                    /* symbol table */
%}

%union {
    int iValue;                 /* integer value */
    char sIndex;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <sIndex> VARIABLE
%token WHILE IF PRINT
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS


%%

%token SELECT FROM SUM COUNT MAX MIN AVG AS WHERE NOT AND OR COMPARISON IDENTIFIER BETWEEN LIKE IN EXISTS ANY ALL SOME OPENBRACKET CLOSEBRACKET
       ORDER BY ASC DESC DIGIT LIMIT OFFSET GROUP;

line : Query '\n' { printf("Syntax Correct\n"); };

Query : SELECT select_expr FROM table_list where_list groupby_list orderby_list limit_list ;

select_expr : '*' | attribute_name ;

attribute_name : Attribute ',' attribute_name  | Attribute  | attribute_name_alias
                 | SUM OPENBRACKET Attribute CLOSEBRACKET AS AliasName | SUM OPENBRACKET Attribute CLOSEBRACKET 
                 | SUM OPENBRACKET Attribute CLOSEBRACKET AS AliasName ',' attribute_name| SUM OPENBRACKET Attribute CLOSEBRACKET ',' attribute_name
                 | COUNT OPENBRACKET Attribute CLOSEBRACKET AS AliasName | COUNT OPENBRACKET Attribute CLOSEBRACKET
                 | COUNT OPENBRACKET Attribute CLOSEBRACKET AS AliasName ',' attribute_name | COUNT OPENBRACKET Attribute CLOSEBRACKET ',' attribute_name 
                 | MAX OPENBRACKET Attribute CLOSEBRACKET AS AliasName | MAX OPENBRACKET Attribute CLOSEBRACKET 
                 | MAX OPENBRACKET Attribute CLOSEBRACKET AS AliasName ',' attribute_name | MAX OPENBRACKET Attribute CLOSEBRACKET ',' attribute_name 
                 | MIN OPENBRACKET Attribute CLOSEBRACKET AS AliasName | MIN OPENBRACKET Attribute CLOSEBRACKET
                 | MIN OPENBRACKET Attribute CLOSEBRACKET AS AliasName ',' attribute_name| MIN OPENBRACKET Attribute CLOSEBRACKET ',' attribute_name 
                 | AVG OPENBRACKET Attribute CLOSEBRACKET AS AliasName | AVG OPENBRACKET Attribute CLOSEBRACKET
                 | AVG OPENBRACKET Attribute CLOSEBRACKET AS AliasName ',' attribute_name | AVG OPENBRACKET Attribute CLOSEBRACKET ',' attribute_name ;

attribute_name_alias : Attribute AS AliasName | Attribute AS AliasName ',' attribute_name;

table_list : TableName ',' table_list | TableName ; 

where_list : WHERE condition_hint | WHERE NOT condition_hint | ;

condition_hint : condition_list AND condition_hint | condition_list OR condition_hint | condition_list;

condition_list : Attribute COMPARISON IDENTIFIER | Attribute BETWEEN IDENTIFIER AND IDENTIFIER | OPENBRACKET Attribute BETWEEN IDENTIFIER AND IDENTIFIER CLOSEBRACKET | Attribute LIKE Pattern;

condition_list : Attribute IN OPENBRACKET attribute_list CLOSEBRACKET | Attribute NOT IN OPENBRACKET attribute_list CLOSEBRACKET | Attribute IN OPENBRACKET Query CLOSEBRACKET 
                | Attribute NOT IN OPENBRACKET Query CLOSEBRACKET | EXISTS OPENBRACKET Query CLOSEBRACKET ;

attribute_list : Attribute ','attribute_list | Attribute ;

condition_list : Attribute COMPARISON OPENBRACKET Query CLOSEBRACKET | Attribute COMPARISON ANY OPENBRACKET Query CLOSEBRACKET | Attribute COMPARISON SOME OPENBRACKET Query CLOSEBRACKET
                 | Attribute COMPARISON ALL OPENBRACKET Query CLOSEBRACKET ;
                 
orderby_list : ORDER BY order_expr | ;

order_expr : Attribute ',' order_expr | Attribute ASC | Attribute DESC
             |Attribute ASC ',' order_expr | Attribute DESC ',' order_expr ;
             
limit_list : LIMIT limit_expr | ;

limit_expr : DIGIT | DIGIT OFFSET DIGIT ; 

groupby_list : GROUP BY groupby_expr | ;

groupby_expr : Attribute ',' groupby_expr | Attribute ;

Pattern   : IDENTIFIER ;
TableName : IDENTIFIER ;
AliasName : IDENTIFIER ;
Attribute : IDENTIFIER ;    
%%

nodeType *con(int value) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.value = value;

    return p;
}

nodeType *id(int i) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.i = i;

    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node, extending op array */
    if ((p = malloc(sizeof(nodeType) + (nops-1) * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free (p);
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}
