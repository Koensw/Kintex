#include <sstream>

#include "operators.h"
#include "../operands/operands.h"
#include "../../processor.h"

using namespace kintex;

DivideOperator *DivideOperator::create(Processor &p){
    //check if match:
    //no: return NULL
    if(p.getChar() != op[0]) return nullptr;
	Position pos = p.getPos();
	
    //yes: goto next char
    p.nextChar();
    
    //create the operator
    return new DivideOperator(p.getPrevExpression(pos), p.getNextExpression(pos));
}

Value DivideOperator::result(Environment &env){
	Value leftResult = children[0]->result(env);
	Value rightResult = children[1]->result(env);
	
	//check if not a zero value righthand
    if((*rightResult == Integer(0))) throw CalculationError(*this, "Divide by zero is impossible.");

    //if right hand is float convert lefthand to float too
    if(typeid(*rightResult) == typeid(FloatingPoint) && typeid(*leftResult) == typeid(Integer)){
        leftResult = FloatingPoint(dynamic_cast<Integer&>(*leftResult));
    }

    //if both are integers calculate a check too and return that check if the normal answer is not correct
    if(typeid(*rightResult) == typeid(Integer) && typeid(*leftResult) == typeid(Integer)){
		Value cpy = leftResult->clone();
    	if(cpy->mod(*rightResult) == Integer(0)){
    		*leftResult /= *rightResult;
    		return leftResult;
    	}else{
	        Value checkLeft = FloatingPoint(dynamic_cast<Integer&>(*leftResult));
	        Value checkRight = FloatingPoint(dynamic_cast<Integer&>(*rightResult));
        
    	    *checkLeft /= *checkRight;
        	return checkLeft;
        }
    }else{
        //calculate
        *leftResult /= *rightResult;
        return leftResult;
    }
}
