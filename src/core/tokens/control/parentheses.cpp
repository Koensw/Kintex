#include <algorithm> 
#include <functional>
#include <utility>

#include "../../token.h"
#include "control.h"
#include "../operands/operands.h"
#include "../../processor.h"
#include "../../group.h"

using namespace kintex;

ParenthesesOperator *ParenthesesOperator::create(Processor &p){
    //check if match
    //no: return;
    if(p.getChar() != '(' && p.getChar() != ',') return nullptr;
	Position pos = p.getPos();
	
    //yes: read in parentheses
	p.nextChar();
	
	//add end
	p.registerGroup(this);
	save = p.getLevel();
	p.resetLevel();
	
	Expression expr = p.getNextExpression(pos, true);
	
	//skip ")" not ,
	if(p.getChar() == ')') p.nextChar();	
	
	p.unregisterGroup();
	p.setLevel(save);
	return new ParenthesesOperator(expr);
}

bool ParenthesesOperator::poll(Processor &p){
	return (p.getChar() == ')' || p.getChar() == ',');
}

Value ParenthesesOperator::result(){
    //set parent of child to parent of parentheses operator (because this parentheses operator should not exist for the user!)
    children[0]->parent = getParent();
    //simply return containing expression
    return children[0]->result();
}

/* Special display operator */
std::ostream &ParenthesesOperator::display(std::ostream &out) const{
    out << children[0];
    return out;
}
