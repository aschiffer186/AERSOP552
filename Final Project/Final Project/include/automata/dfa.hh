#ifndef DFA_HH
#define DFA_HH 1

#include <vector>
#include <unordered_map>
#include <ostream>
#include <algorithm>

#include "nfa.hh"

namespace final_project
{
    namespace automata
    {
        //A struct to represent an DFA
        template<typename _TokTp>
        struct dfa
        {
            typedef std::vector<std::unordered_map<_TokTp, state_t>> table_t;

            public:
                dfa(const std::vector<state_t>& accepting_states, const std::unordered_map<state_t, std::string>& labels,
                   const table_t& transitions)
                    : _M_accepting_states(accepting_states), _M_accepting_labels(labels), _M_transitions(transitions)
                {

                }

                dfa(const std::vector<state_t>& accepting_states,
                   const table_t& transitions)
                    : _M_accepting_states(accepting_states), _M_accepting_labels(), _M_transitions(transitions)
                {

                }

                table_t& get_table()
                {
                    return _M_transitions;
                }

                const table_t& get_table() const 
                {
                    return _M_transitions;
                }

                std::vector<state_t>& get_accepting_states()
                {
                    return _M_accepting_states;
                }

                const std::vector<state_t>& get_accepting_states() const 
                {
                    return _M_accepting_states;
                }

                const std::unordered_map<state_t, std::string>& get_accepting_labels() const 
                {
                    return _M_accepting_labels;
                }

            private:
                std::vector<state_t> _M_accepting_states;
                std::unordered_map<state_t, std::string> _M_accepting_labels;
                table_t _M_transitions;
        };

        //Construct a DFA from the specified NFA using the powerset 
        //construction technique. If the NFA has N states, the 
        //returned DFA may have up to 2^N states. 
        //
        //@param n the NFA to construct from
        //@return a DFA equiavalent to the NFA
        dfa<char> powerset_construction(const nfa& n);

        //Minimizes the specified DFA.
        //
        //@param d the DFA to minimize
        template<typename _Tp>
        void minimize_dfa(dfa<_Tp>& d)
        {
            auto& transitions = d._M_transitions;
            auto& accepting = d.get_accepting_states;

            //Group states in accepting and non-accepting states
            std::vector<state_t> non_accepting;
            for(size_t i = 0; i < transitions.size(); i++)
            {

            }
        }
    } // namespace automata
    
} // namespace final_project

template<typename _Tp>
inline std::ostream& operator<<(std::ostream& os, const final_project::automata::dfa<_Tp>& d)
{   
    auto accepting_states = d.get_accepting_states();
    if(accepting_states.size() > 0) {
        os << "Accepting states: ";
        for(size_t i = 0; i < accepting_states.size() - 1; ++i)
            os << accepting_states[i] << ", ";
        os << accepting_states.back();
    }
    auto transitions = d.get_table();
    if (d.get_accepting_labels().size() > 0) {
        os << "\nAccepting labels: {\n";
        for(const auto& label: d.get_accepting_labels())
        {
            os << "{" << label.first << ": " << label.second << "}";
        }
        os << "}";
    }
    os << "\nTransition table: \n";
    for(size_t i = 0; i < transitions.size(); ++i)
    {
        os << "State " << i << ":";
        const auto& row = transitions[i];
        for(const auto& transition: row)
        {
            os << "\n\t" << transition.first << ": go to state " << transition.second;
        }
        if (std::find(accepting_states.begin(), accepting_states.end(), static_cast<final_project::automata::state_t>(i)) != accepting_states.end())
        {
            os << "\n\tEOF accept"; 
        }
        os << "\n";
    }
    return os;
}

namespace std
{
    template<>
    struct hash<std::set<final_project::automata::state_t>>
    {
        size_t operator()(const std::set<final_project::automata::state_t>& s) const
        {
            std::hash<int> hasher;
            size_t seed = 0;
            for(const auto state: s)
            {
                seed ^= hasher(state) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            }
            return seed;
        }
    };
} // namespace std


#endif
