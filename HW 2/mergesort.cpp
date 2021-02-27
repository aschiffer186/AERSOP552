#include<iostream>

//Swap two integers
void swap(int& i, int& j)
{
    int temp = i;
    i = j;
    j = temp;
}

//Merges two sorted ranges. The merge is performed in-place 
//requiring no extra memory.
//Time Complexity: O(n)
//Space Complexity: O(1)
//@param arr the array containing the data to be sorted 
//@param begin1 the first index of the first sorted range
//@param begin2 the first index of the second sorted range
//@param end the last index of the second sorted range + 1
void merge(int* arr, int begin1, int begin2, int end)
{
    //If we have reached the end of the second sorted range, exit
   for(; begin2 != end;)
   {    //If begin1 == begin2, we have reached the end of the sorted range
        if(begin1 == begin2)
            break;
        //An element is out of order, swap them and advance both begin1 and begin2
        if(arr[begin1] > arr[begin2])
        {
            swap(arr[begin1], arr[begin2]);
            ++begin1;
            ++begin2;
        } else { //Always advance begin1
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
    merge(arr, begin, middle, end);
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
    int* sorted = mergesort(arr, size);
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
    size = 2;
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
    std::cout << "All tests completed" << std::endl;
}