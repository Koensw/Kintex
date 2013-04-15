#ifndef KINTEX_H
#define KINTEX_H

#include <string>
#include <vector>
#include <memory>

#include "../../external/handle.h"

namespace kintex{
    class Tokenizer;
    class Token;
    class Operand;
    class TokenList;
    
    /* Defines a line of code that is used in processors and token */
	/* FIXME: rename to Code ?? */
    struct Line{
        int number;
        std::string file;
		std::pair<size_t,size_t> pos;
		std::pair<size_t,size_t> line_pos;
		std::shared_ptr<std::string> code;
    };
    
    /* Pointerlike handle for Tokens */
    class Expression: public Handle<Token>{
    public:
        Expression();
        Expression(Token *tok);
        Expression(Token &tok);
        Expression(const Token &tok);
    };
    /* Specific pointerlike handle for Operands --> can convert to expression */
    class Value: public Handle<Operand> {
    public:
        Value(Operand *op);
        Value(Operand &op);
        Value(const Operand &op);
        
        operator Expression() const;
    };
    /* Statement is a wrapper for separated expressions */
    class Statement{
    public:
        /* Default constructor */
        Statement();
        /* Initialize statement from expression */
        Statement(const Expression &expression): expr(expression) {}
        
        /* Proxy behavious */
        std::ostream &display(std::ostream &out) const;
        Expression result();
    private:
        Expression expr;
    };
    
    /* Overloaded token operators */
    std::ostream &operator<<(std::ostream &, const Expression &);
    std::ostream &operator<<(std::ostream &, const Statement &);
}

#endif
