%{
#include <stdio.h>
#include "yacc.tab.h"
extern int yylval;
%}



%%
select					return SELECT;
from					return FROM;
sum						return SUM;
count					return COUNT;
max						return MAX;
min						return MIN;
avg						return AVG;
as						return AS;
where					return WHERE;
not						return NOT;
and						return AND;
or						return OR;
"=="					return COMPARISON;
">="					return COMPARISON;
"<="					return COMPARISON;
"<"						return COMPARISON;
">"						return COMPARISON;
"="						return COMPARISON;
"!="					return COMPARISON;
between					return BETWEEN;
like					return LIKE;
in						return IN;
exists					return EXISTS;
any						return ANY;
all						return ALL;
some					return SOME;
order					return ORDER;
by						return BY;
ASC						return ASC;
DESC					return DESC;
LIMIT					return LIMIT;
OFFSET					return OFFSET;
GROUP					return GROUP;
"("						return OPENBRACKET;
")"						return CLOSEBRACKET;
[*]						return *yytext;
[,]						return *yytext;
[a-zA-Z][a-zA-Z0-9]+	return IDENTIFIER;
[0-9]+					return DIGIT;
\n						return *yytext;
[ \t]+					/* ignore whitespace */;
%%
