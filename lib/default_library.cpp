#include <vector>

/* WARNING: all includes from the library are temporarily done by default_library */
#include "default_library.h"

using namespace kintex;

/*
 * Set the return type of the first found bracket-operator, starting from bottom to top
 * Returns: void
 */
Value ReturnFunction::operator()(std::vector<Expression> values, Environment &env){
    //find correct parent
    Token *par = this;
    BracketsOperator *op = nullptr;
    while(par!=nullptr && (op = dynamic_cast<BracketsOperator*>(par->getParent())) == nullptr) {
        par = par->getParent();
    }
    //check if parent is found
    if(op!=nullptr) op->returnVal = values[0]->result(env);
    else throw CalculationError(*this, "No matching brackets found.");
    return Void(getParent());
}

/* 
 * Deletes the first symbol with the given name
 * Returns: void
 */
Value DeleteFunction::operator()(std::vector<Expression> values, Environment &env){
	Expression expr = values[0];
	
	if(typeid(*expr) == typeid(Variable)){
		Variable &var = dynamic_cast<Variable&>(*values[0]);
		
		//real runtime removal
		env.getDynSym().removeToken(var.getId());
		return Void(getParent());
	}
	
	try{
		InstantiatedFunction &func = dynamic_cast<InstantiatedFunction&>(*values[0]);
		
		//FIXME: find a better way to do this
		for(SymbolTable::iterator iter = env.getSym().begin(); iter != env.getSym().end(); ++iter){
			for(Level::iterator iter2 = iter->begin(); iter2 != iter->end(); ++iter2){
				try{
					Function &func2 = dynamic_cast<Function&>(**iter2);
					if(func.getId() == func2.getId()){
						iter->removeToken(iter2);
						break;
					}
				}catch(std::bad_cast) {}
			}
		}
		
		//interpret time removal
		return Void(getParent());
	}catch(std::bad_cast) {}
	
	throw UnsupportedOperation(*expr, values);
}

/*
 * Check if first parameter is true, then execute second statement
 * Returns: void
 */
Value IfFunction::operator()(std::vector<Expression> values, Environment &env){
    if(!(*values[0]->result(env) == Integer(0))) values[1]->result(env);
    return Void(getParent());
}

/*
 * Check if first parameter is true, and keep executing second argument, until first parameter become false
 * Returns: void
 */
Value WhileFunction::operator()(std::vector<Expression> values, Environment &env){
    while(!(*values[0]->result(env) == Integer(0))) values[1]->result(env);
    return Void(getParent());
}

/*
 *  Prints the result of the argument to std-out
 *  Returns: void
 */
Value PrintFunction::operator()(std::vector<Expression> values, Environment &env){
    std::cout << values[0]->result(env) << std::endl;
    return Void(getParent());
}

/*
 *  Prints the expression of the argument to std-out
 *  Returns: void
 */
Value PrintExpressionFunction::operator()(std::vector<Expression> values, Environment &env){
    values[0]->result(env);
    std::cout << values[0] << std::endl;
    return Void(getParent());
}

/*
 *  Computes the square root of an expression
 *  Returns: square root
 */
Value SquareRootFunction::operator()(std::vector<Expression> values, Environment &env){
	Value result(values[0]->result(env));
	if(typeid(*result) == typeid(Integer)){
		result = FloatingPoint(dynamic_cast<Integer&>(*result));
	}

    Value power(FloatingPoint(0.5));
    result->pow(*power);
    return result;
}
