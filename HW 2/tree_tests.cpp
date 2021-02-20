#include "tree.h"
#include <iostream>

//Utility macros I wrote to make writing tests less tedious. 
//They handle most of the repetitive calls to std::cout so I don't have to.
//They also take care of exception handling for me, allowing me to easily 
//surround every test with a try/catch block. They also help with dynamic 
//memory allocation. 

//Define the function header for a test with the given name and create a variable to store the name.
//Prints the name of the test and a description as to what the test does and wraps code in a try block.
//Many tests uses the toarray() function so the header also declares an int* and sets it to nullptr 
//in preparation for that. 
//@param o the name of the test
//@param d a description of what the test does
#define MAKE_TEST(o, d) void test_##o(const char* test_name=#o) \
{\
std::cout<< "\nTest " << #o << std::endl;\
std::cout <<  #d << std::endl;\
try{\
    int* arr = nullptr;
//Provides a catch block to catch any unhandled excpetions and print an error. 
//Provides closing brace for test
#define END_TEST() } catch(...) {\
std::cout << "Unexpected Exception. Test "<< test_name << " Failed!" << std::endl;\
}}
//Print a failure message, delete any dynamically allocated memory, and exit
#define FAILURE() std::cout << "Test " << test_name << " FAILED!" << std::endl;\
    if(arr)\
        delete[] arr;\
    return;
//Print a failure message with an explanation, delete any dynamically allocated memory, and exit. 
//@param a the actual value 
//@param e the expected value 
//@param v the name of the value
#define FAILURE_ARGS(a, e, v) std::cout << "Test "  << test_name << " FAILED! Expected " << v << " :" << e << ", Actual: " << a << std::endl;\
    if(arr)\
        delete[] arr;\
    return;
//Print a success message, delete any dynamically allocated memory, and exit
#define SUCCESS() std::cout << "Test " << test_name << " Passed!" << std::endl;\
if(arr)\
    delete[] arr;\
return;

MAKE_TEST(size_1, Tests size on an empty tree.)
    Tree t;
    int size = t.size();
    int expected = 0;
    if (size == expected) {
        SUCCESS()
    } else  {
        FAILURE_ARGS(size, 0, "size");
    }
END_TEST()

MAKE_TEST(size_2, Tests size on a non-empty tree.)
    Tree t;
    t.insert(1);
    t.insert(2);
    int size = t.size();
    int expected = 2;
    if (size == expected) {
        SUCCESS()
    } else  {
        FAILURE_ARGS(size, expected, "size")
    }
END_TEST()

MAKE_TEST(to_array_1, Tests to array on an empty-tree.)
    Tree t;
    arr = t.toarray();
    if(!arr) 
    {
        SUCCESS()
    }
    else 
    {
        FAILURE()
    }
END_TEST()

MAKE_TEST(insert_1, Tests inserting into an empty tree.)
    Tree t;
    t.insert(1);
    arr = t.toarray();
    int size = t.size();
    if (size != 1)
    {
        FAILURE_ARGS(size, 1, "size")
    } if (arr[0] != 1) {
        FAILURE_ARGS(arr[0], 1, "arr[0]")
    }
    else {
        SUCCESS()
    }
END_TEST()

MAKE_TEST(insert_2, Tests inserting into a non-empty tree.)
    Tree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    int size = t.size();
    arr = t.toarray();
    if(size != 3)
    {
        FAILURE_ARGS(size, 3, "size")
    }
    for (int i = 0; i < size; ++i)
    {
        if(arr[i] != i + 1)
        {
            const char* desc = "arr[" + ('1' + 1) + ']';
            FAILURE_ARGS(i + 1, arr[i], desc);
        }
    }
    SUCCESS()
END_TEST()

MAKE_TEST(insert_3, Test inserting into a non-empty tree.)
    Tree t;
    t.insert(3);
    t.insert(2);
    t.insert(5);
    t.insert(1);
    t.insert(4);
    arr = t.toarray();
    int size = t.size();
    if(size != 5)
    {
        FAILURE_ARGS(size, 5, "size")
    }
    for (int i = 0; i < size; ++i)
    {
        if(arr[i] != i + 1)
        {
            const char* desc = "arr[" + ('1' + 1) + ']';
            FAILURE_ARGS(i + 1, arr[i], desc);
        }
    }
    SUCCESS()
END_TEST()

MAKE_TEST(insert_4, Tests trying to insert a duplicate value into the tree.)
    Tree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.insert(2);
    int size = t.size();
    if(size != 3) 
    {
        FAILURE_ARGS(size, 4, "size")
    } else 
    {
        SUCCESS()
    }   
END_TEST()

MAKE_TEST(erase_1, Tests trying to erase from an empty tree)
    Tree t;
    t.erase(0);
    if(t.size() != 0)
    {
        FAILURE_ARGS(t.size(), 0, "size")
    }
    else
    {
        SUCCESS()
    }
END_TEST()

MAKE_TEST(erase_2, Tests erasing the root with no subtrees.)
    Tree t;
    t.insert(1);
    t.erase(1);
    int size = t.size();
    arr = t.toarray();
    if(arr)
    {
        FAILURE()
    } 
    if (size != 0)
    {
        FAILURE_ARGS(size, 0, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(erase_3, Tests erasing the root with a left subtree.)
    Tree t;
    t.insert(2);
    t.insert(1);
    t.erase(2);
    int size = t.size();
    arr = t.toarray();
    if(size != 1)
    {
        FAILURE_ARGS(size, 1, "size")
    }
    if (arr[0] != 1)
    {
        FAILURE_ARGS(arr[0], 1, "arr[0]")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(erase_4, Tests erasing the root with a right subtree)
    Tree t;
    t.insert(2);
    t.insert(3);
    t.erase(2);
    int size = t.size();
    arr = t.toarray();
    if(size != 1)
    {
        FAILURE_ARGS(size, 1, "size")
    }
    if (arr[0] != 3)
    {
        FAILURE_ARGS(arr[0], 3, "arr[0]")
    } 
    SUCCESS()  
END_TEST()

MAKE_TEST(erase_5, Tests erasing the root with two subtrees.)
    Tree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.erase(2);
END_TEST()

int main(int argc, char** argv)
{
    test_size_1();
    test_size_2();
    test_to_array_1();
    test_insert_1();
    test_insert_2();
    test_insert_3();
    test_insert_4();
    test_erase_1();
    test_erase_2();
    test_erase_3();
    test_erase_4();
    test_erase_5();
}