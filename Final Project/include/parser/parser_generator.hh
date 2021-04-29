#ifndef PARSER_GENERATOR_HH
#define PARSER_GENERATOR_HH 1

#include "automata/dfa.hh"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <istream>
#include <utility>
#include <limits>



namespace final_project
{
    namespace parser
    {
        const static char EPSILON = 0;
        const static std::string TP_EOF = "$";
        const static automata::state_t TP_ACCEPT = std::numeric_limits<automata::state_t>::max();

        class parser_generator
        {
            public:
                struct rule_t
                {
                    size_t _M_pointer;
                    std::string _M_lhs;
                    std::vector<std::string> _M_rhs;
                    //std::vector<std::string> _M_node_types;

                    rule_t& operator++()
                    {
                        ++_M_pointer;
                        return *this;
                    }

                    rule_t operator++(int)
                    {
                        rule_t temp(*this);
                        ++_M_pointer;
                        return temp;
                    }

                };

                struct item_set_t
                {
                    std::set<rule_t> _M_rules;

                    bool add_rule(const rule_t& rule)
                    {
                        size_t old_size = _M_rules.size();
                        _M_rules.insert(rule);
                        return _M_rules.size() != old_size;
                    }

                    bool has_rule(const rule_t& rule) const 
                    {
                        return std::find(_M_rules.begin(), _M_rules.end(), rule) != _M_rules.end();
                    }

                    [[gnu::noinline]]
                    size_t num_rules() const 
                    {
                        return _M_rules.size();
                    }
                };
            
                struct extended_rule_t
                {
                    //Represents a symbol in the extended grammar 
                    typedef std::pair<std::string, std::pair<size_t, size_t>> symbol;
                    symbol _M_lhs;
                    std::vector<symbol> _M_rhs;

                    extended_rule_t() = default;

                    bool operator<(const extended_rule_t& other)
                    {
                        return (_M_rhs.size() < other._M_rhs.size());
                    }
                };

                struct symbol_hash
                {
                    size_t operator()(const extended_rule_t::symbol& s) const 
                    {
                        auto lhs = s.first;
                        auto rhs = s.second;
                        size_t hash = 0;
                        hash ^= std::hash<std::string>{}(lhs) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                        hash ^= rhs.first  + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                        hash ^= rhs.second  + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                        return hash;
                    } 
                };

                typedef std::vector<std::unordered_map<std::string, size_t>> translation_table_t;
            public:
                //Constructs a parser generator that reads in a context free grammar from the specified 
                //input stream 
                //
                //@param in the input stream from which to read the context free grammar
                explicit parser_generator(std::istream& in);

                //Creates the LALR(1) parse tables
                std::pair<automata::dfa<std::string>, automata::dfa<std::string>> create_parse_tables();

                static std::ostream& print_tables(std::ostream& os, const automata::dfa<std::string>& action, 
                    const automata::dfa<std::string>& goto_table);
            #ifdef DEBUG
            public:
            #else
            private:
            #endif
                //Returns true if the string represents a terminal symbol 
                //@param str the string to evaluate
                bool is_terminal(const std::string& str) const;

                //Creates the LALR(1) goto table for the specified grammar
                //
                //@return the LALR(1) goto table
                automata::dfa<std::string> create_goto_table();

                //Creates the LALR(1) action table for the specified grammar 
                //
                //@param sets the grammar's item sets 
                //@return the LALR(1) action table
                automata::dfa<std::string> create_action_table(const std::vector<parser_generator::item_set_t>& sets);

                //Returns the parser generator's grammar 
                //
                //@return the parser generators grammar
                const std::vector<rule_t>& get_grammar() const;
                //Creates the item sets for the grammar
                //@return the grammar's item sets
                std::vector<item_set_t> create_item_sets();

                //Creates the translations table for the grammar 
                //
                //@param sets the grammar's item sets
                void create_translation_table(const std::vector<parser_generator::item_set_t>& sets);

                //Creates an extended grammar for the item sets and translation table
                //
                //@param sets the grammar's item sets 
                void create_extended_grammar(const std::vector<parser_generator::item_set_t>& sets);
                
                //Creates the extended grammar's first sets
                //
                //@param g the extended grammar 
                void create_first_sets();
                std::set<std::string> first(const extended_rule_t::symbol& s);
                //Determines the first set for the specified vector of grammar symbols
                //
                //@param g the extended grammar 
                //@param s the vector of grammar symbols 
                //@return the first set for the vector of grammar symbols
                std::set<std::string> first(const std::vector<extended_rule_t::symbol>& s);
                
                //Determines if the specified grammar symbol is nullable that is
                //if a -> epsilon
                //
                //@param g the extended grammar 
                //@param s the specified symbol 
                //@return true if the symbol is nullable
                bool nullable(const extended_rule_t::symbol& s);

                //Determines if the specifed vector of grammar symbols is nullable 
                //
                //@param g the extended grammar 
                //@param s the specified symbol 
                //@return true if the vector of grammar symbols is nullable
                bool nullable(const std::vector<parser_generator::extended_rule_t::symbol>& s);

                void compute_nullables();
                //Creates the extended grammar's first sets
                //
                //@param g the extended grammar 
                //@return the grammar's first set
                void create_follow_sets();
            private:
                item_set_t create_item_set(const rule_t& kernel);
                void parse_grammar(std::istream& in);
            #ifdef DEBUG
            public: //public for testing ONLY
            #else
            private:
            #endif
                std::istream& _M_in;
                //Set of terminals
                std::set<std::string> _M_terminals;
                //All possible input characters
                std::set<std::string> _M_inputs;
                 //List of grammar rules
                std::vector<rule_t> _M_grammar;
                //Translation table
                translation_table_t _M_translation_table;
                //Nullable
                std::unordered_map<extended_rule_t::symbol, bool, symbol_hash> _M_nullable;
                //First sets 
                std::unordered_map<extended_rule_t::symbol, std::set<std::string>, symbol_hash> _M_first_sets;
                //Follow sets
                std::unordered_map<extended_rule_t::symbol, std::set<std::string>, symbol_hash> _M_follow_sets;
                //Extended grammar
                std::vector<extended_rule_t> _M_extended_grammar;
        };

        inline bool operator<(const parser_generator::rule_t& lhs, const parser_generator::rule_t& other)
        {
            if (lhs._M_lhs < other._M_lhs)
                return true;
            if (lhs._M_lhs > other._M_lhs)
                return false;
            if (lhs._M_rhs.size() < other._M_rhs.size())
                return true;
            if (lhs._M_rhs.size() > other._M_rhs.size())
                return false;
            for(size_t i = 0; i < lhs._M_rhs.size(); ++i)
            {
                if (lhs._M_rhs[i] < other._M_rhs[i])
                    return true;
                if (lhs._M_rhs[i] > other._M_rhs[i])
                    return false;
            }
            if (lhs._M_pointer < other._M_pointer)
                return true;
            return false;
        }

        inline bool operator==(const parser_generator::rule_t& lhs, const parser_generator::rule_t& other)
        {
            if(lhs._M_lhs != other._M_lhs)
                return false;
            if(!std::equal(lhs._M_rhs.begin(), lhs._M_rhs.end(), other._M_rhs.begin()))
                return false;
            return lhs._M_pointer == other._M_pointer;
        }

        inline bool operator>(const parser_generator::rule_t& lhs, const parser_generator::rule_t& rhs)
        {
            return !(lhs < rhs) && !(lhs == rhs);
        }


        inline bool operator<(const parser_generator::item_set_t& lhs, const parser_generator::item_set_t& rhs)
        {
            if(lhs.num_rules() < rhs.num_rules())
                return true;
            if(lhs.num_rules() > rhs.num_rules())
                return false;
            for(auto it1 = lhs._M_rules.begin(), it2 = rhs._M_rules.begin(); it1 != lhs._M_rules.end(); ++it1, ++it2)
            {
                if(*it1 < *it2)
                    return true;
                if(*it1 > *it2)
                    return false;
            }
            return false;
        }

    } // namespace parser
} // namespace final_project



#endif