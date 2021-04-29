#ifdef DEBUG

#include "unit_test_framework.hh"
#include "parser/parser_generator.hh"
#include <fstream>
#include <iostream>

TESTING_SETUP()

using namespace final_project::parser;

//Will be testing each function in this test file 
//instead of each function multiple times. 
//Most functions will only have one test 
//because it is very hard to write test case 
//solutions be hand. 

std::ostream& operator<<(std::ostream& os, const parser_generator::rule_t& r)
{
    os << r._M_lhs;
    os << "->";
    for(size_t i = 0; i < r._M_rhs.size(); ++i)
    {
        if (i == r._M_pointer)
            os << ". ";
        os << r._M_rhs[i] << " ";
    }

    if(r._M_pointer == r._M_rhs.size())
        os << ".";
    return os;
}

std::ostream& operator<<(std::ostream& os, const final_project::parser::parser_generator::translation_table_t& t)
{
    os << "\nTransition table: \n";
    for(const auto& row: t)
    {
        os << "\t";
        for(const auto& state: row)
        {
            std::string c = state.first;
            os << "{" << c << ", {";
            os << state.second << "}} ";
        }
        os << "\n";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const final_project::parser::parser_generator::extended_rule_t& r)
{
    auto lhs = r._M_lhs;
    os << lhs.second.first << lhs.first << lhs.second.second;
    os << "->";
    for(const auto& s: r._M_rhs)
    {
        os << s.second.first << "_" << s.first << "_" << s.second.second << " ";
    }
    return os;
}

void print_action_table(const final_project::automata::dfa<std::string>& dfa)
{
    auto transitions = dfa.get_table();
    for(size_t i = 0; i < transitions.size(); ++i)
    {
        std::cout << "State " << i << ":";
        const auto& row = transitions[i];
        for(const auto& transition: row)
        {
    
            std::cout << "\n\t" << transition.first << ": ";
            if (transition.second == TP_ACCEPT)
                std::cout << "Accept";
            else if (transition.second >= 0)
                std::cout << "Shift and go to state " << transition.second;
            else
                std::cout << "Reduce using rule " << -1*transition.second;
        }
        std::cout << "\n";
    }
}

BEGIN_TEST(Grammar_Parser, Test grammar parser)
    std::ifstream fin("C:\\Users\\aschi\\Dropbox\\AERO 552\\Final Project\\tests\\test_grammar.txt");
    parser_generator p(fin);
    auto grammar = p.get_grammar();
    for(auto rule: grammar)
    {
        std::cout << rule;
        std::cout << std::endl;
    }
    passed = true;
END_TEST()

BEGIN_TEST(Item_Sets, Test making item sets)
    std::ifstream fin("C:\\Users\\aschi\\Dropbox\\AERO 552\\Final Project\\tests\\test_grammar.txt");
    parser_generator p(fin);
    auto item_sets = p.create_item_sets();
    int curr_set = 0;
    for(const auto& set: item_sets)
    {
        std::cout << "Set I" << curr_set << std::endl;
        for(const auto& rule: set._M_rules)
            std::cout << "\t" << rule << std::endl;
        std::cout << std::endl;
        ++curr_set;
    }
END_TEST()

BEGIN_TEST(Translation_Table, Test making translation table)
    std::ifstream fin("C:\\Users\\aschi\\Dropbox\\AERO 552\\Final Project\\tests\\test_grammar.txt");
    parser_generator p(fin);
    auto item_sets = p.create_item_sets();
    p.create_translation_table(item_sets);
    std::cout << p._M_translation_table << std::endl;
END_TEST()

BEGIN_TEST(Extended_Grammar, Test extended grammar)
    std::ifstream fin("C:\\Users\\aschi\\Dropbox\\AERO 552\\Final Project\\tests\\test_grammar.txt");
    parser_generator p(fin);
    auto item_sets = p.create_item_sets();
    p.create_translation_table(item_sets);
    p.create_extended_grammar(item_sets);
    for(const auto& rule: p._M_extended_grammar)
    {
        std::cout << rule << std::endl;
    }
END_TEST()

BEGIN_TEST(First_Set, Test making first set)
    std::ifstream fin("C:\\Users\\aschi\\Dropbox\\AERO 552\\Final Project\\tests\\test_grammar.txt");
    parser_generator p(fin);
    auto item_sets = p.create_item_sets();
    p.create_translation_table(item_sets);
    p.create_extended_grammar(item_sets);
    p.create_first_sets();
    auto sets = p._M_first_sets;
    for(const auto& pair: sets)
    {
        auto s = pair.first;
        std::cout << "{" << s.second.first << "_" << s.first << "_" << s.second.second << ", {";
        for(const auto& str: pair.second)
        {
            std::cout << str << " ";
        }
        std::cout << "}}\n";
    }
END_TEST()

BEGIN_TEST(Follow_Set, Test making follow set)
    std::ifstream fin("C:\\Users\\aschi\\Dropbox\\AERO 552\\Final Project\\tests\\test_grammar.txt");
    parser_generator p(fin);
    auto item_sets = p.create_item_sets();
    p.create_translation_table(item_sets);
    p.create_extended_grammar(item_sets);
    p.create_first_sets();
    p.create_follow_sets();
    auto sets = p._M_follow_sets;
    for(const auto& pair: sets)
    {
        auto s = pair.first;
        std::cout << "{" << s.second.first << "_" << s.first << "_" << s.second.second << ", {";
        for(const auto& str: pair.second)
        {
            std::cout << str << " ";
        }
        std::cout << "}}\n";
    }
END_TEST()

BEGIN_TEST(GOTO_Table, Test making GOTO table)
    std::ifstream fin("C:\\Users\\aschi\\Dropbox\\AERO 552\\Final Project\\tests\\test_grammar.txt");
    parser_generator p(fin);
    auto item_sets = p.create_item_sets();
    p.create_translation_table(item_sets);
    p.create_extended_grammar(item_sets);
    p.create_first_sets();
    p.create_follow_sets();
    auto table = p.create_goto_table();
    std::cout << table << std::endl;
END_TEST()

BEGIN_TEST(Action_Table, Test making action table)
    std::ifstream fin("C:\\Users\\aschi\\Dropbox\\AERO 552\\Final Project\\tests\\test_grammar.txt");
    parser_generator p(fin);
    auto item_sets = p.create_item_sets();
    p.create_translation_table(item_sets);
    p.create_extended_grammar(item_sets);
    p.create_first_sets();
    p.create_follow_sets();
    auto table = p.create_action_table(item_sets);
    print_action_table(table);
END_TEST()

TEST_MAIN()

#endif