#include <iostream>
#include <typeinfo>
#include <cctype>

#include "names.h"
#include "../../processor.h"

using namespace kintex;

Value Variable::result(Environment &env){
	//get actual variable
	DynamicExpression expr = env.getDynSym().getToken(getId());
	
	if(!expr.isBound()){
        throw UndefinedName(*this);
    }
	return expr->val->result(env);
}

Operand &Variable::set(Operand &op, Environment &env){
	//get actual variable
	DynamicExpression expr = env.getDynSym().getToken(getId(), env.getDynSym().currentScope() == DynamicSymbolTable::FUNCTION);
	
	//real variable don't exist, use this one
	if(!expr.isBound()){ 
		expr = DynamicExpression(*this); 
	}
	
    //cast value to type of operand (weak type!) and set it to value
    expr->val = op.clone();
    
    //save value
	env.getDynSym().addToken(expr, env.getDynSym().currentScope() == DynamicSymbolTable::FUNCTION);
	    
    //return value (but not direct, to protect it)
    return op;
}