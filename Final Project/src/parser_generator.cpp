#include "parser/parser_generator.hh"
#include "exception/exceptions.hh"

#include <queue>
#include <sstream>
#include <iterator>
#include <iostream>

namespace final_project
{
    namespace parser
    {

       parser_generator::parser_generator(std::istream& in)
            : _M_in(in), _M_terminals(), _M_inputs(), _M_grammar(), _M_translation_table(),
            _M_first_sets{}, _M_follow_sets{}, _M_extended_grammar()
        {
            parse_grammar(_M_in);
        }

        bool parser_generator::is_terminal(const std::string& str) const 
        {
            return _M_terminals.find(str) != _M_terminals.end();
        }

        const std::vector<parser_generator::rule_t>& parser_generator::get_grammar() const 
        {
            return _M_grammar;
        }

        void parser_generator::parse_grammar(std::istream& in)
        {
            std::string line;
            //Read in rules 
            while(getline(in, line))
            {
                if(line.find("terminals:") != std::string::npos)
                {
                    std::stringstream ss(line);
                    std::istream_iterator<std::string> terminal_itr(ss);
                    ++terminal_itr;
                    std::string terminal;
                    for(; !(terminal_itr == std::istream_iterator<std::string>()); ++terminal_itr)
                    {
                        _M_terminals.insert(*terminal_itr);
                    }
                    continue;
                }
                if (line[0] == '#')
                    continue;
                std::stringstream rule_ss(line);
                std::istream_iterator<std::string> itr(rule_ss);
                std::string lhs = *itr++;
                _M_inputs.insert(lhs);
                ++itr; //skip ->
                std::vector<std::string> rhs;
                if(itr == std::istream_iterator<std::string>()) //RHS is epsilon
                    rhs.push_back("\0");
                for(; !(itr == std::istream_iterator<std::string>()); ++itr)
                {
                    rhs.push_back(*itr);
                    _M_inputs.insert(*itr);
                }
                if (lhs == "S")
                    rhs.push_back(TP_EOF);
                rule_t r = {0, lhs, rhs};
                _M_grammar.push_back(r);
            }
        }

        std::vector<parser_generator::item_set_t> parser_generator::create_item_sets()
        {
            auto I0 = create_item_set(_M_grammar.front());
            std::set<item_set_t> item_sets;
            std::queue<item_set_t> work_list;
            work_list.push(I0);
            item_sets.insert(I0);

            while(!work_list.empty())
            {
                auto curr_set = work_list.front();
                work_list.pop();

                for(const auto& input: _M_inputs)
                {
                    //Find rules "expecting" the given input 
                    auto rules = curr_set._M_rules;
                    for(auto rule : rules)
                    {
                        //Create a new item set with the rule as the kernel
                        if(rule._M_pointer < rule._M_rhs.size() && rule._M_rhs[rule._M_pointer] == input)
                        {
                            auto item_set = create_item_set(++rule);
                            bool inserted = item_sets.insert(item_set).second;
                            //If we have not already seen the item set, add it to the work list
                            if (inserted)
                                work_list.push(item_set);
                        }
                    }
                }
            }
            return std::vector<parser_generator::item_set_t>(item_sets.begin(), item_sets.end());
        }

        parser_generator::item_set_t parser_generator::create_item_set(const rule_t& kernel)
        {
            item_set_t I;
            I.add_rule(kernel);
            std::queue<rule_t> work_list;
            work_list.push(kernel);

            //Similar to BFS
            while(!work_list.empty())
            {
                auto rule = work_list.front();
                work_list.pop();
                //If pointer is at end of rule, there is nothing to be done
                if (rule._M_pointer >= rule._M_rhs.size())
                    continue;

                std::string first = rule._M_rhs[rule._M_pointer];
                if(!is_terminal(first))
                {
                    for(const auto& r: _M_grammar)
                    {
                        if(r._M_lhs == first)
                        {
                            if(!I.has_rule(r))
                            {
                                I.add_rule(r);
                                work_list.push(r);
                            }
                        }
                    }
                }
            }
            return I;
        }

        void parser_generator::create_translation_table(const std::vector<parser_generator::item_set_t>& sets)
        {
            _M_translation_table.resize(sets.size());
            //Iterator over item set 
            for(size_t i = 0; i < sets.size(); ++i)
            {
                const auto& curr_set = sets[i];
                //Give each item set each possible input
                for(const auto& input: _M_inputs)
                {
                    const auto& rules = curr_set._M_rules;
                    for(auto rule : rules) 
                    {
                        //Create a new item set with the rule as the kernel
                        if(rule._M_pointer < rule._M_rhs.size() && rule._M_rhs[rule._M_pointer] == input)
                        {
                            //Advance rule pointer
                            ++rule;
                            //Find item set containing rule 
                            auto it = std::find_if(sets.begin(), sets.end(), [&](const item_set_t& s)
                                {
                                    return std::find(s._M_rules.begin(), s._M_rules.end(), rule) != 
                                        s._M_rules.end();
                                });
                            //Insert into translation table
                            if(it != sets.end())
                                _M_translation_table[i][input] = std::distance(sets.begin(), it);
                        }
                    }
                }
            }
        }

        void parser_generator::create_extended_grammar(const std::vector<parser_generator::item_set_t>& sets)
        {
            //Iterate over all item sets
            for(size_t i = 0; i < sets.size(); ++i)
            {
                const auto& set = sets[i];
                for(const auto& rule: set._M_rules)
                {
                    if (rule._M_pointer == 0)
                    {
                        //Follow rule to add to grammar
                        //LHS of rule
                        size_t lhs_start = i;
                        size_t lhs_end;
                        auto it = _M_translation_table[i].find(rule._M_lhs);
                        if (it != _M_translation_table[i].end())
                            lhs_end = it->second;
                        parser_generator::extended_rule_t::symbol lhs{rule._M_lhs, {lhs_start, lhs_end}};
                        //RHS rule 
                        std::vector<parser_generator::extended_rule_t::symbol> rhs_symbols;
                        size_t start = i;
                        for(const auto& symbol: rule._M_rhs)
                        {
                            size_t end;
                            auto it = _M_translation_table[start].find(symbol);
                            if (it != _M_translation_table[start].end())
                                end = it->second;
                            rhs_symbols.push_back({symbol, {start, end}});
                            start = end;
                        }
                        _M_extended_grammar.push_back({lhs, rhs_symbols});
                    }
                }
            }
        }

        bool parser_generator::nullable(const extended_rule_t::symbol& s)
        {
            if(s.first[0] == EPSILON)
                return true;
            if (s.first == "$")
                return true;
            if(is_terminal(s.first))
            {
                _M_nullable[s] = false;
                return false;
            }
            else
            {
                bool n = false;
                for(const auto& rule: _M_extended_grammar)
                {
                    if (rule._M_lhs == s)
                        n = n || nullable(rule._M_rhs);
                }
                return n;
            }
        }

        bool parser_generator::nullable(const std::vector<parser_generator::extended_rule_t::symbol>& s)
        {
            bool n = true;
            for (const auto& symb: s)
            {
                n = n && _M_nullable[symb];
            }
            return n;
        }

        void parser_generator::compute_nullables()
        {
            std::set<extended_rule_t::symbol> non_terminals;
            std::transform(_M_extended_grammar.begin(), _M_extended_grammar.end(), std::inserter(non_terminals, non_terminals.end()), 
                [](const extended_rule_t& r)
                {
                    return r._M_lhs;
                });
            for(const auto& nt: non_terminals)
                _M_nullable[nt] = false;
            while(true)
            {
                bool changed = false;
                for(const auto& nt: non_terminals)
                {
                    bool old = _M_nullable[nt];
                    _M_nullable[nt] = nullable(nt);
                    if(_M_nullable[nt] != old)
                        changed = true;
                }
                if(!changed)
                    break;
            }
        }

        std::set<std::string> parser_generator::first(const extended_rule_t::symbol& s)
        {
            if(is_terminal(s.first))
                return {s.first};
            else 
                return _M_first_sets[s];
        }

        std::set<std::string> parser_generator::first(const std::vector<parser_generator::extended_rule_t::symbol>& s)
        {
            std::set<std::string> first_set;

            if(s.size() == 1 || !_M_nullable[s.front()])
            {
                if (is_terminal(s.front().first))
                {
                    first_set.insert(s.front().first);
                }
                else
                {
                    auto set = _M_first_sets[s.front()];
                    first_set.insert(set.begin(), set.end());
                }
            }
            else
            {
                if (is_terminal(s.front().first))
                {
                    first_set.insert(s.front().first);
                }
                else
                {
                    auto set = _M_first_sets[s.front()];
                    first_set.insert(set.begin(), set.end());
                }
                auto set = first(std::vector<parser_generator::extended_rule_t::symbol>(s.begin() + 1, s.end()));
                first_set.insert(set.begin(), set.end());
            }
            return first_set;
        }

        void parser_generator::create_first_sets()
        {
            std::set<extended_rule_t::symbol> non_terminals;
            std::transform(_M_extended_grammar.begin(), _M_extended_grammar.end(), std::inserter(non_terminals, non_terminals.end()), 
                [](const extended_rule_t& r)
                {
                    return r._M_lhs;
                });
            //Assume all sets are empty
            for(const auto& nt: non_terminals)
                _M_first_sets[nt] = {};
            while(true)
            {
                bool changed = false;
                for(size_t i = 0; i < _M_extended_grammar.size(); ++i)
                {
                    auto rule = _M_extended_grammar[i];
                    auto first_set = first(rule._M_rhs);
                    auto it = _M_first_sets.find(rule._M_lhs);
                    if (it == _M_first_sets.end())
                    {
                        _M_first_sets[rule._M_lhs] = first_set;
                        changed = true;
                    }
                    else 
                    {
                        auto& set = _M_first_sets[rule._M_lhs];
                        size_t old_size = set.size();
                        set.insert(first_set.begin(), first_set.end());
                        if(old_size != set.size())
                            changed = true;
                    }
                }

                if(!changed)
                    break;
            }
        }

        void parser_generator::create_follow_sets()
        {
            std::set<extended_rule_t::symbol> non_terminals;
            std::transform(_M_extended_grammar.begin(), _M_extended_grammar.end(), std::inserter(non_terminals, non_terminals.end()), 
                [](const extended_rule_t& r)
                {
                    return r._M_lhs;
                });
            //Initalize FOLLOW sets
            for(const auto& nt : non_terminals)
            {
                if(nt.first == "S")
                    _M_follow_sets[nt].insert(TP_EOF);
                for(const auto& rule: _M_extended_grammar)
                {
                    auto it = std::find(rule._M_rhs.begin(), rule._M_rhs.end(), nt);
                    if(it != rule._M_rhs.end())
                    {
                        std::set<std::string> follow_set;
                        bool found_non_nullable = false;
                        //If there are non-nullable symbols after NT
                        //the follow set of NT contains the first set of 
                        //all the nullable symbols with the first set of the 
                        //first non-nullable set
                        while(++it != rule._M_rhs.end())
                        {
                            auto first_set = first(*it);
                            for(const auto& elt: first_set)
                            {
                                if (elt[0] != EPSILON)
                                    follow_set.insert(elt);
                            }
                            //If symbol is nullable, stop taking union of FIRST sets 
                            bool n = (is_terminal(it->first)) ? false : (it->first == "$") ? true : _M_nullable[*it];
                            if(!n)
                            {
                                found_non_nullable = true;
                                break;
                            }
                        }
                        if(found_non_nullable)
                            _M_follow_sets[nt].insert(follow_set.begin(), follow_set.end());
                    }
                }
            }
            //Fixed point interation
            while(true)
            {
                bool changed = false;
                for(const auto& nt: non_terminals)
                {
                    for(const auto& rule: _M_extended_grammar)
                    {
                        auto it = std::find(rule._M_rhs.begin(), rule._M_rhs.end(), nt);
                        //nt is in RHS of given rule
                        if(it != rule._M_rhs.end())
                        {
                            bool found_non_nullable = false;
                            //Check if there are non-nullable symbols after non-terminal
                            while(++it != rule._M_rhs.end())
                            {
                                bool n = (is_terminal(it->first)) ? false : (it->first == "$") ? true : _M_nullable[*it];
                                if(!n)
                                {
                                    found_non_nullable = true;
                                    break;
                                }
                            }
                            //If no non-nullable symbols, add FOLLOW(LHS) to FOLLOW(NT)
                            if(!found_non_nullable)
                            {
                                auto it2 = _M_follow_sets.find(rule._M_lhs);
                                if (it2 != _M_follow_sets.end())
                                {
                                    //We can short-cut here
                                    size_t old_size = _M_follow_sets[nt].size();
                                    _M_follow_sets[nt].insert(it2->second.begin(), it2->second.end());
                                    if(old_size != _M_follow_sets[nt].size())
                                        changed = true;
                                }
                            }
                        }
                    }
                }
                if(!changed)
                    break;
            }
        }

        automata::dfa<std::string> parser_generator::create_goto_table()
        {
            automata::dfa<std::string>::table_t dfa_table(_M_translation_table.size());
            for(size_t i = 0; i < _M_translation_table.size(); ++i)
            {   
                const auto& row = _M_translation_table[i];
                std::unordered_map<std::string, automata::state_t> dfa_row;
                for(const auto& transition: row)
                {
                    if(!is_terminal(transition.first))
                    {
                        dfa_row[transition.first] = static_cast<automata::state_t>(transition.second);
                    }
                }
                dfa_table[i] = dfa_row;
            }
            return automata::dfa<std::string>({}, dfa_table);
        }

        automata::dfa<std::string> parser_generator::create_action_table(const std::vector<parser_generator::item_set_t>& sets)
        {
            automata::dfa<std::string>::table_t dfa_table(_M_translation_table.size());
            //Add in accepting states 
           
            //Add in shifts
            for(size_t i = 0; i < _M_translation_table.size(); ++i)
            {
                const auto& row = _M_translation_table[i];
                std::unordered_map<std::string, automata::state_t> dfa_row = dfa_table[i];
                for(const auto& transition: row)
                {
                    if(is_terminal(transition.first))
                        dfa_row[transition.first] = static_cast<automata::state_t>(transition.second);
                }
                dfa_table[i] = dfa_row;
            }
            //Add in reductions 
            std::unordered_map<size_t, std::pair<rule_t, std::set<std::string>>> condensed_grammar;
            for(const auto& rule: _M_extended_grammar)
            {
                //Get final set of rule
                auto final_set = rule._M_rhs.back().second.second;
                auto it = condensed_grammar.find(final_set);
                if(it == condensed_grammar.end())
                {  
                    std::string lhs = rule._M_lhs.first;
                    std::vector<std::string> rhs;
                    std::transform(rule._M_rhs.begin(), rule._M_rhs.end(), std::back_inserter(rhs), 
                        [](const extended_rule_t::symbol& er)
                        {
                            return er.first;
                        });
                    rule_t r = {0, lhs, rhs};
                    auto it2 = _M_follow_sets.find(rule._M_lhs);
                    if(it2 != _M_follow_sets.end())
                    {
                        auto follow_set = it2->second;
                        condensed_grammar[final_set] = {r, follow_set};
                    }
                }
                else 
                {
                    auto final_set = rule._M_rhs.back().second.second;
                    auto& follow_set = it->second.second;
                    auto it2 = _M_follow_sets.find(rule._M_lhs);
                    if(it2 != _M_follow_sets.end())
                    {
                        auto set = it2->second;
                        follow_set.insert(set.begin(), set.end());
                    }
                }
            }
            for(const auto& row: condensed_grammar)
            {
                size_t state = row.first;
                auto it = std::find(_M_grammar.begin(), _M_grammar.end(), row.second.first);
                automata::state_t reduce_number = -(it-_M_grammar.begin());
                std::unordered_map<std::string, automata::state_t> dfa_row = dfa_table[state];
                for(const auto& symb: row.second.second)
                {
                    dfa_row[symb] = reduce_number;
                }
                dfa_table[state] = dfa_row;
            }

            for(size_t i = 0; i < sets.size(); ++i)
            {
                auto set = sets[i];
                for(const auto& rule: set._M_rules)
                {
                    if (rule._M_lhs == "S" && rule._M_pointer == rule._M_rhs.size() - 1)
                    {
                        dfa_table[i]["$"] = automata::ACCEPT;
                    }
                }
            }
            return automata::dfa<std::string>({}, dfa_table);
        }

        std::pair<automata::dfa<std::string>, automata::dfa<std::string>> parser_generator::create_parse_tables()
        {
            auto item_sets = create_item_sets();
            create_translation_table(item_sets);
            create_extended_grammar(item_sets);
            create_first_sets();
            create_follow_sets();
            auto goto_table = create_goto_table();
            auto action_table = create_action_table(item_sets);
            return std::make_pair(action_table, goto_table);
        }

        std::ostream& parser_generator::print_tables(std::ostream& os, const automata::dfa<std::string>& action, 
                    const automata::dfa<std::string>& goto_table)
        {
            os << "GOTO Table: \n";
            auto table = goto_table.get_table();
            for(size_t i = 0; i < table.size(); ++i)
            {
                auto row = table[i];
                os << "State " << i << ": ";
                for(const auto& transition: row)
                {
                    os << "\t" << transition.first << ": goto state " << transition.second;
                }
                os << "\n";
            }
            os << "\n\nAction Table: \n";
            auto transitions = action.get_table();
            for(size_t i = 0; i < transitions.size(); ++i)
            {
                os << "State " << i << ":";
                const auto& row = transitions[i];
                for(const auto& transition: row)
                {

                    os << "\n\t" << transition.first << ": ";
                    if (transition.second == TP_ACCEPT)
                        os << "Accept";
                    else if (transition.second >= 0)
                        os << "Shift and go to state " << transition.second;
                    else
                        os << "Reduce using rule " << -1*transition.second;
                }
                os << "\n";
            }
            return os;
        }
    } // namespace parser
    
} // namespace final_project
