#include "lexer/lexer_generator.hh"
#include "exception/exceptions.hh"
#include "automata/regex_parser.hh"
#include "automata/nfa.hh"
#include "automata/dfa.hh"

#include <fstream>
#include <iostream>

namespace final_project
{
    namespace lexer
    {
        //Fills in the lexer_skeleton.hh file with enum types based on the tokens 
        //the lexer will create. 
        //
        //@param skeleton_hh_in a file stream connected to the lexer skeleton header file
        //@param lexer_hh_out a file stream connected to the lexer header file
        void generate_hh(std::ifstream& skeleton_hh_in, std::ofstream& lexer_hh_out, 
            const std::unordered_map<automata::state_t, std::string>& labels)
        {
            std::string line;
            std::set<std::string> tokens;
            std::transform(labels.begin(), labels.end(), std::inserter(tokens, tokens.end()), 
                [](const std::pair<automata::state_t, std::string>& p)
                {
                    return p.second;
                });
            bool in_enum = false;
            while(getline(skeleton_hh_in, line))
            {
                //Remove skeleton in include guards
                if (line.find("SKELETON") != line.npos)
                {
                    line.erase(14, 9);
                    lexer_hh_out << line << "\n";
                }
                //Create enum
                //Found enum declaration
                else if (line.find("enum") != line.npos)
                {
                    lexer_hh_out << line << "\n";
                    in_enum = true;
                }
                //Writing enum values
                else if (in_enum)
                {
                    for(auto it = tokens.begin(); it != tokens.end(); ++it)
                        lexer_hh_out << "     tl_" << (*it) << ",\n"; //tl for Turing lexer in reference to Alan Turing 
                    lexer_hh_out << "     tl_EOF,\n     tl_ERROR\n";
                    in_enum = false;
                }
                //Close enum
                else if (in_enum && line.find("}") != line.npos)
                {
                    in_enum = false;
                    lexer_hh_out << line << "\n";
                }
                //Nothing special to do
                else 
                {
                    lexer_hh_out << line << "\n";
                }
            }
        }

        //Generate code to represent DFA table. Converts the DFA table into goto statements 
        //in the code.
        void print_dfa_table(std::ostream& lexer_cpp_out, const automata::dfa<char>& table)
        {
            std::unordered_map<automata::state_t, std::string> accepting_labels = table.get_accepting_labels();
            const auto& transitions = table.get_table();
            const auto& accepting_states = table.get_accepting_states();
            for(size_t i = 0; i < transitions.size(); ++i)
            {
                //Print state name
                lexer_cpp_out << "\n     tl" << i << ":";
                lexer_cpp_out << "\n     {";
                lexer_cpp_out << "\n          char c = next_character();";
                if (i == 0)
                {
                    lexer_cpp_out << "    if(isspace(c))";
                    lexer_cpp_out << "\n  {";
                    lexer_cpp_out << "\n        advance();";
                    lexer_cpp_out << "\n        goto tl0;";
                    lexer_cpp_out << "\n   }";
                }
                //Check if we have seen a space that endicates the end of this token 
                //If so, we need to either make a token instaed of looking for more characters
                else if (std::find(accepting_states.begin(), accepting_states.end(), static_cast<automata::state_t>(i)) != accepting_states.end())
                {
                    lexer_cpp_out << "\n            if(isspace(c))";
                    lexer_cpp_out << "\n                  return make_token(token_type::tl_" << accepting_labels.find(i)->second << ", value);";
                }
                else
                {
                    lexer_cpp_out << "\n          if(isspace(c))";
                    lexer_cpp_out << "\n           {";
                    lexer_cpp_out << "\n                   value += c;";
                    lexer_cpp_out << "\n                   advance();";
                    lexer_cpp_out << "\n                   return make_token(token_type::tl_ERROR , value);";
                    lexer_cpp_out << "\n           }";
                }
                //Get transition transitions form current character
                const auto& row = transitions[i];
                //Print goto statements for transition
                for(auto it = row.begin(); it != row.end(); ++it)
                {
                    if(it->second == automata::ACCEPT) //Handled below
                        continue;
                    if (it == row.begin())
                    {
                        if (it->first == automata::EPSILON)
                            lexer_cpp_out << "\n         if(!c)";
                        else
                            lexer_cpp_out << "\n         if(c == '" << (it->first == '\\' ? "\\" : "" ) << it->first << "')";
                    }
                    else
                    {
                       if (it->first == automata::EPSILON)
                            lexer_cpp_out << "\n         else if(!c)";
                        else
                            lexer_cpp_out << "\n         else if(c == '" << (it->first == '\\' ? "\\" : "" ) << it->first << "')";
                    }
                    lexer_cpp_out << "\n             {";
                    lexer_cpp_out << "\n                  value += c;";
                    lexer_cpp_out << "\n                  advance();";
                    lexer_cpp_out << "\n                  goto tl" << it->second << ";";      
                    lexer_cpp_out << "\n             }";                        
                }
                 //Print accept action
                if (std::find(accepting_states.begin(), accepting_states.end(), static_cast<automata::state_t>(i)) != accepting_states.end())
                {
                    if(row.size() > 1)
                        lexer_cpp_out << "\n            else";
                    lexer_cpp_out << "\n                  return make_token(token_type::tl_" << accepting_labels.find(i)->second << ", value);";
                }
                else
                {
                    lexer_cpp_out << "\n           else";
                    lexer_cpp_out << "\n           {";
                    lexer_cpp_out << "\n                   value += c;";
                    lexer_cpp_out << "\n                   advance();";
                    lexer_cpp_out << "\n                   return make_token(token_type::tl_ERROR , value);";
                    lexer_cpp_out << "\n           }";
                }
                lexer_cpp_out << "\n     }";
            } 
            lexer_cpp_out << "\n     return make_token(token_type::tl_ERROR, value);";
        }

        void generate_lexer_cpp(std::ifstream& skeleton_cpp_in, std::ofstream& lexer_cpp_out, const automata::dfa<char>& table)
        {
            std::string line;
            bool in_next_token = false;
            while(getline(skeleton_cpp_in, line))
            {
                if(line.find("lexer_skeleton.hh") != std::string::npos)
                {
                    lexer_cpp_out << "#include \"lexer.hh\"" << "\n";
                    continue;
                }
                if (line.find("lexer::next_token") != std::string::npos)
                {
                    in_next_token = true;
                    lexer_cpp_out << line << "\n";
                }
                else if (in_next_token && line.find("}") != std::string::npos)
                {
                    in_next_token = false;
                    lexer_cpp_out << "}\n";
                    continue;
                }
                else if (in_next_token)
                {
                    lexer_cpp_out << "     std::string value = \"\";";
                    print_dfa_table(lexer_cpp_out, table);
                }
                else 
                {
                    lexer_cpp_out << line << "\n";
                }
            }
        }

        void generate_lexer(const std::string& filename)
        {
            std::ifstream fin(filename.c_str());
            if(!fin.is_open())
                throw exceptions::file_not_found_exception("Could not open file");
            //Convert file with regular expressions to postfix notation
            regex::regex_parser parser(fin);
            auto parsed = parser.parse();
            //Create NFA
            automata::nfa n = automata::build_nfa(parsed);
            //Create DFA 
            automata::dfa<char> d = automata::powerset_construction(n);
            //File streams connected to skeletons 
            std::ifstream skeleton_hh_in("lexer_skeleton.hh");
            std::ifstream skeleton_cpp_in("lexer_skeleton.cpp");
            //File strems to create .cpp and .hh file
            std::ofstream lexer_hh_out("lexer.hh");
            std::ofstream lexer_cpp_out("lexer.cpp");

            //Create .hh file
            generate_hh(skeleton_hh_in, lexer_hh_out, d.get_accepting_labels());
            generate_lexer_cpp(skeleton_cpp_in, lexer_cpp_out, d);
            //Close file streams
            skeleton_hh_in.close();
            skeleton_cpp_in.close();
            lexer_hh_out.close();
            lexer_cpp_out.close();
        }
    } // namespace lexer
    
} // namespace final_project
