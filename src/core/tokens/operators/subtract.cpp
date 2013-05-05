#include "operators.h"
#include "../operands/operands.h"
#include "../../processor.h"

using namespace kintex;

SubtractOperator *SubtractOperator::create(Processor &p){
    //check if match:
    //no: return NULL
    if(p.getChar() != op[0]) return nullptr;
	Position pos = p.getPos();
	
	//check if not actually a negative float/integer
	Expression prev = p.getPrevExpression(pos, true);
	if(typeid(*prev) == typeid(Void)) return nullptr;
	
    //yes: goto next char and create the operator
    p.nextChar();
	
    //build new operator
    return new SubtractOperator(prev, p.getNextExpression(pos));
}

Value SubtractOperator::result(){
    //if right hand is float convert lefthand to float too
    Value ret = children[0]->result();
    Value result = children[1]->result();
	if(typeid(*ret) == typeid(Void)){
		Value val = Value(new Integer(-1));
		*result *= *val;
		return result;
	}if(typeid(*result) == typeid(FloatingPoint) && typeid(*ret) == typeid(Integer)){
        ret = FloatingPoint(dynamic_cast<Integer&>(*ret));
    }

    *ret -= *result;
    return ret;
}
