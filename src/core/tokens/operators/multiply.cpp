#include <iostream>
#include <typeinfo>

#include "operators.h"
#include "../operands/operands.h"
#include "../../processor.h"

using namespace kintex;

MultiplyOperator *MultiplyOperator::create(Processor &p){
    //check if match:
    //no: return NULL
    if(p.getChar() != op[0]) return nullptr;
	Position pos = p.getPos();
	
    //yes: goto next char
    p.nextChar();
    
    //create the operator
    return new MultiplyOperator(p.getPrevExpression(pos), p.getNextExpression(pos));
}

Value MultiplyOperator::result(Environment &env){
    //if right hand is float convert lefthand to float too
    Value ret = children[0]->result(env);
    Value result = *children[1]->result(env);
    if(typeid(*result) == typeid(FloatingPoint) && typeid(*ret) == typeid(Integer)){
        ret = FloatingPoint(dynamic_cast<Integer&>(*ret));
    }
    
    *ret *= *result;
    return ret;
}