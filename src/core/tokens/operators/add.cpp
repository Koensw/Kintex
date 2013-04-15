#include "operators.h"
#include "../operands/operands.h"
#include "../../processor.h"

using namespace kintex;

AddOperator *AddOperator::create(Processor &p){
    //check if match:
    //no: return NULL
    if(p.getChar() != op[0]) return nullptr;
    
    //yes: goto next char and create the operator
    p.nextChar();
    return new AddOperator(p.getPrevExpression(), p.getNextExpression());
}

Value AddOperator::result(){
    //if right hand is float convert lefthand to float too
    Value ret = children[0]->result();
    Value result = children[1]->result();
    if(typeid(*result) == typeid(FloatingPoint) && typeid(*ret) == typeid(Integer)){
        ret = FloatingPoint(dynamic_cast<Integer&>(*ret));
    }
    
    *ret += *result;
    return ret;
}