#include <algorithm> 
#include <functional>
#include <utility>
#include <tuple>

#include "control.h"
#include "../operands/operands.h"
#include "../../processor.h"

using namespace kintex;

BracketsOperator *BracketsOperator::create(Processor &p){
	//check if match
    //no: return;
    if(p.getChar() != '{') return nullptr;
	Position pos = p.getPos();
	
    //yes: read in parentheses
	p.nextChar();
	
	//add end
	p.registerGroup(this);
	save = p.getLevel();
	p.resetLevel();
	
	std::vector<Expression> stats;
	while(true){
		Expression expr = p.getNextExpression(pos, true);
		stats.push_back(expr);
		if(p.getChar() == '}') {
			p.nextChar();
			break;
		}else {
			if(p.getChar() == '\n') p.nextLine();
			p.nextChar();
			p.clearFinish();
			p.resetLevel();
		}
	}
	p.unregisterGroup();
	p.setLevel(save);
	
	return new BracketsOperator(stats);
}

bool BracketsOperator::poll(Processor &p){
	//std::cout << "BO -> poll: " << p.getChar() << std::endl;
	return (p.getChar() == '}' || p.getChar() == ';' || p.getChar() == '\n');
}

Value BracketsOperator::result(){
    //always return void (only return could change this)
    returnVal = new Void;
    //execute all statements
    if(!children.empty()) {
        //execute all statements (but not the last one)
        for(std::vector<Expression>::iterator iter = children.begin(); iter < children.end(); ++iter){
            (*iter)->parent = this;
            (*iter)->result();
            //stop processing if return value is found (check with parent, that is not set then)
            if(returnVal->getParent() != nullptr) break;
        }
    }
    if(returnVal->getParent() == nullptr) returnVal->parent = this;

    return returnVal;
}

/* Special display operator */
std::ostream &BracketsOperator::display(std::ostream &out) const{
    std::vector<Expression>::const_iterator iter = children.begin();
    out << "{";
    for(; iter < children.end() - 1; ++iter){
        out << *iter << ";"; //<< std::endl;
    }
    out << *iter << "}";
    return out;
}
