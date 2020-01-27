#include<bits/stdc++.h>
#include <map>
#include <string>
#include <stack>
#include "llvm/Analysis/Passes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"

#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/Interval.h"
#include <llvm/IR/CFG.h>
#include "llvm/IR/BasicBlock.h"
#include <llvm/IR/Function.h>
#include <utility>
#include <llvm/IR/Value.h>



using namespace llvm;




using namespace std;




class loopInfo {
    BasicBlock *afterBB, *checkBB;
    llvm::Value *condition;
    std::string loopVariable;
    PHINode *phiVariable;
public:
    loopInfo(BasicBlock *afterBlock, BasicBlock *checkBlock, Value *cond, std::string var, PHINode *phiVar) {
        afterBB = afterBlock;
        checkBB = checkBlock;
        condition = cond;
        loopVariable = var;
        phiVariable = phiVar;
    }
    BasicBlock *getAfterBlock() { return afterBB; }
    BasicBlock *getCheckBlock() { return checkBB; }
    llvm::Value *getCondition() { return condition; }
    PHINode *getPHINode() { return phiVariable; }
    std::string getLoopVariable() { return loopVariable; }
};




class Construct {
public:
    LLVMContext Context;
    Module *TheModule;
    IRBuilder<> *Builder;
    std::map<std::string, llvm::AllocaInst *> NamedValues;
    int errors;
    std::stack<loopInfo *> *loops;
    Construct();
    AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, std::string VarName, std::string type);
};










class VarDeclared;

class VariablesDeclared;
class LValue;
class ASTVisit;
class Visit;
class Expr;
class Literal;
class BinaryExpr;
class integerLiteral;
class charLiteral;
class floatLiteral;
class stringLiteral;
class ArrayDeclared;
class Variables;
class VariableOnly;
class Variables;
class Statements;
class Statement;
class GlobalDecl;
class Visitor
{
	public:
		virtual int visit(ASTVisit *vis){};
		virtual int visit(Expr *vis){};
		virtual int visit(BinaryExpr *vis){};
		virtual int visit(Literal *vis){};
		virtual int visit(integerLiteral *vis){};
		virtual int visit(charLiteral *vis){};
		virtual int visit(Statements *vis){};
		virtual int visit(Statement *vis){};

};



class ASTVisit
{
	public:
		ASTVisit();
		virtual int accept(Visitor *v);
	// 	virtual Value *GenCode();
   	// //	void GenCodeDump();
    // 	Construct *Constructs;
};
class GlobalDecls:public ASTVisit
{
	public:
		vector <class GlobalDecl*> decl_list;
		GlobalDecls()=default;
		void Push_back(class GlobalDecl*);
	
		virtual Value* GenCode(Construct *Constructs) ;

	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};

class GlobalDecl:public ASTVisit
{
	public:
		string datatype;
		//vector <string >var_list;
		vector<VariableOnly*> var_list;
		virtual Value *GenCode(Construct *Constructs);
		GlobalDecl(string,Variables*);
	//	virtual Value *GenCode(std::map<std::string, llvm::AllocaInst *> &, Construct *);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};

class Expr:public ASTVisit
{

	public:
		int expr_type;
		int lit_type;
		virtual Value *GenCode(Construct *Constructs){};
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}


};
class TernaryExpr:public Expr
{

	public:
		Expr *expr1;
		Expr *expr2;
		Expr *expr3;
		TernaryExpr(Expr *expr1,Expr *expr2,Expr *expr3);
		virtual Value *GenCode(Construct *Constructs){};
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}

};

class UnaryExpr:public Expr
{
	public:
		Expr* expr1;
		string oper;
		
		UnaryExpr(string ,Expr*);
		virtual Value *GenCode(Construct *Constructs){};
		
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}

};
class BinaryExpr:public Expr
{
	public:
		Expr* lhs;
		string oper;
		Expr* rhs;
		BinaryExpr(Expr*,string ,Expr*);
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};

class Literal:public Expr
{
	virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};



class integerLiteral:public Literal
{
	public:
		int var;
		integerLiteral(int);
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};


class charLiteral:public Literal
{
	public:
		char ch;
		charLiteral(char);
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};
class floatLiteral:public Literal
{public:
		double ch;
		floatLiteral(double);
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}


};
class boolLiteral:public Literal
{

	public:
		string ch;
		boolLiteral(string);
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}



};
class stringLiteral:public Literal
{
	public:
		string ch;
		stringLiteral(string);
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}

};

class VariablesDeclared:public ASTVisit
{	

	public:
		vector <VarDeclared*> variables;
		void pb(VarDeclared*);
		vector<VarDeclared*> getVarList();
		virtual Value *GenCode(std::map<std::string, llvm::AllocaInst *> &,Construct *Constructs);

	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}




};



class VarDeclared:public ASTVisit
{
	public:
	//	Variables *variables;
		vector <VariableOnly*>  var_list;
		string datatype;
		VarDeclared(string,Variables*);
		virtual Value *GenCode(std::map<std::string, llvm::AllocaInst *> &, Construct *);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};



class Variables:public ASTVisit
{	

	public:
		vector <VariableOnly*> variables;
		void pb(VariableOnly*);
		vector <VariableOnly*> getVarList();
		Variables();
		//virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}




};

class VariableOnly:public ASTVisit
{	

	public:
		string id;
		int dim1;
		int dim2;
		int type;
		VariableOnly(string);
		explicit VariableOnly(string,int);
		explicit VariableOnly(string,int,int);
		
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}

};

class Statement:public ASTVisit
{

	public:
		int type;
		virtual Value *GenCode(Construct *Constructs){};
		
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}

};


class Statements:public ASTVisit
{
	public:
		vector <Statement*> statements;
		
		
		Statements()=default;
		void pb(Statement*);
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};



class Block:public Statement
{
	public:
		VariablesDeclared *vardec;
		Statements* statements;
		Block(VariablesDeclared*, Statements*);
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};

class AssignExpr:public Statement
{
	public:
		LValue* var;
		string assign_op;
		Expr* expr;
		AssignExpr(LValue*,string,Expr*);
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};


class ForLoop:public Statement
{
	public:
		
		Expr* expr1;
		Expr* expr2;
		Expr* expr3;
		Block* blck;
		ForLoop(Expr*,Expr*,Expr*,Block*);
		virtual Value *GenCode(Construct *Constructs){};
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};


class WhileLoop:public Statement
{


	public:
		Expr* expr;
		Block* blck;
		WhileLoop(Expr*,Block*);
		virtual Value *GenCode(Construct *Constructs){};
		
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}


};


class IfElse:public Statement
{

	public:
		
		Block* block1;
		Block* block2;
		Expr* expr;

		IfElse(Expr*,Block*,Block*);
		virtual Value *GenCode(Construct *Constructs);

	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}



};






class Return:public Statement
{
	public:
		Expr* expr;
		Return(Expr*);
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};

class Break:public Statement
{
	public:
		Break();
		virtual Value *GenCode(Construct *Constructs);
	
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};

class LValue:public Expr
{
	public:
		string name;
		virtual Value *GenCode(Construct *Constructs);
		LValue(string);
	
		
		
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};
class Continue:public Statement
{
	public:
		Continue();
		virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};


class funcExpr:public ASTVisit
{
	public:
		vector <Expr*	> expr_list;
		funcExpr()=default;
		void Push_back(Expr*);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};


class Type_plus_ids:public ASTVisit
{
	public:
		vector <class Type_plus_id* >type_plus_id_list;
		Type_plus_ids()=default;
		void Push_back(class Type_plus_id*);
		std::vector<class Type_plus_id *> getArgList();
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};

class Type_plus_id:public ASTVisit
{
	public:
		string type_name;
		string name;
		Type_plus_id(string,string);

		string getType()
		{
			return type_name;
		}

		string getName()
		{
			return name;
		}
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};



class funcDecl:public ASTVisit
{
	public:
		string method_type;
		string name;
		class Type_plus_ids *type_plus_ids;
		class Block *blck;
		funcDecl(string,string,Type_plus_ids*,Block*);
		virtual Function *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};

class funcDecls:public ASTVisit
{
	public:
		funcDecls();
		vector <class funcDecl* > method_decl_list;
		
		void Push_back(class funcDecl*);
		virtual Value *GenCode(Construct *Constructs);
		//	funcDecls();
		
		
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};

class funcCall:public ASTVisit
{
	public:
		string name;
		funcExpr* expr_plus;
		funcCall(string, funcExpr*);
		//virtual Value *GenCode(Construct *Constructs);
	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};


class Program:public ASTVisit
{
	public:
		class GlobalDecls *fields;
		class funcDecls *methods;
		Program(class GlobalDecls *, class funcDecls *);
		virtual Value *GenCode();
   		void GenCodeDump();
    	Construct *Constructs;

	virtual int accept(Visitor *v)
	{
		v->visit(this);
	}
};
