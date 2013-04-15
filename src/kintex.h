#ifndef KINTEX_INTERPRETER_H
#define KINTEX_INTERPRETER_H

#include "core/interpreter.h"
#include "core/level.h"
#include "core/token.h"
#include "core/group.h"

//TODO move to include folder!

//change length for between!
const std::string betweenChars = ";\n";

namespace kintex{
    /* Provide some common methods, is normally the only directly reached class*/
    class Interpreter{
        //friend class Processor;
        public:
            /* Almost all interpreter functions return a list of processed statements */
            typedef std::vector<Statement> StatementList;

            /* Default constructor */
            Interpreter(TokenList tokens, StatementGroup *sg): tokenList(tokens), sgroup(sg) {}

            /* Evaluate an string expression */
            StatementList parse(std::string, std::string = "");

            /* Read stream until one of characters is found */
            StatementList parse(std::istream&, std::string = "");

            /* Read stream until EOL */
            StatementList parseLine(std::istream&, std::string = "");
        private:
            /* Method used by other public parse functions */
            template<typename Iter> StatementList parse(Iter begin, Iter end, std::string = "");
            /* Methods used by friend Processor */
            //TokenList getTokenList(){ return tokenList; }
			//Needed classes
            TokenList tokenList;
			StatementGroup *sgroup;
    };
}

#endif
