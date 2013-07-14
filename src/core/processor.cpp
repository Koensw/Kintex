#include <vector>
#include <cstddef>
#include <typeinfo>
#include <utility>
#include <exception>
#include <memory>
#include <cmath>

#include "processor.h"
#include "token.h"
#include "group.h"
#include "exception.h"
#include "interpreter.h"
#include "table.h"
#include "tokens/operands/operands.h" //FIXME: shouldn't include tokens

using namespace kintex;
using std::vector;

/* Default constructor */
Processor::Processor(SymbolTable &list, StatementGroup *g, std::string code, std::string file):
symTable(list), level(list.begin()), sg(g), finish(false) {
	//initialize code
	line.file = file;
	line.code = std::shared_ptr<std::string>(new std::string(code));
	line.number = 1;
	line.pos = std::make_pair(0, line.code->size());
	
	//remove tabs
	for(size_t i=0; i<line.code->size(); ++i){
		if((*line.code)[i] == '\t') (*line.code)[i] = ' ';
	}
	
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
	Position pos = getPos();
	Expression expr = getNextExpression(pos, true, true);
	
	//do actions after
	sg->after(*this);
	
	//detach statementgroup
	unregisterGroup();
	
	//check if things left
	if(isExpressionLeft()) throw LeftToken(*this, getExpression(prevExpr.begin()));
	
	//return expression
	return expr;
}

/* Get expression from iterator */
Expression Processor::getExpression(std::map<Position, Expression>::iterator iter, bool allowEmpty){
	if(iter == prevExpr.end()){
		if(allowEmpty) return Expression(new Void());
        else throw MissingToken(*this);
	}
	
	Expression returnExpr = iter->second;
	prevExpr.erase(iter);
		
	//loop through all children and set parent
	for(std::vector<Expression>::iterator child_iter = returnExpr->children.begin(); child_iter < returnExpr->children.end(); ++child_iter){
		(*child_iter)->parent = &*returnExpr;
	} 
	return returnExpr;
}

/* Next expression read */
Expression Processor::getNextExpression(Position pos, bool allowEmpty, bool spec){
	//check if expression is already read (return then)
	std::map<Position, Expression>::iterator iter = prevExpr.lower_bound(pos);
	if(!spec && iter!=prevExpr.end()) return getExpression(iter, allowEmpty);
	
	//pass all spaces
	if(getChar()==' '){
		char nextCharacter = nextChar();
		while(nextCharacter==' ') nextCharacter = nextChar();
	}
	//poll current top group for changes (if finish is notified, always finish until action)
	if(!groups.empty() && !finish) {
		Group *g = groups.top();
		finish = g->poll(*this);
	}
	
	//finishing until action is taken
	if(finish){
		//set to return first next expression
		std::map<Position, Expression>::iterator iter = prevExpr.lower_bound(pos);
		return getExpression(iter, allowEmpty);
	}
	
    //save level and begin with token under
    SymbolTable::iterator currentLevel = level;
    level = symTable.begin();
   
   	//std::cout << symTable.end() << std::endl;
    while(level!=symTable.end()){
        //search for next token if table.has higher precedence
        if((level->getAssoc() == Level::Assoc::LEFT && (level->getIndex() - currentLevel->getIndex()) > 1e-10) 
            || (level->getAssoc() == Level::Assoc::RIGHT && 
               (level->getIndex() > currentLevel->getIndex() || std::abs(level->getIndex() - currentLevel->getIndex()) < 1e-10))){
            for(auto iter = level->begin(); iter != level->end(); ++iter){
		        //DEBUG:
		        //char cur = getChar();
				//std::cout << " *> " << cur << " - " << typeid(**iter).name() << " - " << level->name << std::endl; 
				
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

					//add token to level if registered
					if(!regnm.empty()){
						symTable.getLevel(regnm)->addToken(tokenExpr);
						regnm = "";
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
					while(tokenExpr->line.pos.first < tokenExpr->line.pos.second && isspace((*line.code)[tokenExpr->line.pos.second-1])) --tokenExpr->line.pos.second;
					
                    //add expression to previous expression stack
					Position p;
					p.pos = saveCurrent;
                    prevExpr.insert(std::make_pair(p, tokenExpr));
					
                    //reset level and try to match next token that has a higher precedency
                    level = currentLevel;
					Expression ret = getNextExpression(pos, allowEmpty, true);
					return ret;
                }
            }
        }

        //increase level
        ++level;
    }
    
    //check if on lowest level no token could be matched and it's not the end of expression --> then the token is unknown
    if(currentLevel->getIndex()<0) {
        throw UnknownToken(*this);
    }

    //reset level and return previous expression if nothing higher can be matched 
    level = currentLevel;
	iter = prevExpr.lower_bound(pos);
	return getExpression(iter, allowEmpty);
}

/* Prev expression read */
Expression Processor::getPrevExpression(Position pos, bool allowEmpty){
	//search for expression
	std::map<Position, Expression>::iterator iter = prevExpr.lower_bound(pos);
	
    //check if there is a token, else throw an exception
    if(iter == prevExpr.begin()){
		if(allowEmpty) return Expression(new Void());
        else throw MissingToken(*this);
    }
    --iter;
	
	return getExpression(iter, allowEmpty);
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
