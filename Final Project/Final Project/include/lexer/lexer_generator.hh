#ifndef LEXER_GENERATOR_HH
#define LEXER_GENERATOR_HH 1

#include <string>

namespace final_project
{
    namespace lexer
    {
        //Reads in a set of regular expressions from the specified file 
        //and generates a lexer that can split a string into tokens based 
        //on the regular expressions.
        //
        //Creates two files for the lexer: lexer.hh which is the header file 
        //for the lexer and lexer.cpp which is the implementation of the lexer
        //
        //@param filename the name of the file containing the regular expressions
        void generate_lexer(const std::string& filename);
    } // namespace lexer
       
} // namespace final_project::lexer


#endif