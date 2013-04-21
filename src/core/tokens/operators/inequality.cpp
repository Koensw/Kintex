#include "operators.h"
#include "../operands/operands.h"
#include "../../processor.h"

using namespace kintex;

InequalityOperator *InequalityOperator::create(Processor &p){
    //check if match:
    //no: return NULL
    if(p.getChar() != op[0] || p.lookNextChar(1) != op[1]) return nullptr;
	Position pos = p.getPos();
	
    //yes: goto next char and create the operator
    p.nextChar();p.nextChar();
    return new InequalityOperator(p.getPrevExpression(pos), p.getNextExpression(pos));
}

Value InequalityOperator::result(){
    if(*children[0]->result() == *children[1]->result()) return Integer(0, this);
    else return Integer(1, this);
}