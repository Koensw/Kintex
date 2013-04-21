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

Value DivideOperator::result(){
    //check if not a zero value righthand
    if((*children[1]->result() == Integer(0))) throw CalculationError(*this, "Divide by zero is impossible.");

    //if right hand is float convert lefthand to float too
    Value leftResult = children[0]->result();
    Value rightResult = children[1]->result();
    if(typeid(*rightResult) == typeid(FloatingPoint) && typeid(*leftResult) == typeid(Integer)){
        leftResult = FloatingPoint(dynamic_cast<Integer&>(*leftResult));
    }

    //if both are integers calculate a check too and return that check if the normal answer is not correct
    if(typeid(*rightResult) == typeid(Integer) && typeid(*leftResult) == typeid(Integer)){
        Value checkLeft = FloatingPoint(dynamic_cast<Integer&>(*leftResult));
        Value checkRight = FloatingPoint(dynamic_cast<Integer&>(*rightResult));
        
        *checkLeft /= *checkRight;
        *leftResult /= *rightResult;
        //if same return integer, else floatingpoint
        if(*checkLeft == *leftResult){ return leftResult; }
        else return checkLeft;
    }else{
        //calculate
        *leftResult /= *rightResult;
        return leftResult;
    }
}
