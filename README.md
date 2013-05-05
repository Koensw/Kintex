KINTEX 
======

VERSIE: 2.0

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

Kintex is a project for me to learn programming more advanced things and to learn something about compiler/interpreter logic.

DOCUMENTATION
-------------

Documentation is not yet written!

DEVELOPMENT AND KNOWN ISSUES
----------------------------
Read docs/plannen.doc

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
