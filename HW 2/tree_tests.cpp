#include "tree.h"
#include <iostream>

#define TEST_NAME(o) std::cout << "\nTest " << o << std::endl;
#define FAILURE(o) std::cout << "Test " << o << " FAILED!" << std::endl;
#define FAILURE_ARGS(o, a, e, v) std::cout << "Test "  << o << " FAILED! Expected " << v << ":" << e << ", Actual: " << a << std::endl;
#define SUCCESS(o) std::cout << "Test " << o << " Passed!" << std::endl;
#define ERROR(o) std::cout << "Unexpected Exception. Test " << o << " FAILED!" << std::endl;

void test_size_empty()
{
    const char* test_name = "Size Empty";
    TEST_NAME(test_name)
    try {
        Tree t;
        int size = t.size();
        int expected = 0;
        if (size == expected) {
           SUCCESS(test_name)
        } else  {
           FAILURE_ARGS(test_name, size, 0, "size");
        }
    } catch (...) {
       ERROR(test_name)
    }
}

void test_size_nonempty()
{
    const char* test_name = "Size Non-Empty";
    TEST_NAME(test_name)
    try {
        Tree t;
        t.insert(1);
        t.insert(2);
        int size = t.size();
        int expected = 2;
        if (size == expected) {
            SUCCESS(test_name)
        } else  {
           FAILURE_ARGS(test_name, size, expected, "size")
        }
    } catch (...) {
        ERROR(test_name)
    }
}

void test_toarray_empty()
{
    const char* test_name = "ToArray Empty";
    TEST_NAME(test_name)
    try 
    {
        Tree t;
        int* arr = t.toarray();
        if(!arr) 
        {
            SUCCESS(test_name);
        }
        else {
            FAILURE(test_name);
        }
    } catch (...) {
        ERROR(test_name)
    }
}

void test_insert_1()
{
    const char* test_name = "Insert 1";
    TEST_NAME(test_name)
    try {
        Tree t;
        t.insert(1);
        int* arr = t.toarray();
        int size = t.size();
        if (size != 1){
            FAILURE_ARGS(test_name, size, 1, "size")
            delete[] arr;
            return;
        } if (arr[0] != 1) {
            FAILURE_ARGS(test_name, arr[0], 1, "arr[0]")
            delete[] arr;
            return;
        }
        else {
            SUCCESS(test_name)
            delete[] arr;
        }
    } catch (...) {
        ERROR(test_name)
    }
}

void test_insert_2()
{
    const char* test_name = "Insert 2";
    TEST_NAME(test_name)
    try 
    {
        Tree t;
        t.insert(2);
        t.insert(1);
        t.insert(3);
        int* arr = t.toarray();
        int size = t.size();
        if(size != 3)
        {
            FAILURE_ARGS(test_name, size, 3, "size")
            delete[] arr;
            return;
        }
        for (int i = 0; i < size; ++i)
        {
            if(arr[i] != i + 1)
            {
                const char* desc = "arr[" + ('1' + 1) + ']';
                FAILURE_ARGS(test_name, i + 1, arr[i], desc);
                delete[] arr;
                return;
            }
        }
        SUCCESS(test_name)
        delete[] arr;
    } catch (...) {
        ERROR(test_name)
    }
}

void test_insert_3()
{
    const char* test_name = "Insert 3";
    TEST_NAME(test_name)
    try 
    {
        Tree t;
        t.insert(3);
        t.insert(2);
        t.insert(5);
        t.insert(1);
        t.insert(4);
        int* arr = t.toarray();
        int size = t.size();
        if(size != 5)
        {
            FAILURE_ARGS(test_name, size, 5, "size")
            delete[] arr;
            return;
        }
        for (int i = 0; i < size; ++i)
        {
            if(arr[i] != i + 1)
            {
                const char* desc = "arr[" + ('1' + 1) + ']';
                FAILURE_ARGS(test_name, i + 1, arr[i], desc);
                delete[] arr;
                return;
            }
        }
        SUCCESS(test_name)
        delete[] arr;
    } catch (...) {
        ERROR(test_name)
    }
}

int main(int argc, char** argv)
{
    test_size_empty();
    test_size_nonempty();
    test_toarray_empty();
    test_insert_1();
    test_insert_2();
    test_insert_3();
}