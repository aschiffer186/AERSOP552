#include "tree.h"
#include <iostream>

//Utility macros
//They handle most of the repetitive calls to std::cout.
//They also take care of exception handling, surrounding every test with a try/catch block. 
//They also help with dynamic memory allocation. 

//Define the function header for a test with the given name and create a variable to store the name.
//Prints the name of the test and a description as to what the test does and wraps code in a try block. 
//@param o the name of the test
//@param d a description of what the test does
#define MAKE_TEST(o, d) void test_##o(const char* test_name=#o) \
{\
std::cout<< "\nTest " << #o << std::endl;\
std::cout <<  #d << std::endl;\
try{\
//Provides a catch block to catch any unhandled excpetions and print an error. 
//Provides closing brace for test
#define END_TEST() } catch(...) {\
std::cout << "Unexpected Exception. Test "<< test_name << " Failed!" << std::endl;\
}}
//Print a basic failure message
#define FAILURE() std::cout << "Test " << test_name << " FAILED!" << std::endl;\
                  return;
//Print a failure message with an explanation, delete any dynamically allocated memory, and exit. 
//@param a the actual value 
//@param e the expected value 
//@param v the name of the value
#define FAILURE_ARGS(a, e, v) std::cout << "Test "  << test_name << " FAILED! Expected " << v << ": " << e << ", Actual: " << a << std::endl;\
                              return;
//Print a success message, delete any dynamically allocated memory, and exit
#define SUCCESS() std::cout << "Test " << test_name << " Passed!" << std::endl;\
                  return;
//Compares the contents of the tree (as obtained by toarray()) with the specified array.
//Prints an error if they do not match. Cleans up any allocated memory
//@param o an array containing the expected contents of the tree
#define CONTENTS_TEST(tree, o) int* arr = tree.toarray();\
int tree_size = tree.size();\
for(int i = 0; i < tree_size; ++i)\
{\
    if(o[i] != arr[i]) {\
        const char* desc = "arr[" + ('0' + i) + ']';\
        FAILURE_ARGS(arr[i], o[i], desc);\
    }\
}\
delete[] arr;

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
    int* arr = t.toarray();
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
    int expected[] = {1};
    int size = t.size();
    if (size != 1)
    {
        FAILURE_ARGS(size, 1, "size")
    } 
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(insert_2, Tests inserting into a non-empty tree.)
    Tree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    int size = t.size();
    int expected[] = {1, 2, 3};
    if(size != 3)
    {
        FAILURE_ARGS(size, 3, "size")
    }
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(insert_3, Test inserting into a non-empty tree.)
    Tree t;
    t.insert(3);
    t.insert(2);
    t.insert(5);
    t.insert(1);
    t.insert(4);
    int expected[] = {1, 2, 3, 4, 5};
    int size = t.size();
    if(size != 5)
    {
        FAILURE_ARGS(size, 5, "size")
    }
    CONTENTS_TEST(t, expected)
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
    int* arr = t.toarray();
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
    int expected[] = {1};
    if(size != 1)
    {
        FAILURE_ARGS(size, 1, "size")
    }
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(erase_4, Tests erasing the root with a right subtree)
    Tree t;
    t.insert(2);
    t.insert(3);
    t.erase(2);
    int size = t.size();
    int expected[] = {3};
    if(size != 1)
    {
        FAILURE_ARGS(size, 1, "size")
    }
    CONTENTS_TEST(t, expected)
    SUCCESS()  
END_TEST()

MAKE_TEST(erase_5, Tests erasing the root with two subtrees.)
    Tree t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.erase(2);
    int size = t.size();
    int expected[] = {1,3};
    if(size != 2)
    {
        FAILURE_ARGS(size, 2, "size")
    }
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(erase_6, Tests trying to erase an element not in the tree.)
    Tree t;
    t.insert(1);
    t.insert(2);
    t.insert(3);
    t.erase(4);
    int size = t.size();
    int expected[] = {1, 2, 3};
    if(size != 3)
    {
        FAILURE_ARGS(size, 3, "size")
    }
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(erase_7, Tests trying to erase non-root leaf element)
    Tree t;
    t.insert(4);
    t.insert(3);
    t.insert(1);
    t.insert(2);
    t.insert(6);
    t.insert(5);
    t.insert(7);
    t.erase(7);
    int size = t.size();
    int expected[] = {1, 2, 3, 4, 5, 6};
    if(size != 6)
    {
        FAILURE_ARGS(size, 6, "size")
    }
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(erase_8, Test erasing non root element with only right child)
    Tree t;
    t.insert(4);
    t.insert(3);
    t.insert(1);
    t.insert(2);
    t.insert(6);
    t.insert(5);
    t.erase(6);
    int size = t.size();
    int expected2[] = {1, 2, 3, 4, 5};
    if (size != 5)
    {
        FAILURE_ARGS(size, 5, "size")
    }
    CONTENTS_TEST(t, expected2)
    SUCCESS()
END_TEST()

MAKE_TEST(erase_9, Test erasing non-root element with with two children)
    Tree t;
    t.insert(4);
    t.insert(3);
    t.insert(1);
    t.insert(2);
    t.insert(6);
    t.insert(5);
    t.erase(3);
    int size = t.size();
    int expected2[] = {1, 2, 4, 5, 6};
    if (size != 5)
    {
        FAILURE_ARGS(size, 5, "size")
    }
    CONTENTS_TEST(t, expected2)
    SUCCESS()
END_TEST()

MAKE_TEST(erase_10, Test erasing non-root element with only left child)
    Tree t;
    t.insert(4);
    t.insert(3);
    t.insert(1);
    t.insert(2);
    t.insert(6);
    t.insert(7);
    t.erase(6);
    int size = t.size();
    int expected2[] = {1, 2, 3, 4, 7};
    if (size != 5)
    {
        FAILURE_ARGS(size, 5, "size")
    }
    CONTENTS_TEST(t, expected2)
    SUCCESS()
END_TEST()

MAKE_TEST(member_1, Test member on empty tree)
    Tree t;
    bool found = t.member(1);
    if(found)
    {
        std::cout << std::boolalpha;
        FAILURE_ARGS(found, "false", "member")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(member_2, Test member on finding element in tree)
    Tree t;
    t.insert(4);
    t.insert(3);
    t.insert(1);
    t.insert(2);
    t.insert(6);
    t.insert(7);
    bool found = t.member(6);
    if(!found)
    {
        std::cout << std::boolalpha;
        FAILURE_ARGS(found, "true", "member")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(member_3, Test member on finding element not in tree)
    Tree t;
    t.insert(4);
    t.insert(3);
    t.insert(1);
    t.insert(2);
    t.insert(6);
    t.insert(7);
    bool found = t.member(11);
    if(found)
    {
        std::cout << std::boolalpha;
        FAILURE_ARGS(found, "false", "member")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(copy_ctor_1, Test copy constructor with an empty tree)
    Tree t;
    Tree t2(t);
    int size = t2.size();
    if(size != 0)
    {
        FAILURE_ARGS(size, 0, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(copy_ctor_2, Test copy constructor with non-empty tree)
    Tree t2;
    t2.insert(4);
    t2.insert(3);
    t2.insert(1);
    t2.insert(2);
    t2.insert(6);
    t2.insert(7);
    Tree t(t2);
    int size = t.size();
    int expected_size = t2.size();
    if(size != expected_size)
    {
        FAILURE_ARGS(size, expected_size, "size")
    }
    int expected[] = {1, 2, 3, 4, 6, 7};
    CONTENTS_TEST(t, expected)
    //Test for deep copy 
    t2.insert(9);
    size = t.size();
    if(size != expected_size)
    {
        FAILURE_ARGS(size, expected_size, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(assignment_operator_1, Test assigning non-empty tree into empty tree)
    Tree t1;
    t1.insert(1);
    t1.insert(2);
    Tree t2 = t1;
    int size = t2.size();
    int expected_size = t1.size();
    if(size != expected_size)
    {
        FAILURE_ARGS(size, expected_size, "size")
    }
    int expected[] = {1, 2};
    CONTENTS_TEST(t2, expected)
    //Test for deep copy
    t2.insert(3);
    size = t1.size();
    if(size != expected_size)
    {
        FAILURE_ARGS(size, expected_size, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(assignment_operator_2, Test assigning non-empty tree into non-empty tree)
    Tree t1;
    t1.insert(1);
    t1.insert(2);
    Tree t2;
    t2.insert(3);
    t2.insert(4);
    t2.insert(5);
    t2 = t1;
    int size = t2.size();
    int expected_size = t1.size();
    if(size != expected_size)
    {
        FAILURE_ARGS(size, expected_size, "size")
    }
    int expected[] = {1, 2};
    CONTENTS_TEST(t2, expected)
    //Test for deep copy
    t2.insert(3);
    size = t1.size();
    if(size != expected_size)
    {
        FAILURE_ARGS(size, expected_size, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(assignment_operator_3, Test assigning empty tree into non-empty tree)
    Tree t1;
    Tree t2;
    t2.insert(1);
    t2.insert(-6);
    t2 = t1;
    int size = t2.size();
    int expected_size = t1.size();
    if(size != expected_size)
    {
        FAILURE_ARGS(size, expected_size, "size")
    }
    int* arr = t2.toarray();
    if(arr)
    {
        FAILURE_ARGS("not nullptr", "nullptr", "t2.toarray()")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(assignment_operator_4, Test self-assignment)
    Tree t;
    t.insert(1);
    t = t;
    int size = t.size();
    if(size != 1)
    {
        FAILURE_ARGS(size, 1, "size")
    }
    int expected[] = {1};
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(tree_union_1, Test treeUnion with two empty trees)
    Tree a;
    Tree b;
    Tree t = treeUnion(a, b);
    int size = t.size();
    if(size != 0)
    {
        FAILURE_ARGS(size, 0, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(tree_union_2, Test treeUnion with one empty tree)
    Tree a;
    a.insert(1);
    a.insert(2);
    Tree b;
    Tree t = treeUnion(a, b);
    int size = t.size();
    if(size != 2)
    {
        FAILURE_ARGS(size, 2, "size")
    }
    int expected[] = {1, 2};
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(tree_union_3, Test treeUnion with two non-empty trees)
    Tree a;
    a.insert(1);
    a.insert(2);
    Tree b;
    b.insert(2);
    b.insert(3);
    Tree t = treeUnion(a, b);
    int size = t.size();
    if(size != 3)
    {
        FAILURE_ARGS(size, 3, "size")
    }
    int expected[] = {1, 2, 3};
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(tree_intersection_1, Test interesection with two empty trees)
    Tree a;
    Tree b;
    Tree t = intersection(a, b);
    int size = t.size();
    if(size != 0)
    {
        FAILURE_ARGS(size, 0, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(tree_intersection_2, Test intersection with one empty tree)
    Tree a;
    a.insert(1);
    a.insert(2);
    Tree b;
    Tree t = intersection(a, b);
    int size = t.size();
    if(size != 0)
    {
        FAILURE_ARGS(size, 0, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(tree_intersection_3, Test intersection with two non-empty trees of equal size)
    Tree a;
    a.insert(1);
    a.insert(2);
    Tree b;
    b.insert(2);
    b.insert(3);
    Tree t = intersection(a, b);
    int size = t.size();
    if(size != 1)
    {
        FAILURE_ARGS(size, 1, "size")
    }
    int expected[] = {2};
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(tree_intersection_4, Test intersection with trees of different lengths - 1)
    Tree a;
    a.insert(1);
    a.insert(2);
    a.insert(3);
    Tree b;
    b.insert(2);
    b.insert(3);
    Tree t = intersection(a, b);
    int size = t.size();
    if(size != 2)
    {
        FAILURE_ARGS(size, 2, "size")
    }
    int expected[] = {2, 3};
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(tree_intersection_5, Test intersection with trees of different lengths - 2)
    Tree a;
    a.insert(1);
    a.insert(2);
    a.insert(3);
    Tree b;
    b.insert(2);
    b.insert(3);
    Tree t = intersection(b, a);
    int size = t.size();
    if(size != 2)
    {
        FAILURE_ARGS(size, 2, "size")
    }
    int expected[] = {2, 3};
    CONTENTS_TEST(t, expected)
    SUCCESS()
END_TEST()

MAKE_TEST(tree_intersection_6, Test intersection with two disjoint trees of equal length)
    Tree a;
    a.insert(1);
    a.insert(2);
    Tree b;
    b.insert(4);
    b.insert(3);
    Tree t = intersection(a, b);
    int size = t.size();
    if(size != 0)
    {
        FAILURE_ARGS(size, 1, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(tree_intersection_7, Test intersection with two disjoint trees of unequal length - 1)
    Tree a;
    a.insert(1);
    a.insert(2);
    a.insert(9);
    Tree b;
    b.insert(4);
    b.insert(3);
    Tree t = intersection(a, b);
    int size = t.size();
    if(size != 0)
    {
        FAILURE_ARGS(size, 0, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(tree_intersection_8, Test intersection with two disjoint trees of unequal length - 2)
    Tree a;
    a.insert(1);
    a.insert(2);
    Tree b;
    b.insert(4);
    b.insert(3);
    b.insert(9);
    Tree t = intersection(a, b);
    int size = t.size();
    if(size != 0)
    {
        FAILURE_ARGS(size, 0, "size")
    }
    SUCCESS()
END_TEST()

MAKE_TEST(tree_print_nodes_1, Test operator << on empty tree)
    Tree t;
    std::cout << "Expected: <blank>" << std::endl;
    std::cout << "Actual: " << t << std::endl;
END_TEST()

MAKE_TEST(tree_print_nodes_2, Test operator << on non-empty tree)
    Tree t;
    t.insert(1);
    t.insert(2);
    t.insert(4);
    t.insert(3);
    t.insert(9);
    std::cout << "Expected: 1 2 3 4 9" << std::endl;
    std::cout << "Actual: " << t << std::endl;
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
    test_erase_6();
    test_erase_7();
    test_erase_8();
    test_erase_9();
    test_erase_10();
    test_member_1();
    test_member_2();
    test_member_3();
    test_copy_ctor_1();
    test_copy_ctor_2();
    test_assignment_operator_1();
    test_assignment_operator_2();
    test_assignment_operator_3();
    test_assignment_operator_4();
    test_tree_union_1();
    test_tree_union_2();
    test_tree_union_3();
    test_tree_intersection_1();
    test_tree_intersection_2();
    test_tree_intersection_3();
    test_tree_intersection_4();
    test_tree_intersection_5();
    test_tree_intersection_6();
    test_tree_intersection_7();
    test_tree_intersection_8();
    test_tree_print_nodes_1();
    test_tree_print_nodes_2();
}