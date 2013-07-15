//define version information

#define VERSION "2.1"
#define YEAR "2012-2013"

#include <iostream>
#include <fstream>
#include <utility>
#include <typeinfo>
#include <iterator>
#include <string>
#include <cstddef>
#include <cstdlib>

//main
#include "program.h"
#include "kintex.h"
#include "core/table.h"
#include "core/exception.h"
#include "core/token.h"
#include "core/processor.h"
#include "core/operand.h"
#include "core/operator.h"
#include "core/interpreter.h"
#include "core/env.h"
#include "../external/getopt_pp.h"

/* include all core tokens
    TODO: should be configurable
 */
#include "core/tokens/operators/operators.h"
#include "core/tokens/operands/operands.h"
#include "core/tokens/names/names.h"
#include "core/tokens/control/control.h"
#include "../lib/default_library.h"
#include "../lib/interactive_library.h"

using namespace kintex;

//globals
std::string prog_name;
bool quitInteractive = false; //also extern

//possible MODES
enum Mode {INTERACTIVE_MODE,FILE_MODE,DIRECT_MODE};

/* 
 * GRAMMAR FUNCTIONS
 * TODO: support dynamic grammar
 */

// add all interactive library functions 
SymbolTable addInteractiveLibrary(){
	Level baseFunctionLevel("base", Level::Assoc::RIGHT);
	baseFunctionLevel.addToken(new VersionFunction);
	baseFunctionLevel.addToken(new HelpFunction);
	baseFunctionLevel.addToken(new QuitFunction);
	
	SymbolTable table(new Void, new Void);
	table.addLevel(baseFunctionLevel);
	
    return table;
}

// add all default library functions 
SymbolTable addDefaultLibrary(){
	Level baseFunctionLevel("base", Level::Assoc::RIGHT);
	baseFunctionLevel.addToken(new PrintFunction);
	baseFunctionLevel.addToken(new PrintExpressionFunction);
	baseFunctionLevel.addToken(new ReturnFunction);
	baseFunctionLevel.addToken(new DeleteFunction);
	baseFunctionLevel.addToken(new IfFunction);
	baseFunctionLevel.addToken(new WhileFunction);
	baseFunctionLevel.addToken(new SquareRootFunction);
	
	SymbolTable table(new Void, new Void);
	table.getLevel("default")->addToken(new SquareRootFunction);
	table.addLevel(baseFunctionLevel);
	
    return table;
}

//return tokenlist and operand level index (for custom functions)
SymbolTable getDefaultSymbolTable(){
    /* define levels */
    Level baseFunctionLevel("base", Level::Assoc::RIGHT);
    Level setLevel("set", Level::Assoc::RIGHT);
    Level comparisionLevel("compare");
    Level plusMinusLevel("plusmin");
    Level multiplyDivideLevel("multdiv");
    Level exponentRootLevel("exp", Level::Assoc::RIGHT);
    Level creatorLevel("create", Level::Assoc::RIGHT);
    Level operandLevel("default");
	Level controlLevel("control");
    
    /* add builtins */
    setLevel.addToken(new SetOperator);

    comparisionLevel.addToken(new EqualityOperator);
    comparisionLevel.addToken(new InequalityOperator);
    comparisionLevel.addToken(new SmallerOperator);
	comparisionLevel.addToken(new BiggerOperator);
    
    plusMinusLevel.addToken(new AddOperator);
    plusMinusLevel.addToken(new SubtractOperator);

    multiplyDivideLevel.addToken(new MultiplyOperator);
    multiplyDivideLevel.addToken(new DivideOperator);

    exponentRootLevel.addToken(new ExponentOperator);
    
    operandLevel.addToken(new FloatingPoint);
    operandLevel.addToken(new Integer);
    creatorLevel.addToken(new FunctionCreator("default"));
    creatorLevel.addToken(new VariableCreator("default"));
    
    controlLevel.addToken(new BracketsOperator);
    controlLevel.addToken(new ParenthesesOperator);
	
    /* add levels */
    SymbolTable table(new Void, new Void);
    
	table.addEndLevel(controlLevel);
    table.addLevel(baseFunctionLevel);
    SymbolTable::iterator compit = table.addLevel(comparisionLevel);
    table.addLevel(setLevel, compit);
    table.addLevel(plusMinusLevel);
    table.addLevel(multiplyDivideLevel);
    table.addLevel(exponentRootLevel);
	SymbolTable::iterator opit = table.addLevel(operandLevel);
    table.addLevel(creatorLevel, opit);
    
    /* add default library */
    SymbolTable deflib = addDefaultLibrary();
	table.merge(deflib);
    
    return table;
}
    
/*
 * HELPER FUNCTIONS
 */

// call this to report a warning
void warning(std::string msg){
    std::cerr << "Warning: " <<  msg << std::endl;
}

// call this to report normal errors
void error(std::string msg){
    std::cerr << "Error: " <<  msg << std::endl;
    std::cerr << "Type '" << prog_name << "' -h' or '" << prog_name << "' --help' for more information." << std::endl;
    exit(EXIT_FAILURE);
}
// call this to report critical errors (that should not happen!)
void critical_error(std::string msg){
    std::cerr << "Critical program error: " <<  msg << std::endl;
    std::cerr << "The program encountered a condition that should never be reached." << std::endl;
    std::cerr << "Please report this bug to <kswolters@live.nl>." << std::endl;
    abort();
}

/*
 *  HELPER MODES
 */
//usage message
void usage(){
    std::cout << "Usage:\t" << prog_name << " [OPTIONS...] [-i]" << std::endl;
    std::cout << "\t" << prog_name << " [OPTIONS...] [-f] FILE" << std::endl;
    std::cout << "\t" << prog_name << " [OPTIONS...] -c EXPRESSION..." << std::endl;
    std::cout << std::endl;
    std::cout << "The program has three modes (interactive-, file- and command-mode)." << std::endl;
    std::cout << "All options given in brackets are optional." << std::endl;
    std::cout << std::endl;
    std::cout << "OPTIONS could be one of the following:" << std::endl;
    std::cout << "\t-h, --help" << "\tshow this help" << std::endl;
    std::cout << "\t-v, --version" << "\tshow the current version (together with the" << std::endl;
    std::cout <<" \t\t\tcommand build-parameters and the license)" << std::endl;
    std::cout << std::endl;
    std::cout << "This program is an interpreter for files written in the kin language," << std::endl;
    std::cout << "a programming language build to show how a language could be created." << std::endl;
    std::cout << "This program is therefore mainly build for educational use." << std::endl;
    std::cout << "The language support basic mathematical operations, variables and a few functions." << std::endl;
    std::cout << std::endl;
    std::cout << "For more information and documentation on the language, check: <WEBSITE>" << std::endl;
    std::cout << "For the license, run: " << prog_name << " -v" << std::endl;
}

//version message
void version(){
    std::cout << "|Kintex, an expression evaluator|" << std::endl;
    std::cout << "|Copyright (c) " << YEAR << " Koen Wolters (license: GPL)|" << std::endl;
#ifndef _WIN32    
	std::cout << "|Using version " << VERSION << " build with GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << " on " <<  __DATE__ << " " << __TIME__ << "|" << std::endl; 
#else
	std::cout << "|Using version " << VERSION << " build with GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << " (on Windows)|" << std::endl; 
#endif
}

void help(){
    std::cout << "Welcome to Kintex (version " << VERSION << ")! This is the builtin help utility." << std::endl;
    std::cout << std::endl;
    std::cout << "At the moment there is not much documentation for Kintex, this will be written later. Please check again then!" << std::endl;
    std::cout << "You can still just begin experimenting, try to type some common mathematical expressions (like 2*2+4) and check the result." << std::endl;
    std::cout << std::endl;
    std::cout << "Currently the following (important) functions are implemented:" << std::endl;
    std::cout << "\thelp\tshow this help (only in interactive mode)" << std::endl;
    std::cout << "\tversion\tshow version information (only in interactive mode)" << std::endl;
    std::cout << "\tprint\tprint result of expression" << std::endl;
    std::cout << "\tshow\tshow expression (the way it has been read)" << std::endl;
    std::cout << std::endl;
}

/*
 *  MODES
 */ 
//interactive mode
bool interactive_mode(){
    //welcome message
    version();
    std::cout << "|Type 'help' for more information or type 'quit' to stop|" << std::endl;
    
    //build token list (with custom functions)
    SymbolTable tokenList = getDefaultSymbolTable();
    SymbolTable interactive = addInteractiveLibrary();
	tokenList.merge(interactive);
    
	//set default StatementGroup
	DefaultStatementGroup *sg = new DefaultStatementGroup;
	
    //build new interpreter
    Interpreter kintex(tokenList, sg); 
    Environment env(tokenList);
	
    //loop until EOF
    quitInteractive = false;
    while(!std::cin.eof() && !quitInteractive){
        try{
            //show prompt
            std::cout << ">>> ";
            
            //parse stdin
            Interpreter::StatementList results = kintex.parseLine(std::cin);
            
            //work with eof and add endline
            if(std::cin.eof()) std::cout << std::endl;
            
            //print all results
            for(Interpreter::StatementList::iterator iter = results.begin(); iter < results.end(); iter++){
                //execute expression
                Expression result = iter->result(env);
                //print result if not void
                if(typeid(*result) != typeid(Void)) std::cout << *iter << " = " << result << std::endl;
            }
            //bracket syntax error (catched before processing)
        }catch(ScriptError &e){
            std::cerr << "ScriptError: error while reading script!" << std::endl;
            std::cerr << "\t" << e.what() << std::endl;
            //syntax error
        }catch(SyntaxError &e){
            std::cerr << "SyntaxError: error while parsing expression!" << std::endl;
            std::cerr << "\t" << e.what() << std::endl;
            std::cerr << "\t" << e.getTokenString() << std::endl;
            std::cerr << "\t" << e.getErrorPointerString() << std::endl;
            //execution error
        }catch(ExecError &e){
            std::cerr << "ExecError: error while calculating answer!" << std::endl;
            std::cerr << "\t" << e.what() << std::endl;
            std::cerr << "\t" << e.getTokenString() << std::endl;
            std::cerr << "\t" << e.getErrorPointerString() << std::endl;
            //unrecoverable error    
        }catch(std::exception &e){
            critical_error(e.what());
        }
    }
    delete sg;
    
    return true;
}

//file mode
bool file_mode(std::ifstream &file, std::string file_name){
	//set default StatementGroup
	DefaultStatementGroup *sg = new DefaultStatementGroup;
	
    //build new interpreter
    SymbolTable tokenList = getDefaultSymbolTable();
    Interpreter kintex(tokenList, sg);
	Environment env(tokenList);
    
    //parse file
    try{
        //set file stream to not skip spaces and newlines
        file.unsetf(std::ios::skipws);
        
        //build iterators
        Interpreter::StatementList results = kintex.parse(file, file_name);
        
        //print all results
        for(Interpreter::StatementList::iterator iter = results.begin(); iter < results.end(); iter++){
            //execute expression
            Expression result = iter->result(env);
        }
    }catch(ScriptError &e){
        std::cerr << "ScriptError: error while reading script!" << std::endl;
        std::cerr << "\t" << e.what() << std::endl;
        return false;
        //syntax error
    }catch(SyntaxError &e){
        std::cerr << "SyntaxError: error while parsing expression!" << std::endl;
        std::cerr << "\t" << e.what() << std::endl;
        std::cerr << "\t" << e.getTokenString() << std::endl;
        std::cerr << "\t" << e.getErrorPointerString() << std::endl;
        return false;
        //execution error
    }catch(ExecError &e){
        std::cerr << "ExecError: error while calculating answer!" << std::endl;
        std::cerr << "\t" << e.what() << std::endl;
        std::cerr << "\t" << e.getTokenString() << std::endl;
        std::cerr << "\t" << e.getErrorPointerString() << std::endl;
        return false;
        //unrecoverable error    
    }catch(std::exception &e){
        critical_error(e.what());
    }
    delete sg;
    
    return true;
}

//direct mode
bool direct_mode(std::string parseString){
	//set default StatementGroup
	DefaultStatementGroup *sg = new DefaultStatementGroup;
	
    //build new interpreter
    SymbolTable tokenList = getDefaultSymbolTable();
    Interpreter kintex(tokenList, sg);
	Environment env(tokenList);
    
    //parse file
    try{
        Interpreter::StatementList results = kintex.parse(parseString);
        
        //print all results
        for(Interpreter::StatementList::iterator iter = results.begin(); iter < results.end(); iter++){
            //execute expression
            Expression result = iter->result(env);
            //print result (if not void)
            if(typeid(*result) != typeid(Void)) std::cout << result << std::endl;
        }
    }catch(ScriptError &e){
        std::cerr << "ScriptError: " << e.what() << std::endl;
        return false;
        //syntax error
    }catch(SyntaxError &e){
        std::cerr << "SyntaxError: " << e.what() << std::endl;
        return false;
        //execution error
    }catch(ExecError &e){
        std::cerr << "ExecError: " << e.what() << std::endl;
        return false;
        //unrecoverable error    
    }catch(std::exception &e){
        critical_error(e.what());
    }
    delete sg;
    
    return true;
}

/*
 * MAIN PROGRAM
 */

int main(int argc, char **argv){
    //default mode is interactive
    Mode mode = INTERACTIVE_MODE;
    
    //open the argument reader
    GetOpt::GetOpt_pp args(argc, argv);
    
    //set program name
    prog_name = args.app_name();
    
    //parse all flags and options
    if(args >> GetOpt::OptionPresent('h', "help")){
        //check if exclusive
        if(args.options_remain()) warning("Help option should be exclusive!");
        //run usage function
        usage();
        return EXIT_SUCCESS;
    }
    if(args >> GetOpt::OptionPresent('v', "version")){
        //check if exclusive
        if(args.options_remain()) warning("Version option should be exclusive!");
        //run version function
        version();
        return EXIT_SUCCESS;
    }
    
    //get global options
    std::vector<std::string> globals;
    args >> GetOpt::GlobalOption(globals);
    
    //define fstream and string to possibly save info
    std::ifstream file;
    std::string input;
    
    //parse modes
    if(args >> GetOpt::OptionPresent('i', "interactive")){
        //check if only one mode is present
        if(args >> GetOpt::OptionPresent('c', "command") || args >> GetOpt::OptionPresent('f', "file")) error("Only one mode can be specified!");
        
        //set interactive mode
        mode = INTERACTIVE_MODE;
    }else if(args >> GetOpt::OptionPresent('c', "command")){
        //check if only one mode is present
        if(args >> GetOpt::OptionPresent('f', "file")) error("Only one mode can be specified!");
        
        //pass globals to input
        for(size_t i=0; i<globals.size(); ++i){
            input += globals[i];
            input += " ";
        }
    
        //set direct mode
        mode = DIRECT_MODE;
    }else if(globals.size() == 1){
        //ignore possible flag
        args >> GetOpt::OptionPresent('f', "file");
        
        //try to open file
        if(!globals.empty()) file.open(globals[0]);
        
        //check if file is found
        if(!file) error("File not found!");
        
        //set file mode
        mode = FILE_MODE;  
    }
    
    //check if options remain
    if (args.options_remain()) error("Unknown options left!");
    
    //run correct mode
    bool ret;
    switch(mode){
        case DIRECT_MODE:
            //enter direct parse mode
            ret = direct_mode(input);
            break;
        case FILE_MODE:
            //enter file mode
            ret = file_mode(file, globals[0]);
            break; 
        case INTERACTIVE_MODE:
            //entering interactive mode
            ret = interactive_mode();
            break;
    }
    if(ret) return EXIT_SUCCESS;
    else return EXIT_FAILURE;
}
