#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <exception>
#include <cctype>
#include <cmath>

#include "operands.h"
#include "../../processor.h"

using namespace kintex;

/* Make a new integer from a string */
Integer *Integer::create(Processor &p){
    int val;
    bool negative = false;
    //check if integer can be matched
    //no: return NULL;
    if(!isdigit(p.getChar())  && !(!p.isExpressionLeft() && p.getChar() == '-' && isdigit(p.lookNextChar()))) return nullptr;
    //yes create integer
    else{
        if(p.getChar() == '-'){
            p.nextChar();
            negative = true;
        }
        val = p.getChar() - '0';
    }

    //try to increase integer until limit reached
    while(isdigit(p.nextChar())){
        val *= 10;
        val += p.getChar() - '0';
    }
        
    //if value is negative, negate it now
    if(negative) val *= -1;

    return new Integer(val);
}

/* Add operators */
Operand &Integer::operator+=(Integer &op){
    op.val += val;
    return op;
}
Operand &Integer::operator+=(FloatingPoint &op){
    op.val += val;
    return op;
}
/* Minus operators */
Operand &Integer::operator-=(Integer &op){
    op.val -= val;
    return op;
}
Operand &Integer::operator-=(FloatingPoint &op){
    op.val -= val;
    return op;
}
/* Multiply operators */
Operand &Integer::operator*=(Integer &op){
    op.val *= val;
    return op;
}
Operand &Integer::operator*=(FloatingPoint &op){
    op.val *= val;
    return op;
}
/* Divide operators */
Operand &Integer::operator/=(Integer &op){
    op.val /= val;
    return op;
}
Operand &Integer::operator/=(FloatingPoint &op){
    op.val /= val;
    return op;
}
/* Power operators */
Operand &Integer::pow(Integer &op){
    op.val = std::pow(op.val, val);
    return op;
}
Operand &Integer::pow(FloatingPoint &op){
    op.val = std::pow(op.val, val);
    return op;
}

/* Special set operator */
Operand &Integer::operator=(FloatingPoint &op){
    op.val = val;
    return op;
}
Operand &Integer::operator=(Integer &op){
    op.val = val;
    return op;
}

/* Equality operators */
bool Integer::operator==(const Integer &op) const{
    if(op.val == val) return true;
    return false;
}
bool Integer::operator==(const FloatingPoint &op) const{
    if(op.val == val) return true;
    return false;
}
bool Integer::operator==(const Void &op) const{
    return false;
}

/* Lower then operators */
bool Integer::operator<(const Integer &op) const{
    if(op.val > val) return true;
    return false;
}
bool Integer::operator<(const FloatingPoint &op) const{
    if(op.val > val) return true;
    return false;
}
bool Integer::operator<(const Void &op) const{
    return false;
}
