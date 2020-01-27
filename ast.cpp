#include "ast.hh"
#include <bits/stdc++.h>
#include <string.h> 
#include <stdio.h> 
#include <stdarg.h>
#include <string.h>
#include <vector>
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


class ASTVisit;
class Visit;
class Expr;
class Literal;
class BinaryExpr;
class integerLiteral;
class charLiteral;



#define debug 1

using namespace std;



Construct::Construct() {
    this->Builder = new IRBuilder<>(Context);
    this->loops = new std::stack<loopInfo*>();
    errors = 0;
    this->TheModule = new Module("Last Phase Compiler", Context);
}

AllocaInst *Construct::CreateEntryBlockAlloca(Function *TheFunction, std::string VarName, std::string type) {
    IRBuilder<> Build(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    AllocaInst *alloca_instruction = nullptr;
    if (type == "boolean")
    {
        alloca_instruction = Build.CreateAlloca(Type::getInt1Ty(this->Context), 0, VarName);
    }
    else if(type == "int")
    {
        alloca_instruction = Build.CreateAlloca(Type::getInt32Ty(this->Context), 0, VarName);
    }
    return alloca_instruction;
}


map<string,int> var_map;

Value *Program::GenCode()
{
	Value *v;
//	cout<<"gencode of Program"<<endl;
	
	if(fields!=NULL)
	{
		//	cout<<"fieldnot null";

		v=fields->GenCode(this->Constructs);
		if(v==nullptr)
		{
			cerr<<"Invalid field Declarations"<<endl;
            Constructs->errors++;
    	    return nullptr;
		}
	}
	if(methods!=NULL)
	{
	//	cout<<"here fields not null"<<endl;
		v=methods->GenCode(this->Constructs);
		
		if(v==nullptr)
		{
			cerr<<"Invalid method Declarations"<<endl;
            Constructs->errors++;
	        return nullptr;
		}
	}
	cout<<"here";
	return v;
}

// Value* Fors::GenCode(Construct *Constructs)
// {
// 	Value *start = expr1->GenCode(Constructs);
//     if (start == nullptr)
//     {
//         return nullptr;
//     }
//     if (expr1->expr_type==0) {
//         start = Constructs->Builder->CreateLoad(start);
//     }
   

//     Function *TheFunction = Constructs->Builder->GetInsertBlock()->getParent();
    
//     llvm::AllocaInst *Alloca = Constructs->CreateEntryBlockAlloca(TheFunction, name, string("int"));
//     Constructs->Builder->CreateStore(start, Alloca);

//     Value *step_val = ConstantInt::get(Constructs->Context, APInt(32, 1));
//     BasicBlock *pre_header = Constructs->Builder->GetInsertBlock();
//     BasicBlock *loop_body = BasicBlock::Create(Constructs->Context, "loop_body", TheFunction);
//     BasicBlock *afterBB = BasicBlock::Create(Constructs->Context, "afterBB", TheFunction);
//     Constructs->Builder->CreateBr(loop_body);
//     Constructs->Builder->SetInsertPoint(loop_body);

//     PHINode *Variable = Constructs->Builder->CreatePHI(Type::getInt32Ty(Constructs->Context), 2, name);
//     Variable->addIncoming(start, pre_header);
    

//     Value *cond = expr2->GenCode(Constructs);
//     if (cond == nullptr)
//     {
//         Constructs->errors++;
//         cerr<<"Invalid Condition"<<endl;
//         return nullptr;
//     }

//     // Check if condition is a location
//     if (expr2->expr_type==0) 
//     {
//         cond = Constructs->Builder->CreateLoad(cond);
//     }

//     if(!start->getType()->isIntegerTy(32) || !cond->getType()->isIntegerTy(32))
//     {
//         cerr<<"should be int"<<endl;
//         Constructs->errors++;
//         return nullptr;
//     }

//     Constructs->loops->push(new loopInfo(afterBB, loop_body, cond, name, Variable));
//     llvm::AllocaInst *OldVal = Constructs->NamedValues[name];
//     Constructs->NamedValues[name] = Alloca;
    

//     if (blck->GenCode(Constructs) == nullptr) {
//         return nullptr;
//     }

//     Value *cur = Constructs->Builder->CreateLoad(Alloca, name);
//     Value *next_val = Constructs->Builder->CreateAdd(cur, step_val, "Next_Val");
//     Constructs->Builder->CreateStore(next_val, Alloca);
//     cond = Constructs->Builder->CreateICmpSLT(next_val, cond, "loop_condition");
//     BasicBlock *loopEndBlock = Constructs->Builder->GetInsertBlock();
//     Constructs->Builder->CreateCondBr(cond, loop_body, afterBB);
//     Constructs->Builder->SetInsertPoint(afterBB);
//     Variable->addIncoming(next_val, loopEndBlock);

//     if (OldVal)
//     {
//         Constructs->NamedValues[name] = OldVal;
//     } 
//     else
//     {
//         Constructs->NamedValues.erase(name);
//     }
//     llvm::Value *V = ConstantInt::get(Constructs->Context, APInt(32, 1));
//     return V;
// }


// // 

Value *GlobalDecls::GenCode(Construct *Constructs)
{
	for(auto &i:decl_list)
	{
		i->GenCode(Constructs);
	}
	Value *v=ConstantInt::get(Constructs->Context, APInt(32, 1));
	return v;
}
void GlobalDecls::Push_back(GlobalDecl* gd)
{

	(this->decl_list).push_back(gd);
}

GlobalDecl::GlobalDecl(string type,Variables * var)
{

	this->datatype=type;
	this->var_list=var->variables;

	for(auto var:var_list)
    {
        
			cout<<"creating int variable";
            //type_func temp={var->datatype,"GLOBAL"};
		 	string var_name=var->id;		

            var_map[var_name]=0;
       
       
    }


};


Value *GlobalDecl::GenCode(Construct *Constructs)
{
	llvm::Type *ty = nullptr;
	if(datatype=="[int]")
	{
		ty = Type::getInt32Ty(Constructs->Context);
	}
	if(datatype=="[bool]")
	{
		ty = Type::getInt1Ty(Constructs->Context);
	}
	if(datatype=="[char]")
	{
		ty = Type::getInt8Ty(Constructs->Context);
	}

	for(auto var: var_list)
	{
        Value *cur = Constructs->TheModule->getGlobalVariable(var->id);
        if(cur!=nullptr)
        {
            Constructs->errors++;
            cout<<"var name "<< var->id <<" already declared"<<endl;
            continue;
        }
		
            GlobalVariable *gv = new GlobalVariable(*(Constructs->TheModule), ty, false,
                                                    GlobalValue::ExternalLinkage, nullptr,
                                                    var->id);
            gv->setInitializer(Constant::getNullValue(ty));
        
	}
	Value *v = ConstantInt::get( Constructs->Context, APInt(32, 1) );
    return v;

};


raw_ostream &file_write() 
{
    FILE *fp=fopen("IRCODE","w");
    static raw_fd_ostream S(fileno(fp), true);
    return S;
}
void Program::GenCodeDump()
{
    if(Constructs->errors==0)     
	   cerr << "Generating LLVM IR Code\n";
    
    this->Constructs->TheModule->print(file_write(), nullptr);
}








ASTVisit::ASTVisit(){}

int ASTVisit::accept(Visitor *vis)
{
	vis->visit(this);
}
funcCall::funcCall(string name,funcExpr* expr_plus)
{

		this->name=name;
		this->expr_plus=expr_plus;

}
Program::Program(GlobalDecls *vardec,funcDecls*  func)
{
	cout<<"Programm cionstrctr";
	if(vardec==NULL)
		cout<<"varnull"<<endl;
	if(func==NULL)
		cout<<"func is null"<<endl;

	this->fields=vardec;
	this->methods=func;
	this->Constructs = new Construct();

}
Type_plus_id::Type_plus_id(string type_name,string name)
{
		this->type_name=type_name;
		this->name=name;
}

funcDecl::funcDecl(string method,string name,Type_plus_ids *type_plus_ids, Block *blck)
{


		this->method_type=method;
		this->name=name;
		this->type_plus_ids=type_plus_ids;
		this->blck=blck;
		//funcDecl(string,string,Type_plus_ids*,Block*);
}


Function* funcDecl::GenCode(Construct *Constructs)
{
	cout<<"gencode funcdecl"<<endl;
	vector<std::string> arg_names;
    vector<std::string> arg_types;
    vector<class Type_plus_id *> args;
    if(type_plus_ids!=NULL)
    {
    	args = type_plus_ids->type_plus_id_list;
    }

    std::vector<Type *> arguments;
    auto arg_size = args.size();
	cout<<"here "<<arg_size<<endl;
    for (auto &arg : args)
    {
        string argument_type = arg->type_name;
        string argument_name = arg->name;
        if(argument_type == "[int]") 
        {
            arguments.push_back(Type::getInt32Ty(Constructs->Context));
        } 
        else if(argument_type == "[bool]")
        {
            arguments.push_back(Type::getInt1Ty(Constructs->Context));
        } 
		else if(argument_type == "[char]")
        {
            arguments.push_back(Type::getInt8Ty(Constructs->Context));
        } 
        else
        {
            Constructs->errors++;
            cerr<<"Arguments can only be int or boolean or char"<<endl;
            return nullptr;
        }
        arg_types.emplace_back(argument_type);
        arg_names.emplace_back(argument_name);
    }

    Type *returnType;
    if (method_type == "[int]")
    {
        returnType = Type::getInt32Ty(Constructs->Context);
    } 
    else if (method_type == "[bool]")
    {
        returnType = Type::getInt1Ty(Constructs->Context);
    } 
	else if (method_type == "[char]")
    {
        returnType = Type::getInt8Ty(Constructs->Context);
    } 
    else if (method_type == "[void]") 
    {
        returnType = Type::getVoidTy(Constructs->Context);
    } 
    else 
    {
        Constructs->errors++;
        cerr<<"Invalid Return Type for " + name + ". Return Type can only be int or boolean or bool"<<endl;
        return nullptr;
    }

    FunctionType *FT = llvm::FunctionType::get(returnType, arguments, false);
    Function *F = llvm::Function::Create(FT, Function::ExternalLinkage, name, Constructs->TheModule);

    unsigned Idx = 0;
    for (Function::arg_iterator I = F->arg_begin(); Idx != arg_size; ++I, ++Idx)
    {
        I->setName(arg_names[Idx]);
    }


    BasicBlock *BB = BasicBlock::Create(Constructs->Context, "entry", F);
    Constructs->Builder->SetInsertPoint(BB);
    

    unsigned Idx1 = 0;
    for (auto &Arg : F->args()) {
        AllocaInst *Alloca = Constructs->CreateEntryBlockAlloca(F, arg_names[Idx1], arg_types[Idx1]);
        Constructs->Builder->CreateStore(&Arg, Alloca);
        Constructs->NamedValues[arg_names[Idx1]] = Alloca;
        Idx1++;
    }

    Value *RetVal = blck->GenCode(Constructs);
    if(RetVal==nullptr)
    {
        if(method_type=="[void]")
            Constructs->Builder->CreateRetVoid();
        verifyFunction(*F);
        return F;
    }
    if(method_type=="[int]" && !RetVal->getType()->isIntegerTy(32))
    {
        cerr<<"Ret Type and Method type mismatch"<<endl;
        Constructs->errors++;
    }
    if(method_type=="[bool]" && !RetVal->getType()->isIntegerTy(1))
    {
        cerr<<"Ret Type and Method type mismatch"<<endl;
        Constructs->errors++;
    }
	if(method_type=="[char]" && !RetVal->getType()->isIntegerTy(8))
    {
        cerr<<"Ret Type and Method type mismatch"<<endl;
        Constructs->errors++;
    }
    
    if (RetVal) {
        if(method_type=="void")
            Constructs->Builder->CreateRetVoid();
        verifyFunction(*F);
        return F;
    }

    F->eraseFromParent();
    return nullptr;


};



Value *funcDecls::GenCode(Construct *Constructs)
{
	cout<<"Function declaration for fundecls"<<endl;
	Value *V = ConstantInt::get(Constructs->Context, APInt(32, 0));
	for(auto &i:method_decl_list)
	{
		cout<<"nside loop"<<endl;
		V=i->GenCode(Constructs);
		if(V==nullptr)
			return V;
	}
	return V;
};



Value* Block::GenCode(Construct *Constructs)
{
	Value *V;
    map<std::string, llvm::AllocaInst *> in_func_vals;
    if(vardec!=NULL)
    {
	    V = vardec->GenCode(in_func_vals, Constructs);
	    if (V == nullptr) {
	        return V;
	    }
	}
	if(statements!=NULL)
	{
	    V=statements->GenCode(Constructs);
	    for (auto it = in_func_vals.begin(); it != in_func_vals.end(); it++) {
	        Constructs->NamedValues[it->first] = in_func_vals[it->first];
	    }
	}
    return V;


}




TernaryExpr::TernaryExpr(Expr* expr1, Expr* expr2, Expr* expr3)
{


	this->expr1=expr1;
	this->expr2=expr2;
	this->expr3=expr3;
	this->expr_type=4;
	this->lit_type=3;


}



UnaryExpr::UnaryExpr(string oper,Expr *expr)
{
	cout<<"Called constructor for unary expr"<<this->oper<<"\n";

	this->oper=oper;
	this->expr1=expr;
	this->lit_type=1;
	this->expr_type=3;
}
BinaryExpr::BinaryExpr(Expr* lhs, string oper, Expr* rhs)
{
	this->lhs=lhs;
	this->oper=oper;
	this->rhs=rhs;
	cout<<"Called constructrer for binary expr"<<this->oper<<"\n";

	if(oper=="+" || oper=="-" || oper=="*" || oper=="/" || oper=="%")
	{
		
		this->expr_type=0;
	}
	else if(oper=="<" || oper==">"||oper==">="||oper=="<=")
	{
		this->expr_type=1;
	}
	else if(oper=="==" || oper=="!=")
	{
		this->expr_type=2;
	}

};




Value *BinaryExpr::GenCode(Construct *Constructs)
{
	Value *left = lhs->GenCode(Constructs);
    Value *right = rhs->GenCode(Constructs);
    if (lhs->expr_type == 11) {
        left = Constructs->Builder->CreateLoad(left);
    }
    if (rhs->expr_type == 11) {
        right = Constructs->Builder->CreateLoad(right);
    }
    string opr=oper;
    if (left == 0) {
        Constructs->errors++;
        cerr<<"Error in left operand of " + opr<<endl;
        return nullptr;
    } else if (right == 0) {
        Constructs->errors++;
        cerr<<"Error in right operand of " + opr<<endl;
        return nullptr;
    }
    Value *v = nullptr;
    if (opr == "+") 
    {
        if(!left->getType()->isIntegerTy(32)||!right->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateAdd(left, right, "addtmp");
    } 
    else if (opr == "-")
    {
        if(!left->getType()->isIntegerTy(32)||!right->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateSub(left, right, "subtmp");
    } 
    else if (opr == "*") 
    {
        if(!left->getType()->isIntegerTy(32)||!right->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateMul(left, right, "multmp");
    } 
    else if (opr == "/")
    {
        if(!left->getType()->isIntegerTy(32)||!right->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateSDiv(left, right, "divtmp");
    } 
    else if (opr == "%")
    {
        if(!left->getType()->isIntegerTy(32)||!right->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateSRem(left, right, "modtmp");
    } 
    else if (opr == "<")
    {
        if(!left->getType()->isIntegerTy(32)||!right->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateICmpSLT(left, right, "ltcomparetmp");
    } 
    else if (opr == ">") 
    {
        if(!left->getType()->isIntegerTy(32)||!right->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateICmpSGT(left, right, "gtcomparetmp");
    }
    else if (opr == "<=")
    {
        if(!left->getType()->isIntegerTy(32)||!right->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateICmpSLE(left, right, "lecomparetmp");
    } 
    else if (opr == ">=")
    {
        if(!left->getType()->isIntegerTy(32)||!right->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateICmpSGE(left, right, "gecomparetmp");
    }
    else if (opr == "==") 
    {
        if(left->getType()!=right->getType())
        {
            cerr<<"Both should be of same type"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateICmpEQ(left, right, "equalcomparetmp");
    } 
    else if (opr == "!=") 
    {
        if(left->getType()!=right->getType())
        {
            cerr<<"Both should be of same type"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateICmpNE(left, right, "notequalcomparetmp");
    } 
    else if (opr=="&&")
    {
        if(!left->getType()->isIntegerTy(1)||!right->getType()->isIntegerTy(1))
        {
            cerr<<"Both should be bool"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateAnd(left, right, "andtmp");
    }
    else if (opr=="||")
    {
        if(!left->getType()->isIntegerTy(1)||!right->getType()->isIntegerTy(1))
        {
            cerr<<"Both should be bool"<<endl;
            return nullptr;
        }
        v = Constructs->Builder->CreateOr(left, right, "ortmp");
    }
    return v;
}


Value* charLiteral::GenCode(Construct *Constructs)
{
	Value *v = ConstantInt::get(Constructs->Context, llvm::APInt(8, static_cast<uint64_t>(ch)));
    return v;
}

Value* integerLiteral::GenCode(Construct *Constructs)
{
	Value *v = ConstantInt::get(Constructs->Context, llvm::APInt(32, static_cast<uint64_t>(var)));
    return v;
}


Value* boolLiteral::GenCode(Construct *Constructs)
{
	bool val;
    if (ch == "true") val = true;
    else if (ch == "false") val = false;
    else {
        Constructs->errors++;
        cerr<<"Invalid Boolean Literal " + ch<<endl;
        return nullptr;
    }
    Value *v = ConstantInt::get(Constructs->Context, llvm::APInt(1, val));
    return v;
}

integerLiteral::integerLiteral(int var)
{
	this->expr_type=4;
	this->lit_type=0;
	this->var=var;
	cout<<"calling constructor"<<var<<endl;	
	
		
};

charLiteral::charLiteral(char ch)
{
	
	this->expr_type=4;
	this->lit_type=1;
	this->ch=ch;
		
};

stringLiteral::stringLiteral(string ch)
{
	this->expr_type=4;
	this->lit_type=3;
	this->ch=ch;

};

floatLiteral::floatLiteral(double ch)
{
	this->expr_type=4;
	this->lit_type=4;
	this->ch=ch;

};



boolLiteral::boolLiteral(string ch)
{
	this->expr_type=4;
	this->lit_type=5;
	this->ch=ch;

};

vector<VariableOnly*> Variables::getVarList()
{

	return this->variables;
}
void Variables::pb(VariableOnly* var)
{
	cout<<"Pushback function::"<<var->id<<endl;

	(this->variables).push_back(var);
	cout<<"after pushing size is"<<(this->variables).size();

};

void VariablesDeclared::pb(VarDeclared *var)
{	
	(this->variables).push_back(var);

};


VariableOnly::VariableOnly(string id)
{
	
	this->id=id;
	this->type=0;
	cout<<"constructor initiliazed::"<<id<<endl;
};

Value* VariablesDeclared::GenCode(map<string, AllocaInst *> &in_func_values, Construct *Constructs)
{
	Value *v = ConstantInt::get(Constructs->Context, APInt(32, 1));
    for (auto &decl : variables)
    {
        v = decl->GenCode(in_func_values, Constructs);
        if (v == nullptr)
        {
            return nullptr;
        }
    }
    return v;
};


Value* VarDeclared::GenCode(map<string, llvm::AllocaInst *> &Old_vals, Construct *Constructs)
{
	llvm::Function *TheFunction = Constructs->Builder->GetInsertBlock()->getParent();
    for (const auto &var1 : var_list)
    {
		string var=var1->id;
		string type_name=datatype;
        Value *cur = Constructs->TheModule->getGlobalVariable(var);
        if(cur!=nullptr)
        {
            Constructs->errors++;
            cout<<"var name "<< var <<" already declared"<<endl;
            continue;
        }
        cur = Constructs->NamedValues[var];
        if(cur!=nullptr)
        {
            Constructs->errors++;
            cout<<"var name "<< var <<" already declared"<<endl;
            continue;
        }
        llvm::Value *initval = nullptr;
        llvm::AllocaInst *Alloca = nullptr;
        if (type_name == "[int]")
        {
            initval = ConstantInt::get(Constructs->Context, APInt(32, 0));
            Alloca = Constructs->CreateEntryBlockAlloca(TheFunction, var, "int");
        }
        else if (type_name == "[bool]")
        {
            initval = ConstantInt::get(Constructs->Context, APInt(1, 0));
            Alloca = Constructs->CreateEntryBlockAlloca(TheFunction, var, "boolean");
        }

        Constructs->Builder->CreateStore(initval, Alloca);
        Old_vals[var] = Constructs->NamedValues[var];
        Constructs->NamedValues[var] = Alloca;
    }
    Value *v = ConstantInt::get(Constructs->Context, APInt(32, 1));
    return v;
}





VarDeclared::VarDeclared(string type,Variables* var)
{

	this->datatype=type;
	this->var_list=var->variables;

};


VariableOnly::VariableOnly(string id,int dim1)
{
	this->id=id;
	this->dim1=dim1;
	this->type=1;
	cout<<"constructor initiliazed"<<id<<endl;
};

VariableOnly::VariableOnly(string id,int dim1,int dim2)
{
	this->id=id;
	this->dim1=dim1;
	this->dim2=dim2;
	this->type=2;
	cout<<"constructor initiliazed"<<id<<endl;
};




Value* Continue::GenCode(Construct* Constructs)
{

}

Value* Literal::GenCode(Construct* Constructs)
{
	return nullptr;
}

Value* Return::GenCode(Construct* Constructs)
{

	llvm::Value *V = nullptr;
    if (expr != nullptr)
    {
        V = expr->GenCode(Constructs);
        if (expr->expr_type==11)
        {
            V = Constructs->Builder->CreateLoad(V);
        }
        Constructs->Builder->CreateRet(V);
        return V;
    }
    Constructs->Builder->CreateRetVoid();
    return V;

}



Value *Statements::GenCode(Construct* Constructs)
{
	Value *v = ConstantInt::get(Constructs->Context, llvm::APInt(32, 1));
	for (auto &stmt : statements)
    {
        v = stmt->GenCode(Constructs);
    }
    return v;
}

Value* stringLiteral::GenCode(Construct *Constructs)
{


}

Value* floatLiteral::GenCode(Construct *Constructs)
{
	// Value *v = ConstantFP::get(Constructs->Context, llvm::APInt(32, static_cast<uint64_t>(ch)));
    // return v;
	


}

Value *LValue::GenCode(Construct *Constructs)
{
	Value *V = Constructs->NamedValues[name];

	
	if (V == nullptr) 
	{
        V = Constructs->TheModule->getNamedGlobal(name);
    }
    if (V == nullptr) 
    {
        Constructs->errors++;
        cerr<<"Unknown Variable name " + name<<endl;
        return nullptr;
    }

   
    return V;

}


Value* AssignExpr::GenCode(Construct *Constructs)
{
	cout<<"here lhs variable is"<<var->name;
	Value *cur = Constructs->NamedValues[var->name];

    if (cur == nullptr)
    {
        cur = Constructs->TheModule->getGlobalVariable(var->name);
    }
    if (cur == nullptr)
    {
        Constructs->errors++;
        cerr<<"Unknown Variable Name: " +var->name<<endl;
        Constructs->errors++;
        return nullptr;
    }

    Value *val = expr->GenCode(Constructs);
    if (expr->expr_type==11	)
    {
        val = Constructs->Builder->CreateLoad(val);
    }

    Value *lhs = var->GenCode(Constructs);
    cur = Constructs->Builder->CreateLoad(lhs);

    if(cur->getType()!=val->getType())
    {
            cerr<<"Both should be of sametype"<<endl;
            return nullptr;
    }

    if (val == nullptr)
    {
        Constructs->errors++;
        cerr<<"Error in right hand side of the Assignment"<<endl;
        return nullptr;
    }
    if (assign_op == "+=")
    {
        if(!lhs->getType()->isIntegerTy(32)||!val->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        val = Constructs->Builder->CreateAdd(cur, val, "AddEqualToVal");
    } 
    else if (assign_op== "-=") 
    {
        if(!lhs->getType()->isIntegerTy(32)||!val->getType()->isIntegerTy(32))
        {
            cerr<<"Both should be int"<<endl;
            return nullptr;
        }
        val = Constructs->Builder->CreateSub(cur, val, "SubEqualToVal");
    }


    return Constructs->Builder->CreateStore(val, lhs);


}


Value *Break::GenCode(Construct *Constructs) 
{
	llvm::Value *V = llvm::ConstantInt::get(Constructs->Context, llvm::APInt(32, 1));
    if(Constructs->loops->empty())
    {
        cout<<"Break should be inside for"<<endl;
        Constructs->errors++;
        return nullptr;
    }
    loopInfo *currentLoop = Constructs->loops->top();
    Constructs->Builder->CreateBr(currentLoop->getAfterBlock());
    return V;
}


Value* IfElse::GenCode(Construct *Constructs)
{

	Value *cond = expr->GenCode(Constructs);
    if (cond == nullptr)
    {
        Constructs->errors++;
        cerr<<"Invalid Expression in the IF"<<endl;
        return nullptr;
    }

    if(!cond->getType()->isIntegerTy(1))
    {
        cerr<<"if statement must have type boolean."<<endl;
        return nullptr;
    }

    Function *TheFunction = Constructs->Builder->GetInsertBlock()->getParent();
    BasicBlock *ifBlock = BasicBlock::Create(Constructs->Context, "if", TheFunction);
    BasicBlock *elseBlock = BasicBlock::Create(Constructs->Context, "else");
    BasicBlock *nextBlock = BasicBlock::Create(Constructs->Context, "ifcont");
    
    // bool ret_if = blck1->has_return(), ret_else = false;
    // bool break_if=blck1->has_break(), break_else = false;
    // bool continue_if=blck1->has_continue(), continue_else = false;
    
    BasicBlock *otherBlock = elseBlock;

    if (block2 == nullptr)
    {
        otherBlock = nextBlock;
    }

    Constructs->Builder->CreateCondBr(cond, ifBlock, otherBlock);
    Constructs->Builder->SetInsertPoint(ifBlock);


    Value *if_val = block1->GenCode(Constructs);
    if (if_val == nullptr)
    {
        return nullptr;
    }

    // if (!ret_if && !break_if && !continue_if)
    // {
    //     Constructs->Builder->CreateBr(nextBlock);
    // }

    ifBlock = Constructs->Builder->GetInsertBlock();


    Value *else_val = nullptr;

    if (block2 != nullptr)
    {

        TheFunction->getBasicBlockList().push_back(elseBlock);
        Constructs->Builder->SetInsertPoint(elseBlock);
        else_val = block2->GenCode(Constructs);
        if (else_val == nullptr)
        {
            return nullptr;
        }
        // ret_else = blck2->has_return();
        // continue_else = blck2->has_continue();
        // break_else = blck2->has_break();
        // if (!ret_else && !continue_else && !break_else)
        // {
        //     Constructs->Builder->CreateBr(nextBlock);
        // }
    }
    
    TheFunction->getBasicBlockList().push_back(nextBlock);
    Constructs->Builder->SetInsertPoint(nextBlock);
    // if (ret_else && ret_if) {
        
    //     Type *retType = Constructs->Builder->GetInsertBlock()->getParent()->getReturnType();
    //     if (retType == Type::getVoidTy(Constructs->Context))
    //         Constructs->Builder->CreateRetVoid();
    //     else {
    //         Constructs->Builder->CreateRet(ConstantInt::get(Constructs->Context, APInt(32, 0)));
    //     }
    // }
    Value *V = ConstantInt::get(Constructs->Context, APInt(32, 0));
    return V;
}












Return::Return(Expr* expr)
{
	this->type=1;
	this->expr=expr;

};

IfElse::IfElse(Expr*expr,Block*block1,Block* block2)
{

		this->block1=block1;
		this->block2=block2;
		this->expr=expr;

};


WhileLoop::WhileLoop(Expr* expr,Block* blck)
{

	this->expr=expr;
	this->blck=blck;


};

LValue::LValue(string str)
{
//cout<<"LValue Called here"<<name<<endl;
	this->name=str;
	this->expr_type=11;

}
ForLoop::ForLoop(Expr* expr1,Expr*expr2,Expr* expr3,Block* blck)
{
	
		
		this->expr1=expr1;
		this->expr2=expr2;
		this->expr3=expr3;
		this->blck=blck;
	
};



AssignExpr::AssignExpr(LValue* var,string assign_op,Expr* expr)
{

		cout<<"here in constructor"<<assign_op<<endl;
		this->var=var;
		this->assign_op=assign_op;
		this->expr=expr;
		
};
Block::Block(VariablesDeclared* vardec, Statements* stmt)
{
	this->vardec=vardec;
	this->statements=stmt;

};
void Statements::pb(Statement* st)
{
	(this->statements).push_back(st);
};



void funcExpr::Push_back(Expr* expr)
{

	cout<<"caliing push_bacl";
	this->expr_list.push_back(expr);
};

funcDecls::funcDecls()
{
	
};


void funcDecls::Push_back(funcDecl* method_decl)
{
	method_decl_list.push_back(method_decl);
};


std::vector<class Type_plus_id *> Type_plus_ids::getArgList() {
    return type_plus_id_list;
}


void Type_plus_ids::Push_back(Type_plus_id* type_plus_id)
{
	type_plus_id_list.push_back(type_plus_id);
}


Continue::Continue()
{

};
Break::Break()
{

};
Variables::Variables()
{

};