#include "unit_test_framework.hh"

#include "automata/dfa.hh"
#include "automata/nfa.hh"
#include "automata/regex_parser.hh"

#include <sstream>

using namespace final_project::automata;
using namespace final_project::regex;

#define CREATE_NFA(exp)\
    std::string regex = exp;\
    std::istringstream str_in(regex);\
    regex_parser parser(str_in);\
    auto parsed = parser.parse();\
    nfa n = final_project::automata::build_nfa(parsed);

#define DFA_TRANSITION(c, s) {c, s}

#define PRINT_DFA(e, a)\
    std::ostringstream expected_str;\
    expected_str << e;\
    std::ostringstream actual_str;\
    actual_str << a;\
    std::cout << "**Expected DFA**" << std::endl;\
    std::cout << expected_str.str() << std::endl;\
    std::cout << "**Actual DFA**" << std::endl;\
    std::cout << actual_str.str() << std::endl;

TESTING_SETUP()

BEGIN_TEST(DFA_Constructor_1, Construct DFA from single character)
    CREATE_NFA("test: $")
    dfa<char> d = powerset_construction(n);
    /*
    dfa<char> expected (
        {1},
        {
            {DFA_TRANSITION('$', 1)}
        }
    );
    */
    std::cout << d;
    //CONTENT_CHECK(expected_str.str(), actual_str.str())
    //PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(DFA_Constructor_Alternation, Construct DFA from simple alternation)
    CREATE_NFA("test: a|$")
    dfa<char> d = powerset_construction(n);
    std::cout << d;
END_TEST()

BEGIN_TEST(DFA_Alternation_2, More complicated eample of the alternation operator)
    CREATE_NFA("test: ab|c")
    dfa<char> d = powerset_construction(n);
    std::cout << d;
END_TEST()

BEGIN_TEST(DFA_Constructor_Closure, Construct DFA from closure)
    CREATE_NFA("test: a*")
    dfa<char> d = powerset_construction(n);
    std::cout << d;
END_TEST()

BEGIN_TEST(DFA_Constructor_Concatentation, Construct DFA from simple concatentation)
    CREATE_NFA("test: ab")
    dfa<char> d = powerset_construction(n);
    std::cout << d;
END_TEST()

BEGIN_TEST(DFA_Constructor_Multi, DFA Constructor with multiple operators)
    CREATE_NFA("test: (b|c)*a")
    dfa<char> d = powerset_construction(n);
    std::cout << d;
END_TEST()

BEGIN_TEST(DFA_Constructor_Multi_Regex, DFA Generator with multiple regular expressions)
    CREATE_NFA("test1: abc\ntest2: a|b")
    dfa<char> d = powerset_construction(n);
    std::cout << d;
END_TEST()

TEST_MAIN()
