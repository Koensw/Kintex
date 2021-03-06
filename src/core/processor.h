#ifndef KINTEX_PROCESSOR_H
#define KINTEX_PROCESSOR_H

#include <string>
#include <map>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <utility>
#include <memory>
#include <cctype>
#include <iostream>

#include "table.h"
#include "interpreter.h"
#include "group.h"
#include "token.h"

namespace kintex{
    class Expression;
    
    //Processor
	struct Position{
		size_t pos;
		bool operator<(const Position p2) const{
			return pos < p2.pos;
		}
	};
	
    class Processor{
    public:
        /* all syntax errors are friends */
        friend class SyntaxError;
        friend class UnknownToken;
        friend class MissingToken;
        friend class LeftToken;
        
        /** Default constructor */
		Processor(SymbolTable &list, StatementGroup *g, std::string code, std::string file = ""); 
		
		/** 'Normal'-assignment is not possible */
        Processor operator=(const Processor &p) = delete;
        
        /** Get next statement */
		Expression getNextStatement();
		bool isStatementLeft() const{
			return (current != line.code->size());
		}
        
        /** Get current and next character */
        char getChar() const{
            //return 0 if no token left
            if(current == line.code->size()) return 0;
            return (*line.code)[current]; 
        }
        char nextChar(int next = 1){
            //return 0 if no token left
            if(current == line.code->size() || (current+next) > line.code->size()) return 0;
            //increase counter
            current += next;
            //return now current char
            return getChar();
        }
        /** Lookup next chars/string without going to it *
         * WARNING: read next char if you got a real match
         */
        char lookNextChar(int next = 1) const{
			if(current + next == line.code->size()) return 0;
			return (*line.code)[current + next]; 
        }
        std::string lookString(int length = 1) const{
            //return 0 if no token left
			if(current + length > line.code->size()) return "";
			return line.code->substr(current, length); 
        }
        
        /** Returns current position */
        Position getPos() const{
			Position p;
			p.pos = current;
			return p;
		}
        
        /** Get next and previous expressions (and check if expressions left), now support parents */
        Expression getNextExpression(Position pos, bool allowEmpty = false, bool spec = false);
        Expression getPrevExpression(Position pos, bool allowEmpty = false);
		
		/** Check if there is an expression on the stack */
		bool isExpressionLeft() { 
			return !prevExpr.empty(); 
		}
		
		/** Register and unregister groups */
		//register a group
		void registerGroup(Group *g){
			groups.push(g);  
		}
		//unregisters top-group
		void unregisterGroup(){
			finish = false;
			groups.pop();
		}
		//clear finish flag
		void clearFinish(){ finish = false; }
		
		/** Register a static global */
		void registerGlobal(std::string nm){ regnm = nm; }
		
		/** Special methods for special tokens */
		//goto next line
		void nextLine();
		//(re)set level and get level
		SymbolTable::iterator getLevel() const { return level; }
		void setLevel(SymbolTable::iterator iter) { level = iter; }
		void resetLevel() { level = symTable.begin(); }
        
    private:
		/** Converts iterator to correct expression */
		Expression getExpression(std::multimap<Position, Expression>::iterator, bool allowEmpty = false);
		
        /* List of tokens and iterator */
        SymbolTable &symTable;
        SymbolTable::iterator level;
        
        /* List of not assigned expressions */
        std::multimap<Position, Expression> prevExpr;
		
		/* Flow control */
		StatementGroup *sg;
		bool finish;
		std::stack<Group*> groups;
		
		/* Registering globals (static) */
		std::string regnm;
		
        /* Interface for currently executed code */
        Line line;
		size_t current;
        
    };
}

#endif
