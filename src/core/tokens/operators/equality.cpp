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

Value EqualityOperator::result(){
    //if right hand is float convert lefthand to float too
    if(*children[0]->result() == *children[1]->result()) return Integer(1, this);
    else return Integer(0, this);
}