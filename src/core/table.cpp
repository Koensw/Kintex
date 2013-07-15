#include "table.h"
#include "exception.h"
#include "operand.h"

using namespace kintex;

/* === STATIC SYMBOL TABLE === */

/* Add a token to a level */
void Level::addToken(Expression expr){
	//add to tokenlist
	tokenList.push_back(expr);
}

/* Merge another level */
void Level::merge(Level::iterator beg, Level::iterator end){
	while(beg!=end) {
		addToken(*beg);
		++beg;
	}
}

void Level::removeToken(Level::iterator iter){
	tokenList.erase(iter);
}

/* Build new symbol table */
SymbolTable::SymbolTable(Token *vd, Token *tok): mx_ind(1e6){
	//create a zero level
	Level zero("start");
	zero.setIndex(-1);
	levelList.push_back(zero);
    lookup[zero.name] = levelList.begin();
	
	//create a default level
	Level def("default", Level::Assoc::RIGHT);
	def.addToken(vd);
	def.addToken(tok);
    addLevel(def, levelList.end());
}

/* Get a level (from a string) */
SymbolTable::iterator SymbolTable::getLevel(std::string nm){
	auto iter = lookup.find(nm);
	if(iter == lookup.end()) throw TableException();
	return iter->second;
}

/* Add a level to the symbol table */
SymbolTable::iterator SymbolTable::addLevel(Level lev){
	return addLevel(lev, getLevel("default"));
}

SymbolTable::iterator SymbolTable::addLevel(Level lev, SymbolTable::iterator before){
	auto chk = lookup.find(lev.name);
	if(chk != lookup.end()){
		iterator iter = chk->second;
		iter->merge(lev.begin(), lev.end());
		return iter;
	}else if(before!=levelList.end()){
		double ind1 = before->getIndex();
		double ind2 = ind1-10;
		if(ind2 < 0) throw TableException();
		else if(!before->prev.empty()) ind2 = getLevel(before->prev)->getIndex();
		
		lev.setIndex((ind1+ind2)/2);
		lev.prev = before->prev;
		before->prev = lev.name;
	}else{
		lev.setIndex(mx_ind);
		mx_ind+=1e6;
		lev.prev = (--levelList.end())->name;
	}
	levelList.push_back(lev);
	return lookup[lev.name] = --levelList.end();
	
}

SymbolTable::iterator SymbolTable::addEndLevel(Level lev){
	return addLevel(lev, end());
}

/* Merge symbol table */
void SymbolTable::merge(SymbolTable &s) {
	auto iter2 = s.lookup.rend();
	for(auto iter = s.lookup.rbegin(); iter != s.lookup.rend(); ++iter){		
		if(iter->first == "default" || iter->first == "start") continue;
		
		if(iter2 != s.lookup.rend()) {
			addLevel(*iter->second, getLevel(iter2->first));
			++iter2;
		}else {
			addLevel(*iter->second);
			iter2 = s.lookup.rbegin();
		}
	}
}
 
/* === DYNAMIC SYMBOL TABLE === */
DynamicExpression::DynamicExpression(): Handle<DynamicToken>(nullptr) {}
DynamicExpression::DynamicExpression(DynamicToken *tok): Handle<DynamicToken>(tok) {}
DynamicExpression::DynamicExpression(DynamicToken &tok): Handle<DynamicToken>(tok.clone()) {}
DynamicExpression::DynamicExpression(const DynamicToken &tok): Handle<DynamicToken>(tok.clone()) {}

DynamicExpression DynamicSymbolTable::getToken(std::string id, bool est){
	for(auto iter = dynsym.rbegin(); iter != dynsym.rend(); ++iter){
		auto chk = iter->find(id);
		if(chk != iter->end()) return chk->second;
		if(est) break;
	}
	return DynamicExpression();
}
bool DynamicSymbolTable::isToken(std::string id){
	return getToken(id).isBound();
}
    	
void DynamicSymbolTable::addToken(DynamicExpression expr, bool red){
	if(!red){
		for(auto iter = dynsym.rbegin(); iter != dynsym.rend(); ++iter){
			auto chk = iter->find(expr->getId());
			if(chk != iter->end()) {
				chk->second = expr;
				return;
			}
		}
	}
	
	dynsym.back()[expr->getId()] = expr;
}
//TODO: implement
void DynamicSymbolTable::removeToken(std::string id){
	for(auto iter = dynsym.rbegin(); iter != dynsym.rend(); ++iter){
		auto chk = iter->find(id);
		if(chk != iter->end()) {
			iter->erase(chk);
			return;
		}
	}
}

void DynamicSymbolTable::extend(Scope scp){
	dynsym.push_back(std::map<std::string, DynamicExpression>());
	scps.push_back(scp);
}
void DynamicSymbolTable::reduce(){
	dynsym.pop_back();
	scps.pop_back();
	if(dynsym.empty()) extend(DynamicSymbolTable::NORMAL);
}
