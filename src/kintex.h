#ifndef KINTEX_H
#define KINTEX_H

#include "core/interpreter.h"
#include "core/table.h"
#include "core/group.h"
#include "core/operand.h"

//change length for between!
const std::string betweenChars = ";\n";

namespace kintex{
    /* Provide some common methods, is normally the only directly reached class*/
    class Interpreter{
        public:
            /* Almost all interpreter functions return a list of processed statements */
            typedef std::vector<Statement> StatementList;

            /* Default constructor */
            Interpreter(SymbolTable &tokens, StatementGroup *sg): tokenList(tokens), sgroup(sg) {}

            /* Evaluate an string expression */
            StatementList parse(std::string, std::string = "");

            /* Read stream until one of characters is found */
            StatementList parse(std::istream&, std::string = "");

            /* Read stream until EOL */
            StatementList parseLine(std::istream&, std::string = "");
        private:
            /* Method used by other public parse functions */
            template<typename Iter> StatementList parse(Iter begin, Iter end, std::string = "");
			//Needed classes
            SymbolTable &tokenList;
			StatementGroup *sgroup;
    };
}

#endif
