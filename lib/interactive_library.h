#ifndef KINTEX_INTERACTIVE_LIBRARY_H
#define KINTEX_INTERACTIVE_LIBRARY_H 

#include <vector>

#include "../src/program.h"
#include "../src/core/interpreter.h"
#include "../src/core/tokens/names/names.h"
#include "../src/core/tokens/operands/operands.h"

/**
 * Define all custom functions
 */

//show version information
class VersionFunction: public kintex::FunctionBase{
public:
    VersionFunction(): kintex::FunctionBase("version", 0) {}
    VersionFunction *clone() const{ return new VersionFunction(*this); }
    
    kintex::Value operator()(std::vector<kintex::Expression>, kintex::Environment &env);
};

//show help information
class HelpFunction: public kintex::FunctionBase{
public:
    HelpFunction(): kintex::FunctionBase("help", 0) {}
    HelpFunction *clone() const{ return new HelpFunction(*this); }
    
    kintex::Value operator()(std::vector<kintex::Expression>, kintex::Environment &env);
};

//quit interactive mode
class QuitFunction: public kintex::FunctionBase{
public:
    QuitFunction(): kintex::FunctionBase("quit", 0) {}
    QuitFunction *clone() const{ return new QuitFunction(*this); }
    
    kintex::Value operator()(std::vector<kintex::Expression>, kintex::Environment &env);
};

#endif