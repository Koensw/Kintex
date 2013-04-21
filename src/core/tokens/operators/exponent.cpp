#include <iostream>
#include <typeinfo>

#include "operators.h"
#include "../operands/operands.h"
#include "../../processor.h"

using namespace kintex;

ExponentOperator *ExponentOperator::create(Processor &p){
    //check if match:
    //no: return NULL
    if(p.getChar() != op[0]) return nullptr;
	Position pos = p.getPos();
	
    //yes: goto next char and create the operator
    p.nextChar();

    //build new operator
    return new ExponentOperator(p.getPrevExpression(pos), p.getNextExpression(pos));
}

Value ExponentOperator::result(){
    //if right hand is float convert lefthand to float too
    Value ret = children[0]->result();
    Value result = children[1]->result();
    if(typeid(*result) == typeid(FloatingPoint) && typeid(*ret) == typeid(Integer)){
        ret = FloatingPoint(dynamic_cast<Integer&>(*ret));
    }

    //*ret = ret->pow(*result);
    ret->pow(*result);
    return ret;
}
