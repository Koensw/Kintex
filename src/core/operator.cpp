#include <string>
#include <iostream>

#include "operator.h"
#include "interpreter.h"

using namespace kintex;
using std::string;
using std::ostream;

/* Binary operator */
ostream &BinaryOperator::display(ostream &out) const{
    out << "(" << children[0] << op << children[1] << ")";
    return out;
}

/* Unary operator */
ostream &RightUnaryOperator::display(ostream &out) const{
    out << "(" << op << children[0] << ")"; 
    return out;
}