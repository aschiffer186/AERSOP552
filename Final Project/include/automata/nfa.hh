#ifndef NFA_HH
#define NFA_HH 1

#include <vector>
#include <unordered_map>
#include <ostream>
#include <set>
#include <limits>

#define NFA_TRANSITION(c, ...) {c, {__VA_ARGS__}}

namespace final_project
{
    namespace automata
    {
        //A state in the NFA
        typedef int state_t;
        //The epsilon character
        static const char EPSILON = '\0';
        //Accept action
        static const state_t ACCEPT = std::numeric_limits<state_t>::max();

        //A simple struct to represent a non-deterministic finite automatum (NFA).
        //This struct contaions information about the NFA's accepting states and 
        //state transition table. 
        struct nfa
        {
            //The type of the state transition table
            typedef std::vector<std::unordered_map<char, std::vector<state_t>>> table_t;

            //Increments all states in the NFA by the specified number 
            //
            //@param delta the amount to increment the states by
            void renumber(state_t delta);

            //The NFA's alphabet
            std::set<char> _M_alphabet;
            //The NFA's accepting states
            std::vector<state_t> _M_accepting_states;
            //The names of the tokens to be produced in each accepting state
            std::unordered_map<state_t, std::string> _M_accepeting_state_labels;
            //The NFA's state transition table
            table_t _M_transitions;
        };

        inline void nfa::renumber(state_t delta)
        {
            for(auto& state: _M_accepting_states)
                state += delta;
            std::unordered_map<state_t, std::string> temp;
            for(auto it = _M_accepeting_state_labels.begin(); it != _M_accepeting_state_labels.end(); ++it)
            {
                auto pair = std::make_pair(it->first + delta, it->second);
                temp.insert(pair);
            }
            _M_accepeting_state_labels = temp;
            for(auto& row: _M_transitions)
            {
                for(auto& transition: row)
                {
                    for(auto& state: transition.second)
                    {
                        if(state != ACCEPT) 
                            state += delta;
                    }
                }
            }
        }

        //Constructs an NFA using Thompson's construction that can recognize the
        //language defined by the union of the specified regular expressions. 
        //The regular expressions must be in postfix notation.
        //
        //@param regex the regular expressions to create an NFA for 
        //@return an NFA that can be used to recognize the regular expressions
        nfa build_nfa(const std::vector<std::pair<std::string, std::vector<char>>>& regex);
    }
}

 std::ostream& operator<<(std::ostream& os, const final_project::automata::nfa& n);

#endif