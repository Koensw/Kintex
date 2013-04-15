#ifndef KINTEX_LEVEL_H
#define KINTEX_LEVEL_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include <stack>
#include <iterator>
#include <utility>
#include <iostream>

#include "interpreter.h"
#include "token.h"

namespace kintex{  
    class Level{
        friend class TokenList;
    public:
        /* Enum to define different assoc types */
        enum Assoc{
            LEFT, RIGHT
        };
        /* Define constructor */
        Level(int lev, Assoc as = Level::Assoc::LEFT): index(0), assoc(as) {
            setIndex(lev);
        }
        
        /* Get/set methods */
        int getIndex() { return index; }
        void setIndex(int lev){
            if(lev <= 0) throw std::logic_error("Level is not allowed to have an index lower then or equal to zero, they are reserved for internal use.");
            index = lev;
        }
        Assoc getAssoc() { return assoc; }
        void setAssoc(Assoc as) { assoc = as; } 
        
        /* Iterator typedef */
        typedef std::deque<Expression>::iterator iterator;
        
        /* Give it container behaviour */
        iterator begin() { return tokenList.begin(); }
        iterator end() { return tokenList.end(); }
        
        /* Add a token to the list */
        void addToken(Token *tok);
        /* Add a token after --> used by variables and functions */
        void addToken(Expression expr);
        /* Add a token to the list */
        void addTokenFront(Token *tok);
        /* Add a token after --> used by variables and functions */
        void addTokenFront(Expression expr);
    private:
        /* Private default constructor to construct zero-level (default) */
        Level(Assoc as = Level::Assoc::LEFT): index(0), assoc(as) {}
        
        int index;
        Assoc assoc; 
        
        std::deque<Expression> tokenList;
    };
    
    
    //TODO: rename to LevelList && move to kintex.cpp
    class TokenList{
    public:
        TokenList(){
            //add a default zero-level (always needed)
            levelList.push_back(Level());
        }
        /* Iterator typedef */
        typedef std::list<Level> LevelList;
        typedef LevelList::iterator iterator;
        
        /* Give it container behavious */
        iterator begin() { return levelList.begin(); }
        iterator end() { return levelList.end(); }
        
        /* Add level
         * - LEVEL ORDER DEFINES THE MATCHING ORDER (NOT THE LEVEL) 
         */
        void addNextLevel(Level level) { levelList.push_back(level); }
        void addLevelAfter(iterator iter, Level level) { levelList.insert(iter++, level); }
        void addLevelBefore(iterator iter, Level level) { levelList.insert(iter, level); }
        
        /* Returns reference to the level from an index */
        Level &getLevel(int index){
            for(LevelList::iterator iter = levelList.begin(); iter != levelList.end(); ++iter){
                if(iter->getIndex() == index) return *iter;
            }
            //if no match return null level (FIXME: other solution ?)
            return getLevel(0);
        }
    private:
        LevelList levelList;
    };
}

#endif
