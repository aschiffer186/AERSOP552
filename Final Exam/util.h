#ifndef UTIL_HH
#define UTIL_HH 1

#include <iostream>

template<typename _Tp>
class Vector 
{
    public:
        Vector() : 
            _M_size{0}, _M_cap{0}, _M_data{}
        {

        }

        Vector(size_t amt, const _Tp& t = _Tp()) : 
            _M_size(amt), _M_cap(amt)
        {
            _M_data = new _Tp[amt];
            for(size_t i = 0; i < _M_size; ++i)
            {
                _M_data[i] = _Tp(t);
            }
        }

        Vector(const Vector& other) :
            _M_size(other._M_size), _M_cap(other._M_cap)
        {
            _M_data = new _Tp[other._M_cap];
            for(size_t i = 0; i < other._M_size; ++i)
            {
                _M_data[i] = other._M_data[i];
            }
        }

        Vector(Vector&& other) :
            _M_size(other._M_size), _M_cap(other._M_cap)
        {
            _M_data = other._M_data;
            other._M_data = nullptr;
            other._M_size = 0;
            other._M_cap = 0;
        }

        ~Vector()
        {
            if(_M_data)
                delete[] _M_data;
            _M_data = nullptr;
            _M_size = 0;
            _M_cap = 0;
        }

        Vector& operator=(const Vector& other)
        {
            if(&other != this)
            {
                delete[] _M_data;
                _M_size = other._M_size;
                _M_cap = other._M_cap;
                _M_data = new _Tp[other._M_cap];
                for(size_t i = 0; i < other._M_size; ++i)
                {
                    _M_data[i] = other._M_data[i];
                }
            }
            return *this;
        }

        Vector& operator=(Vector&& other)
        {
            if(&other != this)
            {
                _M_size = other._M_size;
                _M_cap = other._M_cap;
                _M_data = other._M_data;
                other._M_data = nullptr;
                other._M_size = 0;
                other._M_cap = 0;
            }
            return *this;
        }

        void push_back(const _Tp& t)
        {
            if(_M_cap == _M_size)
                reallocate();
            _M_data[_M_size++] = _Tp(t);
        }

        void pop_back()
        {
            if(_M_size > 0)
                --_M_size;
        }

        size_t size() const 
        {
            return _M_size;
        }

        bool is_empty() const 
        {
            return size() == 0;
        }

        _Tp& back() 
        {
            return _M_data[_M_size - 1];
        }

        const _Tp& back() const 
        {
            return _M_data[_M_size - 1];
        }

        _Tp& front() 
        {
            return _M_data[0];
        }

        const _Tp& front() const 
        {
            return _M_data[0];
        }

        _Tp& operator[](size_t i)
        {
            return _M_data[i];
        }

        const _Tp& operator[](size_t i) const 
        {
            return _M_data[i];
        }
    
    private:
        void reallocate()
        {
            if (_M_cap == 0)
            {
                _M_data = new _Tp[1];
                _M_cap = 1;
            }
            else 
            {
                size_t new_cap = _M_cap*2;
                _Tp* temp = new _Tp[new_cap];
                for(size_t i = 0; i < _M_size; ++i)
                {
                    temp[i] = std::move(_M_data[i]);
                }
                delete[] _M_data;
                _M_data = temp;
                _M_cap = new_cap;
            }
        }
    private:
        size_t _M_size;
        size_t _M_cap;
        _Tp* _M_data;
};

struct point 
{
    size_t row;
    size_t col;
};

bool operator==(const point& lhs, const point& rhs);

bool operator!=(const point& lhs, const point& rhs);

std::ostream& operator<<(std::ostream& os, const point& p);

class maze
{
    public:
        maze();
        maze(const Vector<Vector<bool>>& map);
        Vector<point> solve_DFS();
        Vector<point> solve_a_star();
    private:
        size_t num_rows() const;
        size_t num_cols() const;
        bool solve_DFS(size_t i, size_t j, Vector<point>& v);
        bool solve_a_star(size_t i, size_t j, Vector<point>& v);
        bool is_path(size_t i, size_t j) const;
        bool is_exit(size_t i, size_t j) const;
        bool has_seen(size_t i, size_t j) const;
        void reset();
        double h(size_t i, size_t j) const;
    private:
        //-1 = barrier 
        //0 = not seen 
        //1 = seen 
        //2 = path
        Vector<Vector<int>> _M_map;
};

template<typename _Tp, typename _Comp>
class priority_queue
{
    public:
        priority_queue(const _Comp& c = _Comp())
            : _M_comp{c}, _M_data{}
        {

        }

    private:
        _Comp _M_comp;
        Vector<_Tp> _M_data;
};


#endif