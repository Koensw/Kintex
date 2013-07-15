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
