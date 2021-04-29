#include "unit_test_framework.hh"
#include "automata/regex_parser.hh"

#include <sstream>

TESTING_SETUP()

BEGIN_TEST(Regex_Parser_Test_Basic, This test tests if the regex parser can handle a very simple regex.)
    std::string regex = "test: abcd";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
    std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'a', 'b', '?', 'c', '?', 'd', '?'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Test_Parenthesis, This tests if the regex parser can properly handle parenthesis.)
    std::string regex = "test: (ab)((a))(01)";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
    std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'a', 'b', '?', 'a', '?', '0', '1', '?', '?'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Test_Alternation_1, This test tests if the regex parser can handle the simplest case of the 
    alternation operator.)
    std::string regex = "test: a|b";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
    std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'a', 'b', '|'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Test_Alternation_2, This test tests if the regex parser can handle the alternation
    operator with parenthesis.)
    std::string regex = "test: a|(bc)";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
    std::vector<std::pair<std::string,std::vector<char>>> expected = {{
        {"test", {'a', 'b', 'c', '?', '|'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Test_Alternation_3, This test tests if the regex parser can handle multiple alternation 
operators)
    std::string regex = "test: a|b|c";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
    std::vector<std::pair<std::string, std::vector<char>>> expected  {{
        {"test", {'a', 'b', '|', 'c', '|'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Test_Kleene_1, This tests tests if the regex parser can handle the simplest case of the 
Kleene closure operator)
    std::string regex = "test: a*";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
     std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'a', '*'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Test_Kleene_2, This test tests if the regex parser can handle the Kleene closure operator and 
and concatenation)
    std::string regex = "test: ba*c";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
     std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'b', 'a', '*', '?', 'c', '?'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Test_OO, This test ensures the parser is respecting order of operations for all operators)
    std::string regex = "test: ab(cd*)|(ab)*";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
    std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'a', 'b', '?', 'c', 'd', '*', '?', '?', 'a', 'b', '?', '*', '|'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Remove_Digits, This test ensures the parser can handle [0-9])
    std::string regex = "test: a[0-9]b[0-9]";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
     std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'a', '0', '1', '|', '2', '|', '3', '|', '4', '|', '5', '|', '6', '|', '7', '|', '8', '|', '9', '|', '?', 'b', '?',
        '0', '1', '|', '2', '|', '3', '|', '4', '|', '5', '|', '6', '|', '7', '|', '8', '|', '9', '|', '?'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Remove_Lower, This test ensures the parser can handle [a-z])
    std::string regex = "test: 0[a-z]b[a-z]";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
    std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'0', 
        'a', 'b', '|', 'c', '|', 'd', '|', 'e', '|', 'f', '|', 'g', '|', 'h', '|', 'i', '|', 'j', '|', 'k', '|', 
        'l', '|', 'm', '|', 'n', '|', 'o', '|', 'p', '|', 'q', '|', 'r', '|', 's', '|', 't', '|', 'u', '|', 'v', '|', 'w',
        '|', 'x', '|', 'y', '|', 'z', '|', '?', 'b', '?', 'a', 'b', '|', 'c', '|', 'd', '|', 'e', '|', 'f', '|', 'g', '|', 'h', '|', 'i', '|', 'j', '|', 'k', '|', 
        'l', '|', 'm', '|', 'n', '|', 'o', '|', 'p', '|', 'q', '|', 'r', '|', 's', '|', 't', '|', 'u', '|', 'v', '|', 'w',
        '|', 'x', '|', 'y', '|', 'z', '|', '?'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Remove_Upper, This test ensures the parser can handle [a-z])
    std::string regex = "test: 0[A-Z]b[A-Z]";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
    std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'0', 
        'A', 'B', '|', 'C', '|', 'D', '|', 'E', '|', 'F', '|', 'G', '|', 'H', '|', 'I', '|', 'J', '|', 'K', '|', 
        'L', '|', 'M', '|', 'N', '|', 'O', '|', 'P', '|', 'Q', '|', 'R', '|', 'S', '|', 'T', '|', 'U', '|', 'V', '|', 'W',
        '|', 'X', '|', 'Y', '|', 'Z', '|', '?', 'b', '?', 'A', 'B', '|', 'C', '|', 'D', '|', 'E', '|', 'F', '|', 'G', '|', 'H', '|', 'I', '|', 'J', '|', 'K', '|', 
        'L', '|', 'M', '|', 'N', '|', 'O', '|', 'P', '|', 'Q', '|', 'R', '|', 'S', '|', 'T', '|', 'U', '|', 'V', '|', 'W',
        '|', 'X', '|', 'Y', '|', 'Z', '|', '?'}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Remove_Letters, This test ensures the parser can handle [A-Za-z])
    std::string regex = "test: 0[A-Za-z]b[A-Za-z]";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
     std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'0',
        'a', 'b', '|', 'c', '|', 'd', '|', 'e', '|', 'f', '|', 'g', '|', 'h', '|', 'i', '|', 'j', '|', 'k', '|', 
        'l', '|', 'm', '|', 'n', '|', 'o', '|', 'p', '|', 'q', '|', 'r', '|', 's', '|', 't', '|', 'u', '|', 'v', '|', 'w',
        '|', 'x', '|', 'y', '|', 'z', '|', 'A', '|', 'B', '|', 'C', '|', 'D', '|', 'E', '|', 'F', '|', 'G', '|', 'H', '|', 'I', '|', 'J', '|', 'K', '|', 
        'L', '|', 'M', '|', 'N', '|', 'O', '|', 'P', '|', 'Q', '|', 'R', '|', 'S', '|', 'T', '|', 'U', '|', 'V', '|', 'W',
        '|', 'X', '|', 'Y', '|', 'Z', '|', '?', 'b', '?', 'a', 'b', '|', 'c', '|', 'd', '|', 'e', '|', 'f', '|', 'g', '|', 'h', '|', 'i', '|', 'j', '|', 'k', '|', 
        'l', '|', 'm', '|', 'n', '|', 'o', '|', 'p', '|', 'q', '|', 'r', '|', 's', '|', 't', '|', 'u', '|', 'v', '|', 'w',
        '|', 'x', '|', 'y', '|', 'z', '|', 'A', '|', 'B', '|', 'C', '|', 'D', '|', 'E', '|', 'F', '|', 'G', '|', 'H', '|', 'I', '|', 'J', '|', 'K', '|', 
        'L', '|', 'M', '|', 'N', '|', 'O', '|', 'P', '|', 'Q', '|', 'R', '|', 'S', '|', 'T', '|', 'U', '|', 'V', '|', 'W',
        '|', 'X', '|', 'Y', '|', 'Z', '|', '?'}}

    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Remove_All, This test ensures the parser can handle [0-9] and [a-z] and [A-Z] and [A-Za-z] all in one regex)
    std::string regex = "test: a[0-9][a-z][A-Z][A-Za-z]b";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
     std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'a',
        '0', '1', '|', '2', '|', '3', '|', '4', '|', '5', '|', '6', '|', '7', '|', '8', '|', '9', '|', '?',
        'a', 'b', '|', 'c', '|', 'd', '|', 'e', '|', 'f', '|', 'g', '|', 'h', '|', 'i', '|', 'j', '|', 'k', '|', 
        'l', '|', 'm', '|', 'n', '|', 'o', '|', 'p', '|', 'q', '|', 'r', '|', 's', '|', 't', '|', 'u', '|', 'v', '|', 'w',
        '|', 'x', '|', 'y', '|', 'z', '|', '?',
        'A', 'B', '|', 'C', '|', 'D', '|', 'E', '|', 'F', '|', 'G', '|', 'H', '|', 'I', '|', 'J', '|', 'K', '|', 
        'L', '|', 'M', '|', 'N', '|', 'O', '|', 'P', '|', 'Q', '|', 'R', '|', 'S', '|', 'T', '|', 'U', '|', 'V', '|', 'W',
        '|', 'X', '|', 'Y', '|', 'Z', '|', '?',
        'a', 'b', '|', 'c', '|', 'd', '|', 'e', '|', 'f', '|', 'g', '|', 'h', '|', 'i', '|', 'j', '|', 'k', '|', 
        'l', '|', 'm', '|', 'n', '|', 'o', '|', 'p', '|', 'q', '|', 'r', '|', 's', '|', 't', '|', 'u', '|', 'v', '|', 'w',
        '|', 'x', '|', 'y', '|', 'z', '|', 'A', '|', 'B', '|', 'C', '|', 'D', '|', 'E', '|', 'F', '|', 'G', '|', 'H', '|', 'I', '|', 'J', '|', 'K', '|', 
        'L', '|', 'M', '|', 'N', '|', 'O', '|', 'P', '|', 'Q', '|', 'R', '|', 'S', '|', 'T', '|', 'U', '|', 'V', '|', 'W',
        '|', 'X', '|', 'Y', '|', 'Z', '|', '?', 'b', '?',}}

    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Escape_Characters, This test ensures the regex parser can handle escape characters)
    std::string regex = "test: a*\\*bc\\?(a|\\|)\\\\";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
     std::vector<std::pair<std::string, std::vector<char>>> expected = {{
        {"test", {'a', '*', '\\', '*', '?', 'b', '?', 'c', '?', '\\', '?', '?', 'a', '\\', '|', '|', '?', '\\', '\\', '?',}}
    }};
    auto parsed = parser.parse();
    CONTENT_CHECK(expected[0].first, parsed[0].first)
    CONTENT_CHECK(expected[0].second, parsed[0].second)
    PASS_OR_FAIL()
END_TEST()

BEGIN_TEST(Regex_Parser_Multiple_Regex, This test ensures the regex parser can handle multiple regex)
    std::string regex = "test1: ab\ntest2: ab";
    std::istringstream str_in(regex);
    final_project::regex::regex_parser parser(str_in);
     std::vector<std::pair<std::string, std::vector<char>>> expected = {
        {"test1", {'a', 'b', '?'}}, 
        {"test2", {'a', 'b', '?'}}
    };
    auto parsed = parser.parse();
    if(parsed.size() != expected.size())
        passed = false;
    else 
    {
        for(size_t i = 0; i < parsed.size(); ++i)
        {
            CONTENT_CHECK(expected[i].second, parsed[i].second);
        }
    }
    PASS_OR_FAIL()
END_TEST()

TEST_MAIN()