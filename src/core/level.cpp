#include "level.h"

using namespace kintex;

/* Add a token to a level 
 * WARNING: control of pointer is passed to Level!
 */
void Level::addToken(Token *tok){
    //bind it to an expression;
    Expression expr(tok);
    //add to tokenlist
    tokenList.push_back(expr); 
}

void Level::addToken(Expression expr){
	//add to tokenlist
	tokenList.push_back(expr);
}

/* Add a token to a level 
 * WARNING: control of pointer is passed to Level!
 */
void Level::addTokenFront(Token *tok){
    //bind it to an expression;
    Expression expr(tok);
    //add to tokenlist
    tokenList.push_front(expr); 
}

void Level::addTokenFront(Expression expr){
    //add to tokenlist
    tokenList.push_front(expr);
}