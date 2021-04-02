#include <iostream> 
#include <cmath>


#define MAKE_ITER_TEST(num, ...)\
    std::cout << "Begin Seth Sort Iter Test " << #num << std::endl;\
    int arr_iter##num[] = {__VA_ARGS__};\
    int size_iter##num = sizeof(arr_iter##num)/sizeof(arr_iter##num[0]);\
    seth_sort(arr_iter##num, size_iter##num);\
    print(arr_iter##num, size_iter##num);\
    std::cout << std::endl;\

#define MAKE_REC_TEST(num, ...)\
    std::cout << "Begin Seth Sort Rec Test " << #num << std::endl;\
    int arr_rec##num[] = {__VA_ARGS__};\
    int size_rec##num = sizeof(arr_rec##num)/sizeof(arr_rec##num[0]);\
    seth_sort_rec(arr_rec##num, size_rec##num);\
    print(arr_rec##num, size_rec##num);\
    std::cout << std::endl;\

void seth_sort(int a[], int n)
{
    for(int i = 0; i < n; ++i)
    {
        int min = a[i];
        int min_index = i;
        for (int j = i; j < n; ++j)
        {
            if (a[j] < min)
            {
                min = a[j];
                min_index = j;
            }
        }
        int temp = a[i];
        a[i] = a[min_index];
        a[min_index] = temp;
    }
}

void find_min_rec(int a[], int n, int j, int& min, int& min_index)
{
    if (j == n)
        return;
    if (a[j] < min)
    {
        min = a[j];
        min_index = j;
    }
    j += 1;
    find_min_rec(a, n, j, min, min_index);
}

void seth_sort_rec_impl(int a[], int i, int n)
{
    if (i == n)
        return;
    int min = a[i];
    int min_index = i;
    find_min_rec(a, n, i, min, min_index);
    int temp = a[i];
    a[i] = a[min_index];
    a[min_index] = temp;
    seth_sort_rec_impl(a, i + 1, n);
}

void seth_sort_rec(int a[], int n)
{
    seth_sort_rec_impl(a, 0, n);
}

void print(int arr[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        std::cout << arr[i] << " ";
    }
}

int main()
{
    MAKE_ITER_TEST(1, 1, -6, 5, 3, 2)
    MAKE_ITER_TEST(2, 1)
    MAKE_ITER_TEST(3, 1, 2, 3)
    MAKE_ITER_TEST(4, 1, 1, 1)
    std::cout << std::endl;
    MAKE_REC_TEST(1, 1, -6, 5, 3, 2)
    MAKE_REC_TEST(2, 1)
    MAKE_REC_TEST(3, 1, 2, 3)
    MAKE_REC_TEST(4, 1, 1, 1)
    return 0;
}