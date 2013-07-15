/*
 * DEFAULT KINTEX LIBRARY (VERSION 1.0)
 */

#ifndef KINTEX_DEFAULT_LIBRARY_H
#define KINTEX_DEFAULT_LIBRARY_H 

#include <vector>

#include "../src/core/env.h"
#include "../src/core/tokens/control/control.h"
#include "../src/core/interpreter.h"
#include "../src/core/tokens/names/names.h"
#include "../src/core/tokens/operands/operands.h"
#include "../src/core/tokens/operators/operators.h"

/**
 * Define all custom functions
 */

/** Core functions */
//return value for bracket-operator
class ReturnFunction: public kintex::FunctionBase{
public:
    ReturnFunction(): kintex::FunctionBase("return", 1) {}
    ReturnFunction *clone() const{ return new ReturnFunction(*this); }
    
    kintex::Value operator()(std::vector<kintex::Expression>, kintex::Environment &env);
};

/** Comparision functions */
//if first argument is true, execute second
class IfFunction: public kintex::FunctionBase{
public:
    IfFunction(): kintex::FunctionBase("if", 2) {}
    IfFunction *clone() const{ return new IfFunction(*this); }
    
    kintex::Value operator()(std::vector<kintex::Expression>, kintex::Environment &env);
    
    /* other display operator */
    std::ostream &display(std::vector<kintex::Expression> values, std::ostream &out) const{
        out << "if(" << values[0] << ") " << values[1];
        return out;
    }
};
//if first argument is true, execute second until second argument is false
class WhileFunction: public kintex::FunctionBase{
public:
    WhileFunction(): kintex::FunctionBase("while", 2) {}
    WhileFunction *clone() const{ return new WhileFunction(*this); }
    
    kintex::Value operator()(std::vector<kintex::Expression>, kintex::Environment &env);
    
    /* other display operator */
    std::ostream &display(std::vector<kintex::Expression> values, std::ostream &out) const{
        out << "while(" << values[0] << ") " << values[1];
        return out;
    }
};

/** Print functions */
//print result expression to std-out
class PrintFunction: public kintex::FunctionBase{
public:
    PrintFunction(): kintex::FunctionBase("print", 1) {}
    PrintFunction *clone() const{ return new PrintFunction(*this); }
    
	kintex::Value operator()(std::vector<kintex::Expression>, kintex::Environment &env);
};

//print expression to std-out
class PrintExpressionFunction: public kintex::FunctionBase{
public:
    PrintExpressionFunction(): kintex::FunctionBase("show", 1) {}
    PrintExpressionFunction *clone() const{ return new PrintExpressionFunction(*this); }
    
    kintex::Value operator()(std::vector<kintex::Expression>, kintex::Environment &env);
};

/** Math function */
//computes square root of given number
class SquareRootFunction: public kintex::FunctionBase{
public:
    SquareRootFunction(): kintex::FunctionBase("sqrt", 1) {}
    SquareRootFunction *clone() const{ return new SquareRootFunction(*this); }
    
    kintex::Value operator()(std::vector<kintex::Expression>, kintex::Environment &env);
};

#endif