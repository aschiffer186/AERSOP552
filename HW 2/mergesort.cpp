#include<iostream>

//Swap two integers
void swap(int* i, int* j)
{
    int temp = *i;
    *i = *j;
    *j = temp;
}

//Merges two sorted ranges. The merge is performed in-place 
//requiring no extra memory.
//Time Complexity: O(n)
//Space Complexity: O(1)
//@param arr the array containing the data to be sorted 
//@param begin1 the first index of the first sorted range
//@param begin2 the first index of the second sorted range
//@param end the last index of the second sorted range + 1
void merge(int* arr, int* begin1, int* begin2, int* end)
{
  for(; begin1 != begin2; )
  {
    if (begin2 == end)
        return;
    if (*begin1 > *begin2)
    {
        swap(begin1, begin2);
        if (begin2 == begin1 + 1) 
            ++begin2;
        else
            ++begin1;
    } 
    else 
    {
        ++begin1;
    }
  }
}

//Recursive implementation of merge sort. Calls mergesort 
//on the left half of the array and the right half of the array 
//then merges the two sorted halves. 
//@param arr the array to be sorted 
//@param begin the first index of the range to be sorted 
//@param end the last index of the range to be sorted + 1
void mergesort_helper(int* arr, int begin, int end)
{
    if(end < begin + 2)
        return;
    int middle = begin + (end-begin)/2;
    mergesort_helper(arr, begin, middle);
    mergesort_helper(arr, middle, end);
    merge(arr, arr + begin, arr + middle, arr + end);
}

//Performs merge sort on the specified array. 
//Modifies the array passed to the function.
//Time Complexity: O(nlog(n)) 
//Space Complexity: O(log(n))
//@param arr the array to be sorted 
//@param n the size of the array 
//@return the sorted array
int* mergesort(int* arr, int n)
{
    mergesort_helper(arr, 0, n);
    return arr;
}

int main (int argc, char** argv)
{
    int arr[] = {6, 1, 21, -7, 2, 8, 10, -6, 9, 10};
    //Test sorting basic array 
    int size = 10;
    int* sorted = mergesort(arr, size);
    for(int i = 0; i < size; ++i)
    {
        std::cout << sorted[i] << " ";
    }

    std::cout << std::endl << std::endl;
    //Test already sorted array 
    int arr6[] = {1, 2, 3, 4, 5, 6, 7,  8, 9, 10};
    size = 10;
    sorted = mergesort(arr6, size);
    for(int i = 0; i < size; ++i)
    {
        std::cout << sorted[i] << " ";
    }

    std::cout << std::endl << std::endl;

    //Test sorting array with duplicates
    int arr2[] = {6, 1, 1, 3, 7};
    size = 5;
    sorted = mergesort(arr2, size);
    for(int i = 0; i < size; ++i)
    {
       std::cout << sorted[i] << " ";
    }

    std::cout << std::endl << std::endl;

    //Test sorting array with 2 elements 
    int arr5[] = {6, 1};
    size = sizeof(arr5)/sizeof(arr5[0]);
    sorted = mergesort(arr5, size);
    for(int i = 0; i < size; ++i)
    {
       std::cout << sorted[i] << " ";
    }

    std::cout << std::endl << std::endl;

    //Test sorting an array with 1 element 
    int arr3[] = {6};
    size = 1;
    sorted = mergesort(arr3, size);
    for(int i = 0; i < size; ++i)
    {
       std::cout << sorted[i] << " ";
    }

    std::cout << std::endl << std::endl;

    //Test sorting an empty array 
    int arr4[] = {6};
    size = 0;
    sorted = mergesort(arr4, size);

    //These are cases that caused trouble when stepping through merge algorithm by hand
    //Test edge case 1
    int edge_1[] = {1, 3, 5, 7, 2, 4, 6, 8,};
    size = sizeof(edge_1)/sizeof(edge_1[0]);
    sorted = mergesort(edge_1, size);
    for(int i = 0; i < size; ++i)
    {
       std::cout << sorted[i] << " ";
    }

    std::cout << std::endl << std::endl;

    //Test edge case 2
    int edge_2[] = {1, 3, 6, 8, 2, 4, 5, 7};
    size = sizeof(edge_2)/sizeof(edge_2[0]);
    sorted = mergesort(edge_2, size);
    for(int i = 0; i < size; ++i)
    {
       std::cout << sorted[i] << " ";
    }

    std::cout << std::endl << std::endl;

    //Test edge case 3
    int edge_3[] = {1, 3, 5, 7, 2, 4, 9, 10, 13};
    size = sizeof(edge_3)/sizeof(edge_3[0]);
    sorted = mergesort(edge_3, size);
    for(int i = 0; i < size; ++i)
    {
       std::cout << sorted[i] << " ";
    }

    std::cout << std::endl << std::endl;
    
    std::cout << "All tests completed" << std::endl;
}