#ifndef KINTEX_CONTROL_H
#define KINTEX_CONTROL_H 

#include "../../interpreter.h"
#include "../../operator.h"
#include "../../../kintex.h"
#include "../operands/operands.h"
#include "../../group.h"
#include "../../table.h"

namespace kintex{
    class Processor;
    
	/* Special default control flow group */
    class DefaultStatementGroup: public StatementGroup{
    public:
        /* Default constructor */
        DefaultStatementGroup() {}
		/* Check if end of statement is reached */
		bool poll(Processor &p);
		void before(Processor &p);
		void after(Processor &p);

		~DefaultStatementGroup(){}
	private:
		SymbolTable::iterator save;
    };
	
    /* Parentheses (special operator to combine tokens) */
    class ParenthesesOperator: public UnaryOperator, public Group{
    public:
        /* Default constructor */
        ParenthesesOperator(): UnaryOperator("(", Expression(new Void)) {}
        /* Create constructor */
        ParenthesesOperator(Expression expr): UnaryOperator("(", expr) {}
        /* Result operator */
        Value result();
		/* Poll current parentheses */
		bool poll(Processor &p);
        /* Display operator */
        std::ostream &display(std::ostream &) const;
        /* Clone operator */
        ParenthesesOperator *clone() const{ return new ParenthesesOperator(*this);}
        /* Create operator */
        ParenthesesOperator *create(Processor&);
        /* Overriding special contents operator to return the child */
        Token &getContents() { 
            return children[0]->getContents();
        }
        /* Name function */
        std::string getName() const{ return children[0]->getName();}
	private:
		SymbolTable::iterator save;
    };
    
    /* Brackets (special operator to combine statements) */
    class BracketsOperator: public Operator, public Group{
    public:
        /* Default constructor */
        BracketsOperator(): Operator("{"), returnVal(new Void) {}
        /* Create constructor */
        BracketsOperator(std::vector<Expression> statements): Operator("{"), returnVal(new Void) {
			children = statements; 
			if(children.empty()) children.push_back(Expression(new Void));
		}
        /* Result operator */
        Value result();
		/* Poll current brackets */
		bool poll(Processor &p);
        /* Display operator */
        std::ostream &display(std::ostream &) const;
        /* Clone operator */
        BracketsOperator *clone() const{ return new BracketsOperator(*this);}
        /* Create operator */
        BracketsOperator *create(Processor&);
        /* Overriding special contents operator to return the child */
        Token &getContents() { 
            return children[0]->getContents();
        }
        /* Name function */
        std::string getName() const{ return children[0]->getName();}
		//FIXME: better way to change returnval
		Value returnVal;
	private:
		SymbolTable::iterator save;
    };
}

#endif
