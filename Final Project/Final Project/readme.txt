This project has two components: a lexer generator and a parse table generator. 

To make the project using CMake, open up the terminal and type the following commands:
    mkdir build 
    cd build 
    cmake .. 
    make install

The lexer generator takes an input file consisting of one or more regular expressions. The regular expressions should be
of the form:
    label: regular expression

The lexer generator can handle a limited subset of regular expresions. It supports the following opertors: 
    1) ab - concatentation 
    2) a|b - alternation 
    3) a* - the Kleene closure
Internally, concatention is represented using the ? operator. To use *, ?, |, and \ in your regular expresion, you must 
escape them using \, e.g. to include * in the regular expression, you must write \*. The lexer generator treates $ as a 
sepcial character; it represents an empty string. To include '$' in your regular expression, you must escape it. 

The lexer generator creates two files representing the lexer: lexer.hh containing the lexer header and lexer.cpp containing 
the implementation of the lexer. To use the lexer, there is a file called test_lexer.cpp. You can compile the test_lexer.cpp 
using the command 
    g++ test_lexer.cpp lexer.cpp -o lexer.exe 

The test_lexer file is a simple file that simply runs the lexer in a loop on user supplied strings. 

The parse table generator takes a context free-grammar in the form 
    terminals: <list of terminals>
    lhs->rhs
The first line is the list of terminals in the grammar (it must start with "terminals:"). The following lines represented 
the grammar. One line of the grammar must contain a rule of the form "S->rhs;" is the goal symbol. 

The parse table generator will create a text file containing the LALR(1) parse tables. 