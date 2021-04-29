#include "automata/regex_parser.hh"
#include "automata/nfa.hh"
#include "unit_test_framework.hh"

#include <sstream>

using nfa = final_project::automata::nfa;
using regex_parser = final_project::regex::regex_parser;
auto EPSILON = final_project::automata::EPSILON;
auto ACCEPT = final_project::automata::ACCEPT;

#define PARSE_REGEX(exp)\
    std::string regex = exp;\
    std::istringstream str_in(regex);\
    regex_parser parser(str_in);\
    auto parsed = parser.parse();

#define PRINT_NFA(e, a)\
    std::ostringstream expected_str;\
    expected_str << e;\
    std::ostringstream actual_str;\
    actual_str << a;\
    std::cout << "**Expected NFA**" << std::endl;\
    std::cout << expected_str.str() << std::endl;\
    std::cout << "**Actual NFA**" << std::endl;\
    std::cout << actual_str.str() << std::endl;

TESTING_SETUP()

BEGIN_TEST(NFA_Generator_Basic, NFA for single character)
    PARSE_REGEX("test: a")
    nfa n = final_project::automata::build_nfa(parsed);
    nfa expected = 
    {
        {'a'}, 
        {2}, 
        {
            {2, "test"}
        },
        {
            {NFA_TRANSITION(EPSILON, 1)},
            {NFA_TRANSITION('a', 2)},
            {NFA_TRANSITION(EPSILON, ACCEPT)}
        }
    };
    PRINT_NFA(expected, n)
    CONTENT_CHECK(expected_str.str(), actual_str.str())
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(NFA_Generator_Concatenate_1, Concatenate two characters)
    PARSE_REGEX("test: ab")
    nfa n = final_project::automata::build_nfa(parsed);
    nfa expected = 
    {
        {'a', 'b'},
        {4},
        {
            {4, "test"}
        },
        {
            {NFA_TRANSITION(EPSILON, 1)},
            {NFA_TRANSITION('a', 2)},
            {NFA_TRANSITION(EPSILON, 3)},
            {NFA_TRANSITION('b', 4)},
            {NFA_TRANSITION(EPSILON, ACCEPT)}
        }
    };
    PRINT_NFA(expected, n)
    CONTENT_CHECK(expected_str.str(), actual_str.str())
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(NFA_Generator_Concatenate_2, Concatenate character and alternation operator)
    PARSE_REGEX("test: a(b|c)")
    nfa n = final_project::automata::build_nfa(parsed);
    nfa expected = 
    {
        {'a', 'b', 'c'},
        {8},
        {
            {8, "test"}
        },
        {
            {NFA_TRANSITION(EPSILON, 1)},
            {NFA_TRANSITION('a', 2)},
            {NFA_TRANSITION(EPSILON, 3)},
            {NFA_TRANSITION(EPSILON, 4, 6)},
            {NFA_TRANSITION('b', 5)},
            {NFA_TRANSITION(EPSILON, 8)},
            {NFA_TRANSITION('c', 7)},
            {NFA_TRANSITION(EPSILON, 8)},
            {NFA_TRANSITION(EPSILON, ACCEPT)}
        }
    };
    PRINT_NFA(expected, n)
    CONTENT_CHECK(expected_str.str(), actual_str.str())
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(NFA_Generator_Alternation_1, Alternation operator with two characters)
    PARSE_REGEX("test: a|b")
    nfa n = final_project::automata::build_nfa(parsed);
    nfa expected = 
    {
        {'a', 'b'},
        {6},
        {
            {6, "test"}
        },
        {
            {NFA_TRANSITION(EPSILON, 1)},
            {NFA_TRANSITION(EPSILON, 2,4)},
            {NFA_TRANSITION('a', 3)},
            {NFA_TRANSITION(EPSILON, 6)},
            {NFA_TRANSITION('b', 5)},
            {NFA_TRANSITION(EPSILON, 6)},
            {NFA_TRANSITION(EPSILON, ACCEPT)}
        }
    };
    PRINT_NFA(expected, n)
    CONTENT_CHECK(expected_str.str(), actual_str.str())
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(NFA_Generator_Alternation_2, Alternation operator with concatenation)
    PARSE_REGEX("test: ab|c")
    nfa n = final_project::automata::build_nfa(parsed);
    nfa expected = 
    {
        {'a', 'b', 'c'},
        {8},
        {
            {8, "test"}
        },
        {
            {NFA_TRANSITION(EPSILON, 1)},
            {NFA_TRANSITION(EPSILON, 2, 6)},
            {NFA_TRANSITION('a', 3)},
            {NFA_TRANSITION(EPSILON, 4)},
            {NFA_TRANSITION('b', 5)},
            {NFA_TRANSITION(EPSILON, 8)},
            {NFA_TRANSITION('c', 7)},
            {NFA_TRANSITION(EPSILON, 8)},
            {NFA_TRANSITION(EPSILON, ACCEPT)}
        }
    };
        PRINT_NFA(expected, n)
        CONTENT_CHECK(expected_str.str(), actual_str.str())
        PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(NFA_Generator_Closure_1, Closure operator with one character)
    PARSE_REGEX("test: a*")
    nfa n = final_project::automata::build_nfa(parsed);
    nfa expected = 
    {
        {'a'},
        {4},
        {
            {4, "test"}
        },
        {
            {NFA_TRANSITION(EPSILON, 1)},
            {NFA_TRANSITION(EPSILON, 2, 4)},
            {NFA_TRANSITION('a', 3)},
            {NFA_TRANSITION(EPSILON, {2, 4})},
            {NFA_TRANSITION(EPSILON, ACCEPT)}
        }
    };
    PRINT_NFA(expected, n)
    CONTENT_CHECK(expected_str.str(), actual_str.str())
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(NFA_Generator_Closure_2, Closure operator with concatenation)
    PARSE_REGEX("test: (ab)*")
    nfa n = final_project::automata::build_nfa(parsed);
    nfa expected = 
    {
        {'a', 'b'},
        {6},
        {
            {6, "test"}
        },
        {
            {NFA_TRANSITION(EPSILON, 1)},
            {NFA_TRANSITION(EPSILON, 2, 6)},
            {NFA_TRANSITION('a', 3)},
            {NFA_TRANSITION(EPSILON, 4)},
            {NFA_TRANSITION('b', 5)},
            {NFA_TRANSITION(EPSILON, 2, 6)},
            {NFA_TRANSITION(EPSILON, ACCEPT)}
        }
    };
    PRINT_NFA(expected, n)
    CONTENT_CHECK(expected_str.str(), actual_str.str())
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(NFA_Generator_Multiple_Operator, NFA generator for regular expression with multiple operators)
    PARSE_REGEX("test: a(b|c)*")
    nfa n = final_project::automata::build_nfa(parsed);
    nfa expected = 
    {
        {'a', 'b', 'c'},
        {10},
        {
            {10, "test"}
        },
        {
            {NFA_TRANSITION(EPSILON, 1)},
            {NFA_TRANSITION('a', 2)},
            {NFA_TRANSITION(EPSILON, 3)},
            {NFA_TRANSITION(EPSILON, 4, 10)},
            {NFA_TRANSITION(EPSILON, 5, 7)},
            {NFA_TRANSITION('b', 6)},
            {NFA_TRANSITION(EPSILON, 9)},
            {NFA_TRANSITION('c', 8)},
            {NFA_TRANSITION(EPSILON, 9)},
            {NFA_TRANSITION(EPSILON, 4, 10)},
            {NFA_TRANSITION(EPSILON, ACCEPT)}
        }
    };
    PRINT_NFA(expected, n)
    CONTENT_CHECK(expected_str.str(), actual_str.str())
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(NFA_Generator_Multiple_Regex_1, Combine two single character regex)
    PARSE_REGEX("test1: a\ntest2: b");
    nfa n = final_project::automata::build_nfa(parsed);
    nfa expected = 
    {
        {'a', 'b'}, 
        {2, 4},
        {
            {2, "test1"}, 
            {4, "test2"}
        },
        {
            {NFA_TRANSITION(EPSILON, 1, 3)},
            {NFA_TRANSITION('a', 2)},
            {NFA_TRANSITION(EPSILON, ACCEPT)},
            {NFA_TRANSITION('b', 4)},
            {NFA_TRANSITION(EPSILON, ACCEPT)}
        }
    };
    PRINT_NFA(expected, n)
    CONTENT_CHECK(expected_str.str(), actual_str.str())
    PASS_OR_FAIL()
END_TEST()

TEST_MAIN()