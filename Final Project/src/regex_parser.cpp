#include "automata/regex_parser.hh"
#include "exception/exceptions.hh"

#include <stdexcept>
#include <stack>
#include <regex>
#include <iostream>

namespace final_project
{
    namespace regex 
    {
    regex_parser::regex_parser(std::istream& in) noexcept
        : _M_in(in)
    {

    }

    std::vector<std::pair<std::string, std::vector<char>>> regex_parser::parse()
    {
        if(!_M_in.good())
            throw std::range_error("Input stream is in invalid state.");
        std::string line;
        std::vector<std::pair<std::string, std::vector<char>>> regex;
        while(getline(_M_in, line))
        {   
            if(line[0] == '#')
                continue;
            size_t colon_index = line.find(":");
            std::string label = line.substr(0, colon_index);
            std::string r = line.substr(line.rfind(" ") + 1);
            regex.push_back(std::make_pair(label, parse_regex(r)));
        }
        return regex;
    }

    std::vector<char> regex_parser::parse_regex(std::string& regex)
    {
        try 
        {
        preprocess(regex);
        } catch (const std::regex_error& e) {
            auto error = e.code();
            switch(error)
            {
                case std::regex_constants::error_collate:
                    std::cout << "Collate" << std::endl;
                    break;
                case std::regex_constants::error_ctype:
                    std::cout << "ctype" << std::endl;
                    break;
                case std::regex_constants::error_escape:
                    std::cout << "escape" << std::endl;
                    break;
                case std::regex_constants::error_backref:
                    std::cout << "backref" << std::endl;
                    break;
                case std::regex_constants::error_brack:
                    std::cout << "brack" << std::endl;
                    break;
                case std::regex_constants::error_paren:
                    std::cout << "paren" << std::endl;
                    break;
                case std::regex_constants::error_brace:
                    std::cout << "brace" << std::endl;
                    break;
                 case std::regex_constants::error_badbrace:
                    std::cout << "badbrace" << std::endl;
                    break;
                 case std::regex_constants::error_range:
                    std::cout << "range" << std::endl;
                    break;
                 case std::regex_constants::error_space:
                    std::cout << "space" << std::endl;
                    break;
                 case std::regex_constants::error_complexity:
                    std::cout << "complexity" << std::endl;
                    break;
                 case std::regex_constants::error_stack:
                    std::cout << "stack" << std::endl;
                    break;
                default:
                    std::cout << "other" << std::endl;
                    break;
            }
        }
        std::vector<char> postfix;
        std::stack<char> op_stack;
        for(size_t i = 0; i < regex.size(); ++i)
        {
            char c = regex[i];
            if(!is_operator(c) && c != '(' && c != ')')
            {
                postfix.push_back(c);
                if(c == '\\')
                {
                    if (i == regex.length() - 1 
                        && regex[i + 1] != '*' 
                        && regex[i + 1] != '|' 
                        && regex[i + 1] != '\\'
                        && regex[i + 1] != '?')
                        throw exceptions::invalid_regex_exception("Incomplate escape character sequence");
                    postfix.push_back(regex[++i]);
                }
            }
            else if(c == '(')
                op_stack.push(c);
            else if (c == ')')
            {
                while(!op_stack.empty() && op_stack.top() != '(')
                {
                    postfix.push_back(op_stack.top());
                    op_stack.pop();
                }
                op_stack.pop();
            }
            else 
            {
                while(!op_stack.empty() && precedence(c) <= precedence(op_stack.top()))
                {
                    postfix.push_back(op_stack.top());
                    op_stack.pop();
                }
                op_stack.push(c);
            }
        }
        while(!op_stack.empty())
        {
            postfix.push_back(op_stack.top());
            op_stack.pop();
        }
        return postfix;
    }

    void regex_parser::preprocess(std::string& regex)
    {
        std::string all_replaced = regex;
        //Add concatenation operator
        //Cases where concatentation operator should be inserted 
        //  1. Between two tokens e.g. ab -> a?b
        //  2. Between token and ( e.g. a(ab) -> a?(a?b)
        //  3. Between * and token e.g. a*b -> a*?b 
        //  4. Between * and ( e.g a*(ab) -> a*?(a?b)
        //  5. Between ) and token e.g. a(ab)c -> a?(a?b)?c
        //  6. Between ) and ( e.g. a(ab)(c)->a?(a?b)?(c)
        for(size_t i = 0; i < all_replaced.length() -1; ++i)
        {
            if(all_replaced[i] == ')')
            {
                if(all_replaced[i+1] == '(' || (!is_operator(all_replaced[i+1]) && all_replaced[i+1] != ')'))
                {
                    all_replaced.insert(i+1,"?");
                    ++i;
                }
            }
            else if (all_replaced[i] == '\\')
            {
                ++i;
                if (i == all_replaced.length() - 1)
                    break;
                if(all_replaced[i+1] == '(' || (!is_operator(all_replaced[i+1]) && all_replaced[i+1] != ')'))
                {
                    all_replaced.insert(i+1,"?");
                    ++i;
                }
            }
            else if(all_replaced[i] == '*')
            {
                if(all_replaced[i+1] == '*') //Expression of form a** which isn't allowed
                    throw exceptions::invalid_regex_exception("Unexpcted token: *");
                if(all_replaced[i+1] == '(' || (!is_operator(all_replaced[i+1]) && all_replaced[i+1] != ')'))
                {
                    all_replaced.insert(i+1,"?");
                    ++i;
                }
            }
            else if(all_replaced[i] != '|' && all_replaced[i] != '(')
            {
                if(all_replaced[i+1] == '(' || (!is_operator(all_replaced[i+1]) && all_replaced[i+1] != ')'))
                {
                    all_replaced.insert(i+1,"?");
                    ++i;
                }
            }
        }
        regex = all_replaced;
    }
    }
} // namespace final_project::regex
