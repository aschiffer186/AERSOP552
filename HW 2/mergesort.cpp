#include<iostream>
#include <cmath>

void swap(int& i, int& j)
{
    int temp = i;
    i = j;
    j = temp;
}

void merge(int* arr, int begin1, int begin2, int end)
{
   for(; begin2 != end;)
   {
        if(begin1 == begin2)
            break;
        if(arr[begin1] > arr[begin2])
        {
            swap(arr[begin1], arr[begin2]);
            ++begin1;
            ++begin2;
        } else {
            ++begin1;
        }
   }
}

void mergesort_helper(int* arr, int begin, int end)
{
    if(end < begin + 2)
        return;
    int middle = begin + (end-begin)/2;
    mergesort_helper(arr, begin, middle);
    mergesort_helper(arr, middle, end);
    merge(arr, begin, middle, end);
}

int* mergesort(int* arr, int n)
{
    mergesort_helper(arr, 0, n);
    return arr;
}

int main (int argc, char** argv)
{
    int arr[1] ={1};
    int n = 0;
    int* sorted = mergesort(arr, n);

    for(int i = 0; i < n; ++i)
    {
        std::cout << sorted[i] << " ";
    }
    std::cout << "Success" << std::endl;
}