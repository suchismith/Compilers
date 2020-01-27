/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    FLOAT = 259,
    STRING = 260,
    INTDECL = 261,
    UINTDECL = 262,
    FLOATDECL = 263,
    STRDECL = 264,
    CHARDECL = 265,
    BOOLDECL = 266,
    VOIDDECL = 267,
    IDNT = 268,
    IMPORT = 269,
    MUL_ASSIGN = 270,
    DIV_ASSIGN = 271,
    ADD_ASSIGN = 272,
    SUB_ASSIGN = 273,
    GE_OP = 274,
    LE_OP = 275,
    AND_OP = 276,
    OR_OP = 277,
    LEFT_OP = 278,
    INC_OP = 279,
    EQ_OP = 280,
    NE_OP = 281,
    DEC_OP = 282,
    CASE = 283,
    IF = 284,
    ELSE = 285,
    SWITCH = 286,
    WHILE = 287,
    FOR = 288,
    CONTINUE = 289,
    BREAK = 290,
    RETURN = 291,
    CONST = 292,
    DO = 293,
    INTCONSTANT = 294,
    CHARCONSTANT = 295,
    STRINGCONSTANT = 296,
    FLOATCONSTANT = 297,
    BOOLCONSTANT = 298
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 13 "parser.y" /* yacc.c:1909  */

    int ival;
    float fval;
    char *sval;

#line 104 "parser.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
