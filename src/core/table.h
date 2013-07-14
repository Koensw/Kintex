#ifndef KINTEX_TABLE_H
#define KINTEX_TABLE_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include <stack>
#include <iterator>
#include <utility>
#include <iostream>
#include <string>
#include <map>

#include "token.h"
#include "name.h"
#include "interpreter.h"

namespace kintex{ 
    class Level{
        friend class SymbolTable;
        friend class Processor;
    public:
        /* Enum to define different assoc types */
        enum Assoc{
            LEFT, RIGHT
        };
        /* Define constructor */
        Level(std::string nm, Assoc as = Level::Assoc::LEFT): prev(""), name(nm), index(0), assoc(as) {}
        
        /* Iterator typedef */
        typedef std::deque<Expression>::iterator iterator;
        
        /* Give it container behaviour */
        iterator begin() { return tokenList.begin(); }
        iterator end() { return tokenList.end(); }
        
        /* Return indexes/assoc (used by Processor) */
        double getIndex() { return index; }
        Assoc getAssoc() { return assoc; }
        
        /* Add a token after */
        void addToken(Expression expr);
        
        /* Merge tokens from other level */
        void merge(iterator begin, iterator end);
    private:
        /* Set indexes (internally used by SymbolTable) */
        void setIndex(double lev){
        	index = lev;
        }
        
        //next level (next index)
        std::string prev;
        
        //name and index
        std::string name;
        double index;
        
        //level config
        Assoc assoc; 
        
        std::deque<Expression> tokenList;
    };
   
    class SymbolTable{
    public:
        SymbolTable(Token *vd, Token *tok);
        /* Iterator typedef */
        typedef std::list<Level> LevelList;
        typedef LevelList::iterator iterator;
        
        /* Give it container behavious */
        iterator begin() { return levelList.begin(); }
        iterator end() { return levelList.end(); }
        
        /* Add level
         * - LIST ORDER DETERMINES MATCHING ORDER (NOT INDEXES)
         * - BEFORE IS ONLY A HINT (it's ignored when a level with same name is found)
         */
        iterator addLevel(Level level);
        iterator addEndLevel(Level level);
        iterator addLevel(Level level, iterator bef); 
        
        /* Merge symbol tables */
        void merge(SymbolTable &s);
        void merge(SymbolTable &s, iterator bef);
        
        iterator getLevel(std::string nm);
    private:
    	std::map<std::string, iterator> lookup;
        LevelList levelList;
        
        double mx_ind;
    };
    
    
    /* Specific pointerlike handle for DynamicTokens --> can convert to expression 
		- DEFINED HERE SO IT DOESNT GIVE A 'UNDEFINED DESTRUCTOR'-WARNING
    */
    class DynamicExpression: public Handle<DynamicToken> {
    public:
        DynamicExpression(DynamicToken *op);
        DynamicExpression(DynamicToken &op);
        DynamicExpression(const DynamicToken &op);
        
        operator Expression() const;
    };
    
    class DynamicSymbolTable{
    public:
    	DynamicSymbolTable() {
    		extend();
    	}
    	~DynamicSymbolTable() {}
    	
    	Expression getToken(std::string id);
    	bool isToken(std::string id);
    	
    	void addToken(DynamicExpression tok);
    	void removeToken(std::string id);
    	
    	void extend();
    	void reduce();
    
    private:
	    std::vector<std::map<std::string, DynamicExpression> > dynsym;
	};
}

#endif
