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

/* Make a new floatingpoint from a string */
FloatingPoint *FloatingPoint::create(Processor &p){
    double val;
    bool negative = false;
    //check if floatingpoint can be matched
    //no: return NULL;
    if(isdigit(p.getChar()) || (p.isExpressionLeft() && p.getChar() == '-' && isdigit(p.lookNextChar()))){
        //it is a integer <-- check if it is a floating point to
        int cnt = 1;
        while(isdigit(p.lookNextChar(cnt))) cnt++;
        //next should be an '.' else it's not a floating point
        if(p.lookNextChar(cnt) != '.') return nullptr;
	
        //check if negative
		if(p.getChar() == '-'){
            p.nextChar();
            negative = true;
        }
    }else{
        //isn't even an integer :(
        return nullptr;
    }

    //yes create floatingpoint
    val = p.getChar() - '0';

    //try to increase integer until limit reached
    while(isdigit(p.nextChar())){
        val *= 10;
        val += p.getChar() - '0';
    }
    //read further with count for 'after comma'
    int cnt = 0;
    while(isdigit(p.nextChar())){
        val *= 10;
        val += p.getChar() - '0';
        cnt++;
    }

    //divide by 10^(count)
    while(cnt--) val /= 10;
        
    //if value is negative, negate it now
    if(negative) val *= -1;
    
    return new FloatingPoint(val);
}

/* Add operators */
Operand &FloatingPoint::operator+=(FloatingPoint &op){
    op.val += val;
    return op;
}
Operand &FloatingPoint::operator+=(Integer &op){
    op.val += val;
    return op;
}
/* Minus operators */
Operand &FloatingPoint::operator-=(FloatingPoint &op){
    op.val -= val;
    return op;
}
Operand &FloatingPoint::operator-=(Integer &op){
    op.val -= val;
    return op;
}
/* Multiply operators */
Operand &FloatingPoint::operator*=(FloatingPoint &op){
    op.val *= val;
    return op;
}
Operand &FloatingPoint::operator*=(Integer &op){
    op.val *= val;
    return op;
}
/* Divide operators */
Operand &FloatingPoint::operator/=(FloatingPoint &op){
    op.val /= val;
    return op;
}
Operand &FloatingPoint::operator/=(Integer &op){
    //always goto floating point, by casting to floatingpoint
    op.val /= val;
    return op;
}
/* Power operators */
Operand &FloatingPoint::pow(Integer &op){
    op.val = std::pow(op.val, val);
    return op;
}
Operand &FloatingPoint::pow(FloatingPoint &op){
    op.val = std::pow(op.val, val);
    return op;
}


/* Special set operator */
Operand &FloatingPoint::operator=(FloatingPoint &op){
    op.val = val;
    return op;
}
Operand &FloatingPoint::operator=(Integer &op){
    op.val = val;
    return op;
}

/* Equality operators */
bool FloatingPoint::operator==(const FloatingPoint &op) const{
    if(op.val == val) return true;
    return false;
}
bool FloatingPoint::operator==(const Integer &op) const{
    if(op.val == val) return true;
    return false;
}
bool FloatingPoint::operator==(const Void &op) const{
    return false;
}

/* Lower then operators */
bool FloatingPoint::operator<(const FloatingPoint &op) const{
    if(op.val > val) return true;
    return false;
}
bool FloatingPoint::operator<(const Integer &op) const{
    if(op.val > val) return true;
    return false;
}
bool FloatingPoint::operator<(const Void &op) const{
    return false;
}
