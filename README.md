KINTEX 
======

VERSION: 2.1  
LICENSE: MIT

(c) 2013 Koen Wolters

INTRODUCTION
------------

Kintex is a simple interpreter written in C++. It has the following features (at the moment):
* Basic operations like addition, subtraction etc.
* Floating-Point support
* Variables
* Functions
* Parentheses and brackets
* If and while statements
* Command-line interface
* Support for informative errors
* Few builtin functions

Kintex was a project for me to learn programming more advanced things and to learn something about compiler/interpreter logic.

DOCUMENTATION
-------------

Documentation is not yet written!

INSTALL
-------

###Linux
Build requirements:
- GCC (version 4.6 or higher) 
- make
- sed
- binutils/coreutils
- GMP

Commands to install:
- Execute 'make'
- Optional: execute 'make test' (to run tests)
- Execute 'sudo make install'
- Run 'kintex'

###Windows
Build requirements:
- mingw (with GCC version 4.6 or higher)
- sed
- GMP (CURRENTLY AN ISSUE!)

Commands to install:
- Execute 'mingw32-make'
- Run bin/kintex.exe
