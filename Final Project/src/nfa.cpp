#include "automata/nfa.hh"

#include <stack>
#include <algorithm>
#include <iostream>

namespace final_project
{
    namespace automata
    {
    inline bool is_operator(char c)
    { 
        return (c == '|') || (c == '?') || (c =='*');
    }

    nfa build_nfa(const std::pair<std::string, std::vector<char>>& regex_pair)
    {
        std::stack<nfa> nfa_stack;
        auto regex = regex_pair.second;
        for(size_t i = 0; i < regex.size(); ++i)
        {
            char c = regex[i];
            if (!is_operator(c))
            {
                if (c == '$')
                    c = EPSILON;
                else if(c == '\\') //Recognize escaped characters
                    c = regex[++i];
                std::set<char> alphabet;
                alphabet.insert(c);
                nfa n = {alphabet, {1}, {{1, regex_pair.first}}, {{NFA_TRANSITION(c, 1)}, {NFA_TRANSITION(EPSILON, ACCEPT)}}};
                nfa_stack.push(n);
            }
            else if (c == '?')
            {
                //Get NFAs to concatenate 
                nfa n2 = nfa_stack.top();
                nfa_stack.pop();
                nfa n1 = nfa_stack.top();
                nfa_stack.pop();
                //Increment all states in NFA 2 by 
                //size of NFA 1 - 1
                n2.renumber(n1._M_transitions.size());
                //Get state tables
                auto table1 = n1._M_transitions;
                auto table2 = n2._M_transitions;
                //Add epsilon transition from NFA 1 accepting state 
                //to NFA 2 start state 
                table1.back()[EPSILON] = {static_cast<state_t>(table1.size())};
                //Merge tables 
                nfa::table_t new_table;
                for(const auto& row: table1)
                    new_table.push_back(row);
                for(const auto& row: table2)
                    new_table.push_back(row);
                //Merge alphabets 
                std::set<char> merged_alphabet;
                std::set_union(n1._M_alphabet.begin(), n1._M_alphabet.end(),
                               n2._M_alphabet.begin(), n2._M_alphabet.end(),
                               std::inserter(merged_alphabet, merged_alphabet.end()));
                //Create NFA
                nfa concatenated = {merged_alphabet, n2._M_accepting_states, {{n2._M_accepting_states.back(), regex_pair.first}}, new_table};
                nfa_stack.push(concatenated);
            } 
            else if (c == '|')
            {   
                //Get NFAs
                nfa n2 = nfa_stack.top();
                nfa_stack.pop();
                nfa n1 = nfa_stack.top();
                nfa_stack.pop();

                //Re-number NFA 1 to account for new start state 
                n1.renumber(1);
                //Re-number NFA 2 to account for new start state +
                //addition of NFA 1
                n2.renumber(1 + n1._M_transitions.size());
                //Make last state in NFA 1 and 2 transition to new 
                //accepting state 
                 state_t accepting_num = 1 + n1._M_transitions.size() + n2._M_transitions.size();
                n1._M_transitions.back()[EPSILON] = {accepting_num};
                n2._M_transitions.back()[EPSILON] = {accepting_num};
                //Create new start state
                std::unordered_map<char, std::vector<state_t>> start_transition = {NFA_TRANSITION(EPSILON, 1, 1 + static_cast<state_t>(n1._M_transitions.size()))};
                //Create accepting state
                std::unordered_map<char, std::vector<state_t>> accepting_state = {NFA_TRANSITION(EPSILON, ACCEPT)};
                //Create new table 
                nfa::table_t new_table;
                new_table.reserve(accepting_num);
                new_table.push_back(start_transition);
                //Add NFA 1 and NFA 2 
                for(const auto& row: n1._M_transitions)
                    new_table.push_back(row);
                for(const auto& row: n2._M_transitions)
                    new_table.push_back(row);
                //Add accepting state 
                new_table.push_back(accepting_state);
                //Create accepting state vector
                std::vector<state_t> accepting = {accepting_num};
                //Merge alphabets
                std::set<char> merged_alphabet;
                std::set_union(n1._M_alphabet.begin(), n1._M_alphabet.end(),
                               n2._M_alphabet.begin(), n2._M_alphabet.end(),
                               std::inserter(merged_alphabet, merged_alphabet.end()));
                //Create new NFA and push it onto stack 
                nfa merged = {merged_alphabet, accepting, {{accepting_num, regex_pair.first}}, new_table};
                nfa_stack.push(merged);
            
            } else if (c == '*')
            {
                //Get NFA
                nfa n = nfa_stack.top();
                nfa_stack.pop();
                //Add transition from NFA accept state to NFA start state
                n._M_transitions.back()[EPSILON] = {0};
                //Re-number NFA states to account for start state 
                n.renumber(1);
                //Add transition last NFA state to new accepting state 
                state_t accepting_num = 1 + n._M_transitions.size();
                n._M_transitions.back()[EPSILON].push_back(accepting_num);
                //Create state state 
                std::unordered_map<char, std::vector<state_t>> start_state = {NFA_TRANSITION(EPSILON, 1, accepting_num)};
                //Create accepting state
                std::unordered_map<char, std::vector<state_t>> accepting_state = {NFA_TRANSITION(EPSILON, ACCEPT)};
                //Create new transition table
                nfa::table_t new_table;
                new_table.reserve(accepting_num);
                new_table.push_back(start_state);
                for(const auto& row: n._M_transitions)
                    new_table.push_back(row);
                new_table.push_back(accepting_state);
                nfa merged = {n._M_alphabet, {accepting_num}, {{accepting_num, regex_pair.first}}, new_table};
                nfa_stack.push(merged);
            }
        }
        return nfa_stack.top();
    }

    nfa build_nfa(const std::vector<std::pair<std::string, std::vector<char>>>& regex)
    {
        nfa n;
        for(const auto& r: regex)
        {
            //Create sub-NFA
            nfa subunit = build_nfa(r);
            //Add epsilon transition to sub-NFA start state
            if(n._M_transitions.empty()) //sub-NFA is first NFA made
            {
                subunit.renumber(1);
                n._M_transitions.push_back({NFA_TRANSITION(EPSILON, 1)});
            }
            else //sub-NFA is being added to current NFA
            {
                subunit.renumber(n._M_transitions.size());
                n._M_transitions[0][EPSILON].push_back(n._M_transitions.size());
            }
            //Combine tables
            for(const auto& row: subunit._M_transitions)
                n._M_transitions.push_back(row);
            //Combine accepting states
            for(auto state: subunit._M_accepting_states)
                n._M_accepting_states.push_back(state);
            //Merge alphabets
            std::set<char> merged_alphabet;
            std::set_union(n._M_alphabet.begin(), n._M_alphabet.end(),
                            subunit._M_alphabet.begin(), subunit._M_alphabet.end(),
                            std::inserter(merged_alphabet, merged_alphabet.end()));
            n._M_alphabet = merged_alphabet;
            //Combine labels 
            auto label = subunit._M_accepeting_state_labels.begin()->second;
            n._M_accepeting_state_labels[n._M_accepting_states.back()] = label;
        }
        return n;
    }
    }
}

#include <iostream>

std::ostream& operator<<(std::ostream& os, const final_project::automata::nfa& n)
    {
        os << "Alphabet: {";
        auto it = n._M_alphabet.begin();
        for(; std::distance(it, n._M_alphabet.end()) > 1; ++it)
            os << *it << ", ";
        os << *(n._M_alphabet.rbegin()) << "}";
        os << "\nAccepting states: {";
        for(size_t i = 0; i < n._M_accepting_states.size() - 1; ++i)
            os << n._M_accepting_states[i] << ", ";
        os << n._M_accepting_states.back() << "}";
        os << "\nAccepting labels: {\n";
        for(const auto& label: n._M_accepeting_state_labels)
        {
            os << "{" << label.first << ": " << label.second << "}";
        }
        os << "}";
        os << "\nTransition table: \n";
        for(const auto& row: n._M_transitions)
        {
            os << "\t";
            for(const auto& state: row)
            {
                char c = state.first;
                if (c == final_project::automata::EPSILON)
                    os << "{epsilon, {";
                else
                    os << "{" << c << ", {";
                for(size_t i = 0; i < state.second.size() -1; ++i)
                {
                    if(state.second[i] == final_project::automata::ACCEPT)
                        os << "ACCEPT, ";
                    else
                        os << state.second[i] << ", ";
                }
                if(state.second.back() == final_project::automata::ACCEPT)
                    os << "ACCEPT" << "}} ";
                else
                    os << state.second.back() << "}} ";
            }
            os << "\n";
        }
        return os;
    }