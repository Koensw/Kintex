#include <iostream>

#include "operands.h"
#include "../../processor.h"

using namespace kintex;

/* Make a new integer from a string */
Void *Void::create(Processor &p){
    //check if match:
    //no: return NULL
    if(p.getChar() != 'v') return nullptr;
    if(p.lookNextChar(1) != 'o') return nullptr;
    if(p.lookNextChar(2) != 'i') return nullptr;
    if(p.lookNextChar(3) != 'd') return nullptr;

    //yes: goto next chars and create the operator
    for(int i=0; i<4; ++i) p.nextChar();
    return new Void();
}

/* Equality operators */
bool Void::operator==(const FloatingPoint &op) const{
    return false;
}
bool Void::operator==(const Integer &op) const{
    return false;
}
bool Void::operator==(const Void &op) const{
    return true;
}

/* Lower then operators */
bool Void::operator<(const FloatingPoint &op) const{
    return false;
}
bool Void::operator<(const Integer &op) const{
    return false;
}
bool Void::operator<(const Void &op) const{
    return true;
}