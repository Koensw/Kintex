#ifndef KINTEX_OPERATORS_H
#define KINTEX_OPERATORS_H 

#include "../../interpreter.h"
#include "../../operator.h"
#include "../operands/operands.h"
#include "../../operand.h"
//TODO: necessary include
#include "../../../kintex.h"

namespace kintex{
    class Processor;
    
    /* Default operators */
    class AddOperator: public BinaryOperator{
    public:
        /* Default constructor */
        AddOperator(): BinaryOperator("+", Expression(new Void), Expression(new Void)) {}
        /* Create constructor */
        AddOperator(Expression lhs, Expression rhs): BinaryOperator("+", lhs, rhs) {}
        /* Result operator */
        Value result(Environment &env);
        /* Clone operator */
        AddOperator *clone() const{ return new AddOperator(*this);}
        /* Create operator */
        AddOperator *create(Processor&);
        /* Name function */
        std::string getName() const{ return "addition";}
    };
    
    class SubtractOperator: public BinaryOperator{
    public:
        /* Default constructor */
        SubtractOperator(): BinaryOperator("-", Expression(new Void), Expression(new Void)){}
        /* Create constructor */
        SubtractOperator(Expression lhs, Expression rhs): BinaryOperator("-", lhs, rhs){}
        /* Result operator */
        Value result(Environment &env);
        /* Clone operator */
        SubtractOperator *clone() const{ return new SubtractOperator(*this);}
        /* Create operator */
        SubtractOperator *create(Processor&);
        /* Name function */
        std::string getName() const{ return "subtract";}
    };
    class MultiplyOperator: public BinaryOperator{
    public:
        /* Default constructor */
        MultiplyOperator(): BinaryOperator("*", Expression(new Void), Expression(new Void)) {}
        /* Createconstructor */
        MultiplyOperator(Expression lhs, Expression rhs): BinaryOperator("*", lhs, rhs) {}
        /* Result operator */
        Value result(Environment &env);
        /* Clone operator */
        MultiplyOperator *clone() const{ return new MultiplyOperator(*this);}
        /* Create operator */
        MultiplyOperator *create(Processor&);
        /* Name function */
        std::string getName() const{ return "multiplication";}
    };
    class DivideOperator: public BinaryOperator{
    public:
        /* Default constructor */
        DivideOperator(): BinaryOperator("/", Expression(new Void), Expression(new Void)) {}
        /* Create constructor */
        DivideOperator(Expression lhs, Expression rhs): BinaryOperator("/", lhs, rhs) {}
        /* Result operator */
        Value result(Environment &env);
        /* Clone operator */
        DivideOperator *clone() const{ return new DivideOperator(*this);}
        /* Create operator */
        DivideOperator *create(Processor&);
        /* Name function */
        std::string getName() const{ return "divide";}
    };
    class EqualityOperator: public BinaryOperator{
    public:
        /* Default constructor */
        EqualityOperator(): BinaryOperator("==", Expression(new Void), Expression(new Void)) {}
        /* Create constructor */
        EqualityOperator(Expression lhs, Expression rhs): BinaryOperator("==", lhs, rhs) {}
        /* Result operator */
        Value result(Environment &env);
        /* Clone operator */
        EqualityOperator *clone() const{ return new EqualityOperator(*this);}
        /* Create operator */
        EqualityOperator *create(Processor&);
        /* Name function */
        std::string getName() const{ return "equality";}
        
    };
    class InequalityOperator: public BinaryOperator{
    public:
        /* Default constructor */
        InequalityOperator(): BinaryOperator("!=", Expression(new Void), Expression(new Void)) {}
        /* Create constructor */
        InequalityOperator(Expression lhs, Expression rhs): BinaryOperator("!=", lhs, rhs) {}
        /* Result operator */
        Value result(Environment &env);
        /* Clone operator */
        InequalityOperator *clone() const{ return new InequalityOperator(*this);}
        /* Create operator */
        InequalityOperator *create(Processor&);
        /* Name function */
        std::string getName() const{ return "inequality";}
    };
	class SmallerOperator: public BinaryOperator{
    public:
        /* Default constructor */
        SmallerOperator(): BinaryOperator("<", Expression(new Void), Expression(new Void)), equal(false) {}
        /* Create constructor */
        SmallerOperator(Expression lhs, Expression rhs, bool eq): BinaryOperator("<", lhs, rhs), equal(eq) {}
        /* Result operator */
        Value result(Environment &env);
        /* Clone operator */
        SmallerOperator *clone() const{ return new SmallerOperator(*this);}
        /* Create operator */
        SmallerOperator *create(Processor&);
		/* Display operator (for special =) */
		std::ostream &display(std::ostream &) const;
        /* Name function */
        std::string getName() const{ return "smaller";}
	private:
		bool equal;
    };
	class BiggerOperator: public BinaryOperator{
    public:
        /* Default constructor */
        BiggerOperator(): BinaryOperator(">", Expression(new Void), Expression(new Void)), equal(false) {}
        /* Create constructor */
        BiggerOperator(Expression lhs, Expression rhs, bool eq): BinaryOperator(">", lhs, rhs), equal(eq) {}
        /* Result operator */
        Value result(Environment &env);
        /* Clone operator */
        BiggerOperator *clone() const{ return new BiggerOperator(*this);}
        /* Create operator */
        BiggerOperator *create(Processor&);
		/* Display operator (for special =) */
		std::ostream &display(std::ostream &) const;
        /* Name function */
        std::string getName() const{ return "bigger";}
	private:
		bool equal;
    };
    /* Set operator (=) a bit special because it only works on variables (and functions) */
    class SetOperator: public BinaryOperator{
    public:
        /* Default constructor */
        SetOperator(): BinaryOperator("=", Expression(new Void), Expression(new Void)) {}
        /* Create constructor */
        SetOperator(Expression lhs, Expression rhs): BinaryOperator("=", lhs, rhs) {}
        /* Result operator */
        Value result(Environment &env);
        /* Clone operator */
        SetOperator *clone() const{ return new SetOperator(*this);}
        /* Create operator */
        SetOperator *create(Processor&);
        /* Name function */
        std::string getName() const{ return "assign";}
    };
    
    /* Exponentation operator */
    class ExponentOperator: public BinaryOperator{
    public:
        /* Default constructor */
        ExponentOperator(): BinaryOperator("^", Expression(new Void), Expression(new Void)) {}
        /* Create constructor */
        ExponentOperator(Expression lhs, Expression rhs): BinaryOperator("^", lhs, rhs) {}
        /* Result operator */
        Value result(Environment &env);
        /* Clone operator */
        ExponentOperator *clone() const{ return new ExponentOperator(*this);}
        /* Create operator */
        ExponentOperator *create(Processor&);
        /* Name function */
        std::string getName() const{ return "power";}
    }; 
}

#endif
