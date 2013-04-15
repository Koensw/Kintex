#include <iostream>
#include <cctype>

#include "names.h"
//FIXME: see line 47
#include "../operators/operators.h"
#include "../../processor.h"

using namespace kintex;

/* Make a new function */
Function *FunctionCreator::create(Processor &p){
    //create function
    return nullptr;
    /* std::string varName;
    if(isalpha(p.getChar())){
        varName += p.getChar();
        
        //loop until space or end of expression
        int i = 1;
        while(isalpha(p.lookNextChar(i)) && FIXME: p.isExpressionLeft(i)){
            varName += p.lookNextChar(i);
            ++i;
        }
        int save=i;
        
        //skip spaces
        while(isspace(p.lookNextChar(i)) && FIXME: p.isExpressionLeft(i)) ++i;
        
        //check for '(' and ')'
        if(p.lookNextChar(i) == '('){
            ++i;
            while(p.lookNextChar(i) != ')'){
                //stop if end of processing is reached
                if(!p.isExpressionLeft(i)) return nullptr;
                
                ++i;
            }
        }else return nullptr;
        ++i;
        
		return nullptr;
        std::deque<Expression> args;
        if(save - i != 2){
            //calculate positions
            int begin = (p.current-p.line->code.begin()) + save;
            Line::Positions pos = std::make_pair(begin, begin + (i - save));
            
            //build argument processor
            Processor proc(p.getTokenList(), p.line, pos);
            
            //FIXME: process from left to right instead of right to left!
            //get variables
            Expression expr = proc.getNextExpression();
            if(typeid(expr->getContents()) != typeid(Variable)) return nullptr;
            args.push_back(expr);
            while(proc.isPrevExpressionLeft()){
                Expression expr = proc.getPrevExpression();
                if(typeid(expr->getContents()) != typeid(Variable)) return nullptr;
                args.push_front(expr);
            }
            
            //finish processor
            proc.finish();  
        }
        //goto current token
        while(i--!=0) p.nextChar();
        
        //built token and return it
        Function *func = new Function(varName, std::vector<Expression>(args.begin(), args.end())); 
        return func;
		
    }
    
    //if creation was not succesfull return nullptr
    return nullptr;*/
}