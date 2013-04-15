#include <vector>
#include <cstdlib>
#include <cstddef>

/* WARNING: all includes from the library are temporarily done by interactive_library */
#include "interactive_library.h"

using namespace kintex;

/*
 *  Return the version information
 *  Returns: void
 */
Value VersionFunction::operator()(std::vector<Expression> values){
    version();
    return Void(getParent());
}

/*
 *  Return the help information (FIXME: extensive help)
 *  Returns: void
 */
Value HelpFunction::operator()(std::vector<Expression> values){
    help();
    return Void(getParent());
}

/*
 *  Quits interactive mode
 *  Returns: void
 */
Value QuitFunction::operator()(std::vector<Expression> values){
    //set quit app variable
    quitInteractive = true;
    return Void(getParent());
}