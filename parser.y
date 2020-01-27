%{
  #include <iostream>
  #include <cstdio>
  #include "ast.hh"
  using namespace std;
  extern int yylex();
  extern int yyparse();
  extern FILE *yyin;
  extern int line_num;
extern int errors;
int errors=0;

class Program* start=NULL;
  #define YYDEBUG 1
   
  void yyerror(const char *s);
%}



%union {
    int ival;
    double fval;
    char cval;

    char *str;
    class integerLiteral *intLit;
    class BinaryExpr *binexpr;
    class Expr *expr;
    class Literal* lit;
    class VarDeclared* vd;

    class VariableOnly* vo;
    class Variables* va;
    class Block* block;
    class Statement* stmt;
class LValue* lval;
  
    class Statements* stml;
class funcExpr *funcexpr;
class funcCall *funcCall;
class funcDecl *fndef;
class Type_plus_ids* tpis;
class Type_plus_id* tpi;
class funcDecls* fndecls;
class Program* prog;
class VariablesDeclared* vardec;
class GlobalDecls* gds;
class GlobalDecl* gd;
};
%type<fndecls> FNDEFL;
%type<funcexpr> FNCALLARGS;
%type<funcCall> FNCALL;
%type<fndef> FNDEF;

%type<vo> ARRAYDEC2;
%type<tpis> OFNARGS;
%type<tpi> FUNCARG;
%type<lval> LVAL
%type<stml> STMTL
%type<expr> EXPRESSION_STMT
%type<stmt> STMT AssignEXPR
%type<stmt>IFBLOCK LOOP FLOWSTMT  
%type<block> CMPD
%type<vardec> DECL
%type<expr>  POSTFIX EXPR MUL ADD EQUALITY RELEXP logical_and logical_or logical_not ternary_exp bitwise_and bitwise_or bitwise_xor UNARY_EXP
%type <vd> VARDEC
%type<va> IDNTL
%type<vo>VARTYPE
%token <ival> INT
%token <fval> FLOAT
%token <str> STRING
%token <str> INTDECL
%token <str> UINTDECL;
%token <str> FLOATDECL
%token <str> STRDECL
%token <str> CHARDECL
%token <str> BOOLDECL;
%type<str> TYPE;
%token <str> VOIDDECL
%token <str> IDNT;
%token IMPORT;
%token<str>  MUL_ASSIGN DIV_ASSIGN   ADD_ASSIGN SUB_ASSIGN GE_OP LE_OP AND_OP OR_OP LEFT_OP INC_OP EQ_OP NE_OP DEC_OP 
%token<str> CASE IF ELSE SWITCH WHILE FOR CONTINUE BREAK RETURN CONST DO    
%token<ival> INTCONSTANT
%token<str> STRINGCONSTANT
%token<cval> CHARCONSTANT
%token<str> BOOLCONSTANT
%token<fval> FLOATCONSTANT
%token <str> '!' '&' '|' '^'
%token <str> '='
%token <str> '>'
%token <str> '<'
%token <str> '+'
%token <str> '-'
%token <str> '*'
%token <str> '/'
%token <str> '%'
%type <str> assignop
%type <expr>PRIMARY_EXP 
%type<prog> PROG

%type<gds> GLOBALDECLS
%type<gd> GLOBALDECL
%%

//import statements and variable declarations in global scope



PROG:GLOBALDECLS   FNDEFL {  
        $$=new Program($1,$2);
        start=$$;
      
    }| GLOBALDECLS {  
        $$=new Program($1,NULL);
        cout<<"this defintion"<<endl;
      start=$$;
    }  |
    FNDEFL{
         $$=new Program(NULL,$1);
        start=$$;
    cout<<"only fnd"<<endl;
}; 

GLOBALDECLS: GLOBALDECL{
    $$= new GlobalDecls();
	$$->Push_back($1);
} |  GLOBALDECLS GLOBALDECL
{
    $$->Push_back($2);
};

GLOBALDECL: TYPE IDNTL ';'{
     $$=new GlobalDecl(string($1),$2);

};

FNDEFL:FNDEF{

        $$=new funcDecls();
        $$->Push_back($1);

} | FNDEF FNDEFL{
        $2->Push_back($1);
        $$=$2;

};

STMTL: STMT{
        $$=new Statements();
        $$->pb($1);
        
    } | STMT  STMTL    
    {   
        $2->pb($1);
        $$=$2;

    };


STMT:IFBLOCK
    |LOOP
    |FLOWSTMT
    |LVAL assignop ternary_exp ';'{
    $$ = new AssignExpr($1,string($2),$3);
     // cout<<"AssignmentExpression "<< $1->name<<" "<<$1<<" "<<$3<<endl;
  }
    |FNCALL
    |CMPD{cout<<"compund";}
    
    ;



EXPR:AssignEXPR;


EXPRESSION_STMT: AssignEXPR ';';




//If block,loop and flow statemts grammar


IFBLOCK
  : IF '(' EXPR ')' CMPD { 
      $$=new IfElse( $3,$5,NULL);
      cout<<"Bison:If called here"<<endl;
}
  | IF '(' EXPR ')' CMPD ELSE CMPD{

    $$=new IfElse( $3,$5,$7);

  }
  ;


LOOP
  : WHILE '(' EXPR ')' CMPD {cout<<"Bison: While matched"<<endl;}
  {
      $$=new WhileLoop($3,$5);
  }
  | FOR '(' EXPRESSION_STMT EXPRESSION_STMT  EXPR')' CMPD{
       $$=new ForLoop($3,$4,$5,$7);


  }
  ;



FLOWSTMT:CONTINUE ';'{

$$=new Continue();

}
  | BREAK ';'{

      $$=new Break();
  }
  | RETURN ';' {
      $$=new Return(NULL);
  }|RETURN EXPRESSION_STMT{


      $$=new Return($2);
  } ; 



//Operator related
assignop : '='{
    cout<<"Bison:found assign op"<<endl;
}
  | MUL_ASSIGN
  | DIV_ASSIGN
  | ADD_ASSIGN
  | SUB_ASSIGN
  ;

// Related to expressions 



PRIMARY_EXP:
    FLOATCONSTANT
  {
     // cout<<"Bison:found int constant"<<$1<<" "<<endl;
       $$=new floatLiteral($1);
  }
  | INTCONSTANT
  {
    //  cout<<"Bison:found int constant"<<$1<<" "<<endl;
    cout<<"inte"<<$$;
       $$=new integerLiteral($1);
  }
  | STRINGCONSTANT
  {
     // cout<<"Bison:found int constant"<<$1<<" "<<endl;
       $$=new stringLiteral($1);
  }
  | BOOLCONSTANT
   {
     // cout<<"Bison:found int constant"<<$1<<" "<<endl;
       $$=new boolLiteral($1);
  }|
  LVAL{

      //cout<<"this lval being called"<<endl;

  } 
   ;
POSTFIX
  :PRIMARY_EXP
  |POSTFIX INC_OP {  
      cout<<"postfix op found"<<endl;
  }
  |POSTFIX DEC_OP
  ;
UNARY_EXP
  : POSTFIX
  | INC_OP UNARY_EXP
  | DEC_OP UNARY_EXP
  ;






// HighPrecedence operators

MUL: MUL '*' UNARY_EXP{
    cout<<"Multiplication"<<$2<<endl;
 $$ = new BinaryExpr($1,string($2),$3);

}
    | MUL '/'  UNARY_EXP{
         cout<<"Division"<<$1<<" "<<$2<<" "<<$3<<endl;
         $$ = new BinaryExpr($1,string($2),$3);
    }
    | UNARY_EXP;



ADD:ADD '+' MUL {
    cout<<"Matched addition rule"<< $2<<endl;
  $$ = new BinaryExpr($1,string($2),$3);
 
    }
    |MUL


//Relational expression



RELEXP
  : ADD
  | RELEXP '<' ADD{
     // cout<<"relational exp"<<endl;
       $$ = new BinaryExpr($1,string($2),$3);
  }
  | RELEXP '>' ADD{
       $$ = new BinaryExpr($1,string($2),$3);
  }
  | RELEXP LE_OP ADD{
      
       $$ = new BinaryExpr($1,string($2),$3);
  }
  | RELEXP GE_OP ADD{
      
       $$ = new BinaryExpr($1,string($2),$3);
  }



EQUALITY
  : RELEXP
  | EQUALITY EQ_OP RELEXP
  | EQUALITY NE_OP RELEXP
  ;



// Bitwise and logical operators




bitwise_and
  : EQUALITY
  | bitwise_and '&' EQUALITY{
       $$ = new BinaryExpr($1,string($2),$3);
  }
  ;

bitwise_xor
  : bitwise_and
  | bitwise_xor '^' bitwise_and{
       $$ = new BinaryExpr($1,string($2),$3);
  }
  ;

bitwise_or
  : bitwise_xor
  | bitwise_or '|' bitwise_xor{
       $$ = new BinaryExpr($1,string($2),$3);
  }
  ;



logical_not:bitwise_or
            |'!'logical_not ;


logical_and
  : logical_not
  | logical_and AND_OP logical_not{
       $$ = new BinaryExpr($1,string($2),$3);
  }
  ;

logical_or
  : logical_and
  | logical_or OR_OP logical_and{
       $$ = new BinaryExpr($1,string($2),$3);
  }
  ;
ternary_exp
  : logical_or
  | logical_or '?' EXPR ':' ternary_exp{
      cout<<"Ternary found"<<endl;
       $$ = new TernaryExpr($1,$3,$5);
  }


AssignEXPR:ternary_exp
  | LVAL assignop ternary_exp{
    $$ = new AssignExpr($1,string($2),$3);
      cout<<"AssignmentExpression "<< $1->name<<" "<<$1<<" "<<$3<<endl;
  };

LVAL:IDNT{
    cout<<"Single lval declaration found"<<$$<<endl;
    $$=new LValue( string($1) );
//  cout<<"vtype:idnt" << $$<<endl;

} | IDNT '{' IDNT '}' ;


//Function Definition rules
FNCALLARGS:EXPR{
    	$$ = new funcExpr();
		$$->Push_back($1);

} | 
    EXPR ',' FNCALLARGS{

           $3->Push_back($1);
           $$=$3;


    };


FNCALL: IDNT '(' FNCALLARGS ')' {
    cout << "function call found : " << $1 << endl;

    $$=new funcCall($1,$3);
};





FNDEF: TYPE IDNT '(' OFNARGS ')' CMPD ';' {
  //  cout << "fn defn :: " << $2 << endl;

    $$=new funcDecl($1,$2,$4,$6);
}| TYPE IDNT '('  ')' CMPD ';' {
  //  cout << "fn defn :: " << $2 << endl;

    $$=new funcDecl($1,$2,NULL,$5);
};


OFNARGS:FUNCARG{

            $$ = new Type_plus_ids();
			$$->Push_back($1);
       // cout << "fn args empty :: " << $2 << endl;
    }|  FUNCARG ',' OFNARGS{

            //cout << "fn args :: " << $2 << endl;
            $3->Push_back($1);
            $$=$3;
    };

FUNCARG:TYPE IDNT{
    $$=new Type_plus_id(string($1),string($2));
};
			
				
CMPD: '{' '}' {
        $$=new Block(NULL,NULL);
    cout<<"cmpd bracke"<<endl;}
  | '{' STMTL '}'{

        $$=new Block(NULL,$2);
       cout<<"Compound mutiple inside bracket"<<endl;
       }
  | '{' DECL  '}'{

        $$=new Block($2,NULL);
        cout<<"onluy declar"<<endl;

  }
  | '{' DECL  STMTL '}'{


        $$=new Block($2,$3);
        cout<<"onluy declar"<<endl;

  };



DECL: VARDEC  {
    cout<<"Bison: var declaration"<<endl;
    $$=new VariablesDeclared();
    $$->pb($1);
    
    }
    |   DECL VARDEC{
        
        cout<<"Multiple vardecs found"<<endl;
        $1->pb($2);
        $$=$1;
    };
    


VARDEC: TYPE IDNTL ';'  {
    cout<<"Bison:found a variable declaration"<< endl;
    $$=new VarDeclared(string($1),$2);

} ;



IDNTL: VARTYPE{

        $$=new Variables();
        //cout<<"idntl:vartype " << $$<<endl;
        $$->pb($1);
        //cout<<"pushed"<<$1->id<<endl;
        //cout<<"varlist size"<<$$->getVarList().size()<<endl;

    }|
    IDNTL ',' VARTYPE {
         //cout<<"coomma::"<<($1)->id<<endl;
       // cout<<"idntl:comma:vartype' idntl " << $1<<endl;
    // cout<<$$->getVarList()<<endl;
        $1->pb($3);
        $$=$1;

    };



VARTYPE: IDNT{
    cout<<"Single id declaration found"<<endl;
    $$=new VariableOnly(string($1));
    cout<<"vtype:idnt" << $$<<endl;

} | ARRAYDEC2 ;




ARRAYDEC2: IDNT'{' INTCONSTANT '}'{
             $$=new VariableOnly(string($1),$3);
        cout<<"found one array definitonhere"<<endl;
};




TYPE: INTDECL{
    cout << "found int decl bison " << $1 << endl;
    }
    | UINTDECL{
        cout << "found uint decl bison " << $1 << endl;
    }
    | FLOATDECL{
        cout << "found float decl bison " << $1 << endl;
    }
    | STRDECL{
        cout << "found string decl bison " << $1 << endl;
    }
    | CHARDECL{
        cout << "found char decl bison " << $1 << endl;
    }
    | BOOLDECL{
        cout << "found bool decl bison " << $1 << endl;
    }
    | VOIDDECL{
        cout << "found void decl bison " << $1 << endl;
    };







//Unary operators

%%

int main(int, char *argv[]) {
  // lex through the input:
//   while (yylex());
    FILE *f = fopen(argv[1], "r");
    
    if(!f){
        cout << "[!] file not opened." << endl;
        return -1;
    }
    yyin = f;
    yyparse();
    printf("Parsing over");

    if(start)
	{
        	if(start->fields==NULL)
			cout<<"field null"<<endl;		
		start->GenCode();
        if(errors==0)
		{
            
			start->GenCodeDump();
		}
		else
		{
			cerr<<"Error"<<endl;
		}
		
	}
}


void yyerror(const char *s)
{
	fprintf(stderr, "error: %s\n", s);
}