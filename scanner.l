%{
  #include <iostream>
  #include <cstring>
  #include "parser.tab.h"
  using namespace std;
  extern int yylex();
  int line_num = 1;
%}
%option noyywrap
%%
[ \t]         ; // eats whitespace

'const'     { cout << "Found an const stmt : " << yytext << endl; 
              return CONST;}
"if"        { cout << "Found an if stmt : " << yytext << endl;
                return IF; }

 "else" {


  return ELSE;
 }             
"for"       { cout << "Found a for stmt : " << yytext << endl;
                
                return FOR;
                
                 }
"while"     { cout << "Found a while stmt : " << yytext << endl;
  return WHILE; }
"do"        { cout << "Found a do stmt : " << yytext << endl;
              return DO; }
"break"   { cout << "Found an break stmt : " << yytext << endl;

            return BREAK;
               }
"return"    { cout << "Found an return stmt : " << yytext << endl; 
              return RETURN ;}
import  { cout << "Found an import stmt : " << yytext << endl;  return IMPORT; }

\#.*$        { // COMMENT
  cout << "Found a comment:" << yytext << endl;
  }

[0-9]+\.[0-9]+   { // FLOATING POINT NUMBER
  //cout << "F : Found a floating-point number:" << yytext << endl;
  //yylval.fval = atof(yytext); 
  return FLOATCONSTANT; 
  }

[0-9]+         { // INTEGER VALUE
   cout<< "F : Found an integer:" << yytext << endl; 
  yylval.ival = atoi(yytext); 
  return INTCONSTANT;
  }

\'([a-zA-Z0-9])\'|\'\\n\'|\'\\t\' { // CHARACTER VALUE
  cout << "Found a char: " << yytext << endl;
  return CHARCONSTANT;
  }

\".*\"  { // STRING VALUE
  //cout << "F : Found a string: " << yytext << endl;
  yylval.str = strdup(yytext);
  return STRINGCONSTANT;
  }

\[int\]        { 
  cout << "Found an integer decl:" << yytext << endl;
  yylval.str = strdup(yytext);
  return INTDECL;
}

\[uint\]        { 
  cout << "Found an unsigned integer decl:" << yytext << endl;
  yylval.str = strdup(yytext);
  return UINTDECL;
  }

\[float\]        { 
  cout << "Found an float decl:" << yytext << endl;
  yylval.str = strdup(yytext);
  return FLOATDECL;
  }

\[bool\]        { 
  cout << "Found an bool decl:" << yytext << endl;
  yylval.str = strdup(yytext);
  return BOOLDECL;
  }

\[string\]       {
  cout << "Found an string decl:" << yytext << endl;
  yylval.str = strdup(yytext);
  return STRDECL;
  }



\[char\]        { 
  cout << "Found an char decl:" << yytext << endl;
  yylval.str = strdup(yytext);
  return CHARDECL;
  }

\[void\]        { 
  cout << "Found an void decl:" << yytext << endl;
  yylval.str = strdup(yytext);
  return VOIDDECL;
  }
"<<"                    { return(LEFT_OP);         }
"++"                    {  return(INC_OP);          }

"--"                    {return(DEC_OP);}

"&&"                    {   return(AND_OP);          }
"||"                    {   return(OR_OP);           }
"+="                    {    return(ADD_ASSIGN);      }
"-="                    {     return(SUB_ASSIGN);      }
"*="                    {     return(MUL_ASSIGN);      }
"/="                    {    return(DIV_ASSIGN);      }


"<=" { cout<<"found lessequal"<<endl;return LE_OP;}
">=" {cout<<"found greater equal"<<endl;return GE_OP;}


"=="                    {    return(EQ_OP);           }
"!="                    {    return(NE_OP); }



"+"					  {   cout<<"addition found"<<endl; yylval.str = strdup(yytext);return '+';}
"-"					{yylval.str = strdup(yytext);return '-';}
"*"					{yylval.str = strdup(yytext);return '*';}
"/"					{yylval.str = strdup(yytext);return '/';}
"%"					{yylval.str = strdup(yytext);return '%';}

"<"     {yylval.str = strdup(yytext);return '<';}

">"     {yylval.str = strdup(yytext);return '>';}


"true" {return BOOLCONSTANT;}
"false" {return BOOLCONSTANT;}



[\&\&|\|\||]  { cout << "Found a logical operator: " << yytext << endl; }
[\&\|\^\~] { cout << "found bitwise operator: "  << yytext << endl; }


(_+[a-zA-Z0-9]*)|([a-zA-Z][a-zA-Z0-9]*) { 
  cout << "Found an identifier: " << yytext << endl;
  yylval.str = strdup(yytext);
  return IDNT;
}

\n { ++line_num; }


"("                     {    return('(');             }
")"                     {    return(')');             }

"{"           {  cout<<"opening curly bracket"<<endl;  return('{');             }
"}"                     {  cout<<"closing curly bracket"<<endl;  return('}');             }

":"            { return(':'); }
"!"            {return('!');}

"-"  { return('-');}
"=" { yylval.str = strdup(yytext);return '=';}


[\+\-\*\\\!]    { cout << "Found an operator: " << yytext << endl;
      return yytext[0];
 }
[\{|\}|;|,|? ]    {  
  cout<<"here"<<endl;
  return yytext[0];
   }
%%
// int main(int, char**) {
//   // lex through the input:
//   while (yylex());
// }