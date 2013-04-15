#ifndef KINTEX_PROGRAM_H
#define KINTEX_PROGRAM_H 

#include <string>
#include <fstream>

#include "core/level.h"

//boolean used by QuitFunction to terminate application (no memory leaks!)
extern bool quitInteractive;

//grammar functions
void addInteractiveLibrary(kintex::TokenList &tokenList);
void addDefaultLibrary(kintex::TokenList &tokenList);
kintex::TokenList getDefaultTokenList();

//helper functions
void critical_error(std::string msg);
void error(std::string msg);
void warning(std::string msg);

//helper modes
void usage();
void help();
void version();

//modes
bool direct_mode(std::string parseString);
bool file_mode(std::ifstream &file, std::string file_name);
bool interactive_mode();

//main
int main(int argc, char **argv);

#endif