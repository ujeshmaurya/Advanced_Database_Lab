
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     VARIABLE = 259,
     WHILE = 260,
     IF = 261,
     PRINT = 262,
     IFX = 263,
     ELSE = 264,
     NE = 265,
     EQ = 266,
     LE = 267,
     GE = 268,
     UMINUS = 269,
     SELECT = 270,
     FROM = 271,
     SUM = 272,
     COUNT = 273,
     MAX = 274,
     MIN = 275,
     AVG = 276,
     AS = 277,
     WHERE = 278,
     NOT = 279,
     AND = 280,
     OR = 281,
     COMPARISON = 282,
     IDENTIFIER = 283,
     BETWEEN = 284,
     LIKE = 285,
     IN = 286,
     EXISTS = 287,
     ANY = 288,
     ALL = 289,
     SOME = 290,
     OPENBRACKET = 291,
     CLOSEBRACKET = 292,
     ORDER = 293,
     BY = 294,
     ASC = 295,
     DESC = 296,
     DIGIT = 297,
     LIMIT = 298,
     OFFSET = 299,
     GROUP = 300
   };
#endif
/* Tokens.  */
#define INTEGER 258
#define VARIABLE 259
#define WHILE 260
#define IF 261
#define PRINT 262
#define IFX 263
#define ELSE 264
#define NE 265
#define EQ 266
#define LE 267
#define GE 268
#define UMINUS 269
#define SELECT 270
#define FROM 271
#define SUM 272
#define COUNT 273
#define MAX 274
#define MIN 275
#define AVG 276
#define AS 277
#define WHERE 278
#define NOT 279
#define AND 280
#define OR 281
#define COMPARISON 282
#define IDENTIFIER 283
#define BETWEEN 284
#define LIKE 285
#define IN 286
#define EXISTS 287
#define ANY 288
#define ALL 289
#define SOME 290
#define OPENBRACKET 291
#define CLOSEBRACKET 292
#define ORDER 293
#define BY 294
#define ASC 295
#define DESC 296
#define DIGIT 297
#define LIMIT 298
#define OFFSET 299
#define GROUP 300




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 19 "calc3.y"

    int iValue;                 /* integer value */
    char sIndex;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */



/* Line 1676 of yacc.c  */
#line 150 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

