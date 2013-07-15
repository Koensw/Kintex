#ifndef KINTEX_OPERATOR_H
#define KINTEX_OPERATOR_H

#include <string>
#include <iostream>

#include "token.h"
#include "interpreter.h"
#include "operand.h"

namespace kintex{
    
    class Expression;
    
    /** Base operator class */
    class Operator: public Token{
        friend class UnsupportedOperation;
    public:
        /** Constructor
         */
        Operator(std::string oper): op(oper) {}
    protected:
        std::string op;
    };
    
    /** Base class for binary operator */
    class BinaryOperator: public Operator{
    public:
        /* Constructor
         */
        BinaryOperator(std::string op, Expression left, Expression right): Operator(op) {children.push_back(left); children.push_back(right);}
        /** Display operation for binary operator */
        std::ostream &display(std::ostream &) const;
    };
    
    /** Base class for unary operator */
    class UnaryOperator: public Operator{
    public:
        /* Constructor
         */
        UnaryOperator(std::string op, Expression expression): Operator(op) {children.push_back(expression);}
    };
    
    /** Base class for right unary operator */
    class RightUnaryOperator: public UnaryOperator{
    public:
        /* Constructor
         */
        RightUnaryOperator(std::string op, Expression expr): UnaryOperator(op, expr) {}
        /** Display operation for unary operator */
        std::ostream &display(std::ostream &) const;
    };
    
}

#endif