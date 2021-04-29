#ifndef REGEX_PARSER_H
#define REGEX_PARSER_H

#include <istream>
#include <regex>
#include <vector>

namespace final_project
{
    namespace regex 
    {
        //A class to convert a regular expression (regex) from 
        //infix notation to postfix notation to ease the 
        //creation of DFAs. This class can handle regular 
        //expressions that adhere to the following convention 
        //
        //  1. The alphabet of the regular expression contains ASCII characters.
        //  2. The regular expression only uses the alternation (|) and Kleene 
        //     closure (*) operators. 
        //  3. The regular expression only uses the character classes [0-9], [A-Z], 
        //     [a-z], and [A-Za-z].
        //
        //Given one or more regex, the class will produce a vector 
        //of the regular expressions in post-fix notation
        class regex_parser 
        {
            public:
                //Constructs a new regex_parser that reads 
                //from the specified input stream. 
                //
                //@param in the input stream to read from
                //@requires in be a valid input stream
                explicit regex_parser(std::istream& in) noexcept;

                //Reads all regex in the input stream attached 
                //to the class and converts all regex from 
                //infix notation to postfix notation using the 
                //Shunting Yard Algorithm. 
                //
                //Throws 
                //exceptions::invalid_regex_exception at the 
                //first regex that does not adhere to the required
                //syntax. 
                //
                //Throws <specifiy exception> if the input stream 
                //cannot be read from. 
                //
                //@returns a vector containing the regex in the input 
                //         stream in postfix notation 
                //@modifies the input stream attached to the class
                //@effects the input stream attached to the class 
                //         will be at the end of the text it is 
                //         attached to 
                std::vector<std::pair<std::string, std::vector<char>>> parse();
            private:
                //Preprocesses a single regular expression to make it 
                //easier to parse. Inserts a concatenation operator (?) 
                //where appropriate. Converts character classes into the 
                //unions of the characters they represent. 
                //
                //@param regex the regex to preprocess
                //@modified regex
                void preprocess(std::string& regex);

                //Converts a single regular expression from infix 
                //notation to postifx notation. Throws 
                //exceptions::invalid_regex_exception if the 
                //regex being parsed does not adhered to the required 
                //syntax.
                //
                //@param regex the regular expression to be parsed
                //@return a vector containing the regular expression 
                //        in postfix notation
                std::vector<char> parse_regex(std::string& regex);

                //Returns true if the specified character is the 
                //alternation, concatenation, or Kleene concatenation 
                //operators. 
                //
                //@param c the character to check 
                //@return true if it is an operator
                bool is_operator(char c) const;

                //Returns the precedence of the operator or 
                //-1 if the operator is not an operator. 
                //
                //@param op the operator to return the precednece of
                //@return the precedence of the operator
                int precedence(char op) const;
            private:
                //The input stream to read regex from 
                std::istream& _M_in;
        };

        inline bool regex_parser::is_operator(char c) const 
        { 
            return (c == '|') || (c == '?') || (c =='*');
        }

        inline int regex_parser::precedence(char c) const 
        {
            if (c == '*')
                return 3;
            else if (c == '?')
                return 2;
            else if (c == '|')
                return 1;
            else 
                return -1;
        }
    }
} // namespace final_project::regex


#endif