#include "automata/dfa.hh"

#include <unordered_set>
#include <algorithm>
#include <deque>
#include <iostream>

void print_set(const std::set<final_project::automata::state_t>& set)
{
    for(auto s: set)
        std::cout << s << " ";
    std::cout << std::endl;
}

namespace final_project
{
    namespace automata
    {
        //Compute the epsilon closure of a state (all of the states that can be reached
        //with e-moves from the current state).
        std::set<state_t> epsilon_closure(state_t start_state, const nfa::table_t& transitions)
        {
            std::set<state_t> e_closure;
            std::deque<state_t> work_list;
            work_list.push_back(start_state);
            //Epsilon closure is essentially a BFS but only including epsilon transitions
            while(!work_list.empty())
            {
                state_t curr = work_list.front();
                work_list.pop_front();
                e_closure.insert(curr);

                const auto& row = transitions[curr];
                //Search all states that can be reached from this state 
                for(const auto& transition: row)
                {
                    //Add all states that can be reached with an epsilon transition
                    if (transition.first == EPSILON && std::find(transition.second.begin(), transition.second.end(), ACCEPT) == transition.second.end())
                    {
                        for(auto state: transition.second) //Thompson's construction won't create a cycle that we can't get out of
                            work_list.push_back(state);
                    }
                }
            }
            return e_closure;
        }

        dfa<char> powerset_construction(const nfa& n)
        {
           const auto& nfa_table = n._M_transitions;
           const auto& alphabet = n._M_alphabet;
           auto nfa_accepting_states = n._M_accepting_states;
           //Each set of states in the NFA will become a state in the DFA 

           //Compute set of states reachable with epsilon transition from 
           //NFA start state
           std::set<state_t> q0 = epsilon_closure(0, nfa_table);
           std::vector<std::set<state_t>> Q = {q0};
           std::deque<std::set<state_t>> work_list = {q0};
           std::unordered_map<std::set<state_t>, std::unordered_map<char, std::set<state_t>>> T;

           while(!work_list.empty())
           {
                auto q = work_list.front();
                work_list.pop_front();
                //Loop over each character in the NFA's alphabet
                for(auto c: alphabet)
                {
                    std::set<state_t> t;
                    //Get the states that can be reached from the current state 
                    //upon seeing an input of c 
                    for(auto state: q)
                    {
                        const auto& row = nfa_table[state];
                        auto it = row.find(c);
                        if (it != row.end())
                        {
                            auto reachable_states = it->second;
                            for(auto reachable_state : reachable_states)
                            {
                                if(reachable_state == ACCEPT)
                                    continue;
                                auto closure = epsilon_closure(reachable_state, nfa_table);
                                for(auto s: closure)
                                {
                                    t.insert(s);
                                }
                            }
                        }
                    }
                    if(!t.empty())
                    {
                        T[q][c] = t;
                        auto it2 = std::find_if(Q.begin(), Q.end(), [&](const std::set<state_t>& curr)
                        {
                            return std::equal(curr.begin(), curr.end(), t.begin());
                        });
                        if(it2 == Q.end())
                        {
                            Q.push_back(t);
                            work_list.push_back(t);
                        }
                    }
                }
           }

            //USe Q and T to make DFA table
            std::vector<state_t> dfa_accepting_states;
            std::unordered_map<state_t, std::string> dfa_accepting_labels;
            dfa<char>::table_t dfa_table;
            for(size_t i = 0; i < Q.size(); ++i)
            {
                auto curr_state = Q[i];
                auto it = std::find_first_of(nfa_accepting_states.begin(), nfa_accepting_states.end(), curr_state.begin(), curr_state.end());
                if (it != nfa_accepting_states.end())
                {
                    dfa_accepting_states.push_back(i);
                    auto it2 = n._M_accepeting_state_labels.find(*it);
                    if (it2 != n._M_accepeting_state_labels.end())
                        dfa_accepting_labels[i] = it2->second;
                }
                std::unordered_map<char, state_t> dfa_row;
                auto T_row = T[curr_state];
                for(auto pair: T_row)
                {
                    auto state_set = pair.second;
                    auto it2 = std::find_if(Q.begin(), Q.end(), [&](const std::set<state_t>& curr)
                    {
                        return std::equal(curr.begin(), curr.end(), state_set.begin());
                    });
                    state_t state = it2 - Q.begin();
                    dfa_row[pair.first] = state;
                }
                dfa_table.push_back(dfa_row);
            }
            return dfa<char>(dfa_accepting_states, dfa_accepting_labels, dfa_table);
        }
    } // namespace automata
    
} // namespace final_project
