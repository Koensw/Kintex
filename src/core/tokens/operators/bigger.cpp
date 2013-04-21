#include "operators.h"
#include "../operands/operands.h"
#include "../../processor.h"

using namespace kintex;

BiggerOperator *BiggerOperator::create(Processor &p){
    //check if match:
    //no: return NULL
    if(p.getChar() != op[0]) return nullptr;
	Position pos = p.getPos();
	
    //yes: goto next char and create the operator
    p.nextChar();
	bool eq = false;
	if(p.getChar() == '='){
		p.nextChar();
		eq = true;
	}
    return new BiggerOperator(p.getPrevExpression(pos), p.getNextExpression(pos), eq);
}

Value BiggerOperator::result(){
    //if right hand is float convert lefthand to float too
    Value left = children[0]->result();
	Value right = children[1]->result();
    if(*left < *right || (!equal && *left == *right)) return Integer(0, this);
    else return Integer(1, this);
}

std::ostream &BiggerOperator::display(std::ostream &out) const{
	out << "(" << children[0] << ">";
	if(equal) out << "=";
	out << children[1] << ")";
	return out;
}