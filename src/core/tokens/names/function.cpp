#include <iostream>
#include <cctype>

#include "names.h"
#include "../../processor.h"

using namespace kintex;

/* Make a new function */
InstantiatedFunction *FunctionBase::create(Processor &p){
	//check if correct
	if(p.lookString(name.size()) == name && !isalpha(p.lookNextChar(name.size()))){
		Position pos = p.getPos();
		p.nextChar(name.size());
		
		//get argument values
		std::deque<Expression> argVals;
		for(size_t i=0; i<argumentSize; ++i){
			Expression expr = p.getNextExpression(pos);
			argVals.push_back(expr);
		}
		
		//make instance
		InstantiatedFunction *instance = new InstantiatedFunction(this, std::vector<Expression>(argVals.begin(), argVals.end()));
        return instance;
	}
	
	return nullptr;
    //check if function is same
    /*if(p.getChar() == name[0]){
        unsigned int i = 1;
        for(;i<name.length();++i){
            if (p.lookNextChar(i) != name[i]) return nullptr;
        }
        
        //check if finished
        if (i==name.length()){
            //check if not more alpha characters
            if(isalpha(p.lookNextChar(i))) return nullptr;
            
            //match --> return instantiated function
            while(i--!=0) p.nextChar();             
            
            //FIXME: process from left to right instead of right to left!
            //get argument values
            std::deque<Expression> argVals;
            //get first argument with nextExpression if not an no-argument function
            if(argumentSize != 0){
                Expression expr = p.getNextExpression();
                argVals.push_back(expr);
                //get the other functions
                for(size_t j=1; j<argumentSize; ++j){
                    Expression expr = p.getPrevExpression();
                    argVals.push_front(expr);
                }
            }
            InstantiatedFunction *instance = new InstantiatedFunction(this, std::vector<Expression>(argVals.begin(), argVals.end()));
            return instance;
        }
    }
    
    //if instantiation was not succesful return nullptr
    return nullptr;*/
}