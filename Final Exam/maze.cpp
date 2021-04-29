#include "util.h"
#include <cmath>

bool operator==(const point& lhs, const point& rhs)
{
    return lhs.row == rhs.row && lhs.col == rhs.col;
}

bool operator!=(const point& lhs, const point& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const point& p)
{
    os << "(" << p.row << ", " << p.col << ")";
    return os;
}

void swap(double* i, double* j)
{
    double temp = *i;
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
void merge(double* arr, double* begin1, double* begin2, double* end)
{
  for(; begin1 != begin2; )
  {
    if (begin2 == end)
        return;
    if (*begin1 < *begin2)
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
void mergesort_helper(double* arr, int begin, int end)
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
double* mergesort(double* arr, int n)
{
    mergesort_helper(arr, 0, n);
    return arr;
}

maze::maze()
    : _M_map{}
{

}

maze::maze(const Vector<Vector<bool>>& map)
{
    for(size_t i = 0; i < map.size(); ++i)
    {
        Vector<int> m;
        for(size_t j = 0; j < map[i].size(); ++j)
        {
            if(!map[i][j])
                m.push_back(-1);
            else
                m.push_back(0);
        }
        _M_map.push_back(m);
    }
}

size_t maze::num_rows() const 
{
    return _M_map.size();
}

size_t maze::num_cols() const 
{
    if(num_rows() == 0) 
        return 0;
    return _M_map[0].size();
}

Vector<point> maze::solve_DFS()
{
    //No entry into maze
    if(_M_map[0][0] == -1 || num_rows() == 0 || num_cols() == 0)
        return Vector<point>();
    if(_M_map[0][0] != 0)
        reset();
    Vector<point> path;
    solve_DFS(0, 0, path);
    point exit = {num_rows() - 1, num_cols() - 1};
    //No path through maze
    if (path.is_empty() || path.front() != exit)
        return Vector<point>();
    return path;
}

Vector<point> maze::solve_a_star()
{
    if(_M_map[0][0] == -1 || num_rows() == 0 || num_cols() == 0)
        return Vector<point>();
    if(_M_map[0][0] != 0)
        reset();
    Vector<point> path;
    bool found = solve_a_star(0, 0, path);
    if(!found)
        return Vector<point>();
    return path;
}

bool maze::solve_a_star(size_t i, size_t j, Vector<point>& v)
{
    if(i < 0 || i >= num_rows())
        return false;
    if (j < 0 || j >= num_rows())
        return false;
    if(!is_path(i, j))
        return false;
    if(has_seen(i, j))
        return false;
    if(is_exit(i, j))
    {
        v.push_back({i, j});
        _M_map[i][j] = 2;
        return true;
    }
    _M_map[i][j] = 1; //Indicate we've seen this cell
    double h1 = h(i + 1, j);
    double h2 = h(i, j + 1);
    double h3 = h(i - 1, j);
    double h4 = h(i, j - 1);

    double arr[] = {h1, h2, h3, h4};
    int back = 3;
    mergesort(arr, 4);

    bool path_exists = false;
    while(back >= 0)
    {
        auto val = arr[back];
        if(std::abs(arr[back] - h1) < 1e-6)
        {
            if(solve_a_star(i + 1, j, v))
            {
                path_exists = true;
                break;
            }
        }
        else if (std::abs(arr[back] - h2) < 1e-6)
        {
            if(solve_a_star(i, j + 1, v))
            {
                path_exists = true;
                break;
            }
        }
        else if (std::abs(arr[back] - h3) < 1e-6)
        {
            if(solve_a_star(i -1, j, v))
            {
                path_exists = true;
                break;
            }
        }
        else
        {
            if(solve_a_star(i, j - 1, v))
            {
                path_exists = true;
                break;
            }
        }
        --back;
    }
    if(path_exists)
    {
        v.push_back({i, j});
        _M_map[i][j] = 2;
        return true;
    }
    return false;
}

bool maze::solve_DFS(size_t i, size_t j, Vector<point>& v)
{
    if (i < 0 || i >= num_rows()) //Out of bounds
        return false;
    if (j < 0 || j >= num_cols()) //Out of bounds
        return false;
    if(!is_path(i, j)) //Hit barrier
        return false;
    if(has_seen(i, j)) //Already explored this cell
        return false;
    if(is_exit(i, j)) //Seen exit
    {
        v.push_back({i, j});
        _M_map[i][j] = 2;
        return true;
    }
    //Indicate we've seen this cell
    _M_map[i][j] = 1;
    if(solve_DFS(i + 1, j, v) ||
        solve_DFS(i, j + 1, v) ||
        solve_DFS(i - 1, j, v) ||
        solve_DFS(i, j - 1, v)) //There is a path from this point to the exit
    {
        v.push_back({i, j});
        _M_map[i][j] = 2;
        return true;
    }
    //No path from this point to the exit
    return false;
} 

bool maze::is_path(size_t i, size_t j) const
{
    return _M_map[i][j] != -1;
}

bool maze::has_seen(size_t i, size_t j) const 
{
    return _M_map[i][j] == 1 || _M_map[i][j] == 2;
}

bool maze::is_exit(size_t i, size_t j) const 
{
    return i == num_rows() - 1 && j == num_cols() - 1;
}

void maze::reset()
{
    for(size_t i = 0; i < num_rows(); ++i)
    {
        for(size_t j = 0; j < num_cols(); ++j)
        {
            if(_M_map[i][j] != -1)
                _M_map[i][j] = 0;
        }
    }
}

double maze::h(size_t i, size_t j) const 
{
    int dx = int(i - (num_rows() - 1));
    int dy = int(i - (num_cols() - 1));
    return sqrt(dx*dx + dy*dy);
}
