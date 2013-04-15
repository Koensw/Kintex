#include "control.h"

using namespace kintex;

bool DefaultStatementGroup::poll(Processor &p){
	//std::cout << "SG -> poll: " << p.getChar() << std::endl;
	
    //check if action is needed
    return (p.getChar() == '\n' || p.getChar() == ';' || p.getChar() == 0);
}

void DefaultStatementGroup::before(Processor &p){
	p.resetLevel();
}

void DefaultStatementGroup::after(Processor &p){
	//handle
	if(p.getChar() == '\n') p.nextLine();
	if(p.getChar() != 0) p.nextChar();

}