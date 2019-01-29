#C Shell 

Implemented a basic shell using C and system calls

#Instructions for running:

First compile using the Makefile and then run the executable:
---
make
./cshell


#Features Implemented

- Supports basic shell builtins like cd,pwd,echo
- Also supports ls (as a builtin ) with some options
- Pinfo for information about processes
- Error handling
- Exit Shell by typing exit
- Semi-colon separated list of commands
- Supports background processes


#File Information
- cshell.h      : Header file with all declarations
- builtinFuncsc : Definition of functions used for implementing pwd,cd,echo
- ls.c          : Definition of ls function with all options
- pinfo.c       : Definition of pinfo command
- readSplit.c   : Definitions for tokenizing and reading input
- execute.c     : For handling the execution
- launch        : For commands which are not builtins
- run.c         : Contains the main part to start the shell and print 

# Authors 
- Mohit Sharma(20161031)





