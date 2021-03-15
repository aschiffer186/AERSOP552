#include "automaton.h"
#include <fstream>
#include <iostream>

//Exception inmplementation 
//Modeled afterstd::exception
file_not_found_exception::file_not_found_exception(const std::string& message)
    : _M_message(message) {}

const char* file_not_found_exception::what() const noexcept 
{
    return _M_message.c_str();
}

invalid_dfa_exception::invalid_dfa_exception(const std::string& message)
    : _M_messsage(message) {}

const char* invalid_dfa_exception::what() const noexcept 
{
    return _M_messsage.c_str();
}

//DFA implementation
dfa::dfa(const std::string& file_name)
    : _M_alphabet(), _M_states(), _M_init(), _M_accepting(), _M_transitions(nullptr), _M_curr_state(0), 
      _M_num_states(0),  _M_cap(0)
    {
        read_file(file_name);
    }

dfa::~dfa() 
{
    if(_M_transitions)
        delete[] _M_transitions;
}

void dfa::read_file(const std::string& file)
{
    try 
    {
        std::fstream fin(file.c_str());
        if (!fin.is_open())
            throw file_not_found_exception("Error opening file " + file);
        std::string curr_property = "";
        std::string line;
        std::string init = "";
        std::string accepting = "";
        std::string transitions = "";
        size_t largest_state;
        while (getline(fin, line))
        {
            if (line.find("alphabet") != std::string::npos)
                curr_property = "alphabet";
            else if (line.find("states") != std::string::npos)
                curr_property = "states";
            else if (line.find("initial") != std::string::npos)
                curr_property = "initial";
            else if (line.find("final") != std::string::npos)
                curr_property = "final";
            else if (line.find("transitions") != std::string::npos)
                curr_property = "transitions";
            else 
            {
                if (curr_property == "alphabet")
                {
                    if(line.empty())
                        throw invalid_dfa_exception("Empty alphabet");
                    _M_alphabet = line;
                } 
                else if (curr_property == "states")
                {
                    if(_M_alphabet.empty())
                        throw invalid_dfa_exception("Alphabet of DFA not defined");
                    if(line.empty())
                        throw invalid_dfa_exception("No states defined");
                    _M_states = line;
                    size_t last_space = _M_states.rfind(" ");
                    largest_state = std::stoull(_M_states.substr(last_space + 1));
                    _M_num_states = largest_state;
                    _M_transitions = new std::string[_M_num_states];
                    //for(size_t i = 0; i < largest_state; ++i)
                        //_M_transitions[i] = "";
                }
                else if (curr_property == "initial")
                {
                    if (_M_states.empty())
                        throw invalid_dfa_exception("State of DFA not defined");
                    if (line.empty())
                        throw invalid_dfa_exception("No initial state defined");
                    
                    _M_init = line;
                    _M_curr_state = std::stoull(_M_init);
                }
                else if (curr_property == "final")
                {
                    if (_M_init.empty())
                        throw invalid_dfa_exception("Initial state of DFA not defined");
                    if(line.empty())
                        throw invalid_dfa_exception("No final state defined");
                    _M_accepting = line;
                }
                else 
                {
                    if (_M_accepting.empty())
                        throw invalid_dfa_exception("Accepting states of DFA not defined");
                    if (line.empty() && _M_transitions[0].empty())
                        throw invalid_dfa_exception("No transition defined");
                    std::string transition = read_transition(line);
                    size_t sep_index = transition.find("-");
                    size_t first_state = std::stoull(transition.substr(0, sep_index));
                    _M_transitions[first_state - 1].append(transition.substr(sep_index + 1));
                }
            }
        }
    } catch (...) {
        delete[] _M_transitions;
        throw;
    }
}

std::string dfa::read_transition(const std::string& line) const
{
    std::string transition;
    size_t first_sep_index = line.find("-", 0);
    transition = line.substr(0, first_sep_index - 1);
    transition += "-";
    size_t second_sep_index = line.find("-", first_sep_index + 3);
    transition += line.substr(first_sep_index + 3, (second_sep_index - (first_sep_index + 3) - 1));
    transition += "-";
    transition += line.substr(second_sep_index + 4);
    transition += "|";
    return transition;
}

bool dfa::is_accepting_state() const 
{
    for(size_t i = 0; i < _M_accepting.length(); ++i)
    {
        if(_M_accepting[i] != ' ')
        {
            std::string state = "";
            for(; _M_accepting[i] != ' ' && i < _M_accepting.length(); ++i)
                state += _M_accepting[i];
            if (std::stoull(state) == _M_curr_state)
                return true;
        }
    }
    return false;
}

void dfa::transition(char c)
{
    const std::string& t = _M_transitions[_M_curr_state - 1];
    for(size_t i = 0; i < t.length(); ++i)
    {
        size_t sep_index = t.find("|", i);
        if (t[i] == c)
        {
            _M_curr_state = std::stoull(t.substr(i + 2, (sep_index - (i + 2))));
            return;
        }
        i = sep_index + 1;
    }
}

bool dfa::is_valid(const std::string& text)
{
    for(size_t i = 0; i < text.length(); ++i)
    {
        char curr = text[i];
        transition(curr);
    }
    return is_accepting_state();
}

bool automaton(std::string file, std::string text)
{
    dfa d(file);
    return d.is_valid(text);
}

int main(int argc, char** argv)
{
    std::string file = "file.txt";
    if (argc >1 )
        file = argv[1];
    std::string text;
    while (true)
    {
        try 
        {
            #ifdef DEBUG
                text = "aaaa";
            #else
            std::cout << "Enter text or \"quit\" to quit: \n";
            getline(std::cin, text);
            if (text == "quit") break;
            #endif
            bool is_valid = automaton(file, text);
            std::cout << std::boolalpha << is_valid << std::endl;
            #ifdef DEBUG
                return 0;
            #endif
        } catch (const file_not_found_exception& ex)
        {   
            std::cout << ex.what() << std::endl;
            return EXIT_FAILURE;
        }
        catch (const invalid_dfa_exception& ex)
        {
            std::cout << ex.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}