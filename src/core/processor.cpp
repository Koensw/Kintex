#include <vector>
#include <cstddef>
#include <typeinfo>
#include <utility>
#include <exception>
#include <memory>

#include "processor.h"
#include "token.h"
#include "group.h"
#include "exception.h"
#include "interpreter.h"
#include "level.h"
#include "tokens/operands/operands.h"
#include "tokens/operators/operators.h"
#include "tokens/names/names.h"
#include "tokens/control/control.h"


using namespace kintex;
using std::vector;

/* Default constructor (FIXME: move constructor) */
Processor::Processor(TokenList &list, StatementGroup *g, std::string code, std::string file):
tokenList(list), level(list.begin()), depth(1), sg(g), finish(false) {
	//initialize code
	line.file = file;
	line.code = std::shared_ptr<std::string>(new std::string(code));
	line.number = 1;
	line.pos = std::make_pair(0, line.code->size());
	
	//set positions
	size_t pos = line.code->find('\n');
	if(pos == std::string::npos) line.line_pos = std::make_pair(0, line.code->size());
	else line.line_pos = std::make_pair(0, pos);
	
	//set iterator
	current = 0;
}

/*
 * CORE METHODS
 */ 

/* Get next statement */
Expression Processor::getNextStatement(){	
	//attach default statement-group
	registerGroup(sg);
	
	//do actions before
	sg->before(*this);
	
	//read expression
	Expression expr = getNextExpression(true);
	
	//do actions after
	sg->after(*this);
	
	//detach statementgroup
	unregisterGroup();
	
	//check if things left
	if(isExpressionLeft()) throw LeftToken(*this, getPrevExpression(true));
	
	//return expression
	return expr;
}

/* Next expression read */
Expression Processor::getNextExpression(bool allowEmpty){
	++depth;
	
	//check if expression is already read
	std::map<size_t, Expression>::iterator iter = prevExpr.upper_bound(depth);
	if(iter!=prevExpr.end()){
		for(auto iter2 = prevExpr.begin(); iter2!= prevExpr.end(); ++iter2){
			std::cout << iter2->first << " - " << iter2->second << std::endl;
		}
		std::cout << "-- " << iter->first << " - " << iter->second << std::endl;
		Expression returnExpr = iter->second;
		prevExpr.erase(iter);
		
		//loop through all children and set parent
		for(std::vector<Expression>::iterator child_iter = returnExpr->children.begin(); child_iter < returnExpr->children.end(); ++child_iter){
			(*child_iter)->parent = &*returnExpr;
		} 
		return returnExpr;
	}
	
	//pass all spaces
	if(getChar()==' '){
		char nextCharacter = nextChar();
		while(nextCharacter==' ') nextCharacter = nextChar();
	}
	
	//poll current top group for changes (if finish is notified, always finish until action)
	else if(!groups.empty() && !finish) {
		Group *g = groups.top();
		finish = g->poll(*this);
	}
	
	//finishing until action is taken
	if(finish){
		--depth;
		return getPrevExpression(allowEmpty);
	}
	
    //save level and begin with token under
    TokenList::iterator currentLevel = level;
    level = tokenList.begin();
   
    while(level!=tokenList.end()){
        //search for next token if level has higher precedence
        if((level->getAssoc() == Level::Assoc::LEFT && level->getIndex() > currentLevel->getIndex()) 
            || (level->getAssoc() == Level::Assoc::RIGHT && level->getIndex() >= currentLevel->getIndex())){
            for(Level::iterator iter = level->begin(); iter < level->end(); ++iter){
		        //DEBUG:
		        //std::cout << " *> " << getChar() << " - " << typeid(**iter).name() << std::endl;
		        //save current positions
		        size_t saveCurrent = current;

                //create token
                Token* tok;
                tok = (*iter)->create(*this);

                //wait until instance succeeded
                if(tok != nullptr){
					//build new expression or get old if operand points to the same
					Expression tokenExpr;
					if(tok == &(**iter)) tokenExpr = *iter; //get old expression if the same (else memory errors!)
					else tokenExpr = Expression(tok); //create an expression (smart pointer) from the token

					//add token to level if needed
					//FIXME: make interface better!
                    if((*iter)->getAddLevelIndex() != 0){
                        for(TokenList::LevelList::iterator it = tokenList.begin(); it != tokenList.end(); ++it){
                            if(it->getIndex() == (*iter)->getAddLevelIndex()) it->addTokenFront(tokenExpr);
                        }
                    }
                    
                    //set line and positions
                    tokenExpr->line = line;
					
                    //calculate default positions
                    tokenExpr->line.pos  = std::make_pair(saveCurrent, current);
                    //set positions to first and last children if possible
                    if(!tokenExpr->children.empty()){
						if(tokenExpr->children.front()->line.pos.first < tokenExpr->line.pos.first) tokenExpr->line.pos.first = tokenExpr->children.front()->line.pos.first;
						if(tokenExpr->children.back()->line.pos.second > tokenExpr->line.pos.second) tokenExpr->line.pos.second = tokenExpr->children.back()->line.pos.second;
					}
                    //add expression to previous expression stack
                    prevExpr.insert(std::make_pair(depth, tokenExpr));
					
                    //reset level and try to match next token that has a higher precedency
                    level = currentLevel;
					Expression ret = getNextExpression(allowEmpty);
					--depth;
					return ret;
                }
            }
        }

        //increase level
        ++level;
    }
    
    //check if on lowest level no token could be matched and it's not the end of expression --> then the token is unknown
    if(currentLevel->getIndex()==0) {
        throw UnknownToken(*this);
    }

    //reset level and return previous expression if nothing higher can be matched 
    level = currentLevel;
	--depth;
    return getPrevExpression(allowEmpty);
}

/* Prev expression read */
Expression Processor::getPrevExpression(bool allowEmpty){
	//search for expression
	std::map<size_t, Expression>::iterator iter = prevExpr.begin();
	
    //check if there is a token, else throw an exception
    if(iter == prevExpr.end()){
		if(allowEmpty) return Expression(new Void());
        else throw MissingToken(*this);
    }
	
	//get token and erase it from stack
	Expression returnExpr = iter->second;
	prevExpr.erase(iter);
  	
    //loop through all children and set parent
    for(std::vector<Expression>::iterator child_iter = returnExpr->children.begin(); child_iter < returnExpr->children.end(); ++child_iter){
        (*child_iter)->parent = &*returnExpr;
    } 

	//return expression
    return returnExpr;
}

/*
 * SPECIAL METHODS
 */ 
/* Goto next line */
void Processor::nextLine(){
	//set next positions
	line.line_pos.first = line.line_pos.second+1;
	line.line_pos.second = line.code->find('\n', line.line_pos.first);
	if(line.line_pos.second == std::string::npos) line.line_pos.second = line.code->size();
	
	//set next line
	++line.number;
}
