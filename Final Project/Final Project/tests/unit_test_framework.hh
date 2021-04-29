#ifndef UNIT_TEST_FRAMEWORK_H
#define UNIT_TEST_FRAMEWORK_H 1


//A unit test framework that simplify the creation 
//of unit tests. 

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <functional>

typedef std::map<std::string, int> testing_map;
typedef std::function<void()> test_t;
typedef std::vector<test_t> test_list_t;

#define GRN "\e[0;32m"
#define NC "\e[0m"
#define RED "\e[0;31m"

//Creates the necessary variables to track 
//the results of the units tests. Creates a 
//map to keep track of which tests succeed and fail 
//as well as two variables to keep track of the number 
//of testds that have passed and failed
#define TESTING_SETUP()\
    testing_map test_results;\
    int num_passed = 0;\
    int num_failed = 0;\
    test_list_t tests;\
    template<typename _TestTp>\
    struct test_listener\
    {\
        test_listener(_TestTp test)\
        {\
            tests.push_back(test);\
        }\
    };

//Creates a new unit test with the specifed name and description. 
//Prints the test name and description then surrounds then creates a 
//try block to surround the unit test. 
//
//@param name the name of the testd 
//@param desc the description of the test
#define BEGIN_TEST(name, desc)\
    void test_##name();\
    test_listener<test_t> register_##name(&test_##name);\
    void test_##name(){\
        const char* test_name = #name;\
        int passed = 0;\
        std::cout << "Begin " << test_name << std::endl;\
        std::cout << "Test description: " << #desc << std::endl;\
        test_results[test_name] = 0;\
        try{\


//Provides clean up code to end a unit test. Creates two catch blocks: one to 
//handle exceptions that descend from std::exception and one to handle other 
//exceptions. 
//
//When an exception descended from std::exception is encountered, prints the 
//exception's message. Otherwwise indicates that an unknown exception was caught. 
#define END_TEST()\
        } catch (const std::exception& ex) {\
            std::cout << "Exception during test " << test_name << std::endl;\
            std::cout << "Exception message: " << ex.what() << std::endl;\
            std::cout << test_name << " failed!" << std::endl;\
            test_results[test_name] = -1;\
        } catch (...) {\
            std::cout << "Unknown exception thrown during test: " << test_name << std::endl;\
            std::cout << test_name << " failed!" << std::endl;\
            ++num_failed;\
            test_results[test_name] = -1;\
        }\
        std::cout << std::endl;\
    }\

//Checks to see if the unit test has passed or failed and prints a message 
//accordingly. 
#define PASS_OR_FAIL()\
    if(passed == 1){\
        std::cout << test_name << " passed!" << std::endl;\
        ++num_passed;\
        test_results[test_name] = 1;\
    } else if (passed == -1) {\
        std::cout << test_name << " failed!" << std::endl;\
        ++num_failed;\
        test_results[test_name] = -1;\
    }

//Compares the contents of two containters to see if they match. 
//Will work with any object that has a .size() method and an overloaded [] operator. 
//If the size of the container containing the actual results differs from the expected size, 
//prints an error. If the contents of the container differ from the expected contents, prints 
//an error message the contains information about what index/indices differ and what the 
//expected value is. 
//
//@param expected a container containing the expected results 
//@param actual a container containing the actual results
#define CONTENT_CHECK(expected, actual)\
    if(expected.size() != actual.size()){\
        std::cout << test_name << ". Expected size: " << expected.size() << ", actual size: " << actual.size() << std::endl;\
        passed = -1;\
    }\
    for(size_t i = 0; i < std::min(expected.size(), actual.size()); ++i)\
    {\
        if(expected[i] != actual[i])\
        {\
            std::cout << test_name << ". At position " << i << " expected " << expected[i] << ", found " << actual[i] << std::endl;\
            passed = -1;\
        }\
    }

//Invokes all tests then prints information about which tests succeeded. 
//First, this macro prints. the number of tests that succeeded and failed. 
//Then the maco prints whether each test passed or failed. 
//
//Generates a main function for the file with the test cases.
#define TEST_MAIN()\
        int main(int argc, char** argv){\
        for(auto& test: tests)\
            test();\
        std::cout << "\n**Test Sumary**" << std::endl;\
        std::cout << num_passed << " tests pased." << std::endl;\
        std::cout << num_failed << " tests failed." << std::endl;\
        std::cout << "\nInvidual Breakdown" << std::endl;\
        for(const auto& test: test_results)\
        {\
            int result = test.second;\
            if (result == 1)\
                std::cout << test.first << ": " << GRN << "pased" << NC << std::endl;\
            else if (result == -1)\
                std::cout << test.first << ": " << RED << "failed" << NC << std::endl;\
            else\
                std::cout << test.first << ": " << "completed, check manually" << std::endl;\
        }\
        if(num_failed > 0)\
            return EXIT_FAILURE;\
        return 0;\
    }
   

#endif