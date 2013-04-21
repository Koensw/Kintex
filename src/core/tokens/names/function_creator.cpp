#include <iostream>
#include <cctype>

#include "names.h"
#include "../operators/operators.h"
#include "../../processor.h"

using namespace kintex;

/* Make a new function */
Function *FunctionCreator::create(Processor &p){
    //create function
	std::string funcName;
	if(isalpha(p.getChar())){
        funcName += p.getChar();
		Position pos = p.getPos();
        
        //loop until space or end of expression
        int i = 1;
        while(isalpha(p.lookNextChar(i))){
            funcName += p.lookNextChar(i);
            ++i;
        }
        
        //skip spaces
        while(isspace(p.lookNextChar(i))) ++i;
		
		//check if it is a function (TODO: functions start with '(' ??)
        if(p.lookNextChar(i) != '(') return nullptr;
			  
        //skip tokens
        while(i--!=0) p.nextChar();
        
		//read arguments
		std::vector<Expression> args;
		while(true){
			Expression expr = p.getNextExpression(pos, true);
			if(typeid(expr->getContents()) == typeid(Void)) break; 
			args.push_back(expr);
		}
			  
        //built token
        Function *func = new Function(funcName, args); 
        return func;
    }
    return nullptr;
}