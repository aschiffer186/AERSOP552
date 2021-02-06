#include <fstream>
#include <cstring>
#include <iostream>

static const char DEAD = ' ';
static const char ALIVE = 'O';


struct board
{
    public:
    board() :
    _M_num_rows{}, _M_num_cols{}, _M_board(nullptr)
    {

    }

    board(size_t num_rows, size_t num_cols) :
    _M_num_rows(num_rows), _M_num_cols(num_cols)
    {
        _M_board = new char*[_M_num_rows];
        for(size_t i = 0; i < _M_num_rows; ++i)
        {
            _M_board[i] = new char[num_cols + 1];
            _M_board[i][num_cols] = '\0';
        }
    }

    board(const board& other) :
    _M_num_rows(other._M_num_rows), _M_num_cols(other._M_num_cols)
    {
        _M_board = new char*[_M_num_rows];
        for(size_t i = 0; i < _M_num_rows; ++i)
        {
            _M_board[i] = new char[_M_num_cols + 1];
            for (size_t j = 0; j < _M_num_cols + 1; ++j)
            {
                _M_board[i][j] = other._M_board[i][j];
            }
        }
    }

    board& operator=(const board& other)
    {
        if (this != &other)
        {
            destroy_board();
            _M_num_rows = other._M_num_rows;
            _M_num_cols = other._M_num_cols;
            _M_board = new char*[_M_num_rows];
            for(size_t i = 0; i < _M_num_rows; ++i)
            {
                _M_board[i] = new char[_M_num_cols + 1];
                for (size_t j = 0; j < _M_num_cols + 1; ++j)
                {
                    _M_board[i][j] = other._M_board[i][j];
                }
            }
        }
        return *this;
    }

    ~board()
    {
        destroy_board();
    }

    size_t get_num_rows() const 
    {
        return _M_num_rows;
    }

    size_t get_num_cols() const 
    {
        return _M_num_cols;
    }

    char& operator()(size_t i, size_t j) 
    {
        return _M_board[i][j];
    }

    const char& operator()(size_t i, size_t j) const 
    {
        return _M_board[i][j];
    }

    void push_row(const char* c, size_t row)
    {
        size_t len = strlen(c);
        for (size_t i = 0; i < len; ++i)
        {
            _M_board[row][i] = c[i];
        }
    }

    char * get_row(size_t row)
    {
        return _M_board[row];
    }

    private:
        void destroy_board() noexcept
        {
            for(size_t i = 0; i < _M_num_rows; ++i)
                delete[] _M_board[i];
            delete[] _M_board;
        }
    private:
        friend std::ostream& operator<<(std::ostream& os, const board& b);
    private:
        size_t _M_num_rows;
        size_t _M_num_cols;
        char** _M_board;
};

std::ostream& operator<<(std::ostream& os, const board& b)
{
    for (size_t i = 0; i < b._M_num_rows; ++i)
    {
        os << b._M_board[i] << std::endl;
    }
    return os;
}

board read_file(const char* filename)
{
    size_t m = 0, n = 0;
    char row[10000];
    size_t tmp_m;
    std::ifstream file(filename);
    while(file.getline(row, 10000)) 
    {
        if (n == 0) {
            m = strlen(row);
        } 
        else {
            tmp_m = strlen(row);
            if (tmp_m != m) 
            {
                std::cout << "Invalid pattern! Rows not same size." << std::endl;
                return board();
            }
            for (size_t i = 0; i < m; i++) {
                if ( (row[i] != 'O') && (row[i] != ' ') ) 
                {
                    std::cout << "Invalid pattern! Invalid character." << std::endl;
                    return board();
                }
            }
        }
        n++;
   }

   board b(n, m);
   file.close();
   file.clear();
   file.seekg(0, file.beg);
   file.open(filename);
   for(size_t i = 0; i < n; ++i)
   {
       file.getline(b.get_row(i), m+1);
   }
   file.close();
   return b;
}

int count_neigbors(const board& b, int r, int c)
{
    //Check along top row 
    int count = 0;
    for (int i = -1; i < 2; ++i)
    {
        for (int j = -1; j < 2; ++j)
        {
            //Don't count cell itself
            if (i == 0 && j == 0) continue;
            //Check for out of bounds
            if (r + i < 0 || c + j < 0) continue;
            if (r + i >= int(b.get_num_rows()) || c + j  >= int(b.get_num_cols())) continue;
            if (b(size_t(r + i), size_t(c + j)) == ALIVE) ++count;
        }
    }
    return count;
}

void life()
{
    #ifdef DEBUG
        std::string name = "C:\\Users\\aschi\\Dropbox\\AERO 552\\HW 1\\life.dat";
    #else
        std::cout << "Enter a file name: " << std::endl;
        std::string name;
        std::cin >> name;
    #endif
    board b = read_file(name.c_str());
    if (b.get_num_rows() == 0) return;
    std::string choice = "l";
    board next_gen;
    while(true)
    {
       next_gen = b;
        for(size_t i = 0; i < b.get_num_rows(); ++i)
        {
            for(size_t j = 0; j < b.get_num_cols(); ++j)
            {
                int count = count_neigbors(b, i, j);
                char cell = b(i,j);
                if (cell == ALIVE && count < 2)
                    next_gen(i,j) = DEAD;
                else if (cell == ALIVE && count >= 4)
                    next_gen(i,j) = DEAD;
                else if(cell == DEAD && count == 3)
                    next_gen(i,j) = ALIVE;
                else
                    next_gen(i,j) = b(i,j);
            }
        }
        //Print board
        std::cout << next_gen;
        std::cout << "Press <Enter> to continue or type quit\n";
        std::cin.ignore();
        if (std::cin.peek() != '\n') break;
        b = next_gen;
    }
}

int main()
{
    life();
    return 0;
}