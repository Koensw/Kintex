#include "operators.h"
#include "../operands/operands.h"
#include "../../processor.h"

using namespace kintex;

EqualityOperator *EqualityOperator::create(Processor &p){
    //check if match:
    //no: return NULL
    if(p.getChar() != op[0] || p.lookNextChar(1) != op[1]) return nullptr;
	Position pos = p.getPos();
	
    //yes: goto next char and create the operator
    p.nextChar();p.nextChar();
    return new EqualityOperator(p.getPrevExpression(pos), p.getNextExpression(pos));
}

Value EqualityOperator::result(Environment &env){
    if(*children[0]->result(env) == *children[1]->result(env)) return Integer(1, this);
    else return Integer(0, this);
}