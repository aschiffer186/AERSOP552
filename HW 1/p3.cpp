#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>

static const char DEAD = ' ';
static const char ALIVE = 'O';

/*
//This function is terrible
std::string* read_file_array(const char* filename, int& num_rows, int& num_cols)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
        throw std::invalid_argument("Error opening file");
    std::string line;
    int curr_size = 0;
    std::string* board;
    while(std::getline(fin, line))
    {
        bool invalid_character = std::any_of(line.begin(), line.end(), [](char c) {return !isspace(c) && c != 'O';});
        if(invalid_character)
            throw std::invalid_argument("Invalid character in game board");
        std::string* temp = new std::string[curr_size+1];
        if (curr_size > 1 && temp[curr_size - 1].size() != line.size())
             throw std::invalid_argument("All lines must be the same length");
        for(int i = 0; i < curr_size; ++i)
        {
            temp[i] = board[i];
        }
        temp[curr_size++] = line;
        delete[] board;
        board = temp;
    }
    num_rows = curr_size;
    num_cols = line.size();
    return board;
}

int count_neigbors(std::string board[], int r, int c, int num_rows, int num_cols)
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
            if (r + i >= num_rows || c + j  >= num_cols) continue;
            if (board[r + i][c + j] == ALIVE) ++count;
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
    std::string* board;
    int num_rows = 0;
    int num_cols = 0;
    try 
    {
        board = read_file_array(name.c_str(), num_rows, num_cols);
    } catch (const std::invalid_argument& ex) {
        std::cout << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string choice = "l";
    std::string* next_gen = new std::string[num_rows];
    while(true)
    {
        for(size_t i = 0; i < num_rows; ++i)
        {
            next_gen[i] = board[i];
        }
        for(size_t i = 0; i < num_rows; ++i)
        {
            for(size_t j = 0; j < num_cols; ++j)
            {
                int count = count_neigbors(board, i, j, num_rows, num_cols);
                char cell = board[i][j];
                if (cell == ALIVE && count < 2)
                    next_gen[i][j] = DEAD;
                else if (cell == ALIVE && count >= 4)
                    next_gen[i][j] = DEAD;
                else if(cell == DEAD && count == 3)
                    next_gen[i][j] = ALIVE;
                else
                    next_gen[i][j] = board[i][j];
            }
        }
        //Print board
        print_board(next_gen, num_rows);
        std::cout << "Press <Enter> to continue or type quit\n";
        std::cin.ignore();
        if (std::cin.peek() != '\n') break;
        for (size_t i = 0; i < num_rows; ++i)
        {
            board[i] = next_gen[i];
        }
    }
    delete[] board;
    delete[] next_gen;
}
*/

void arr_copy(std::string* src, std::string* dest, int n)
{
    for(int i = 0; i < n; ++i)
    {
        dest[i] = src[i];
    }
}

std::string* read_from_file(const char* filename, int& num_rows, int& num_cols)
{
    std::string line;
    std::ifstream fin(filename);
    if(!fin.is_open())
    {
        std::cerr << "Invalid filename" << std::endl;
        return nullptr;
    }
    int curr_size = 0;
    std::string* board = nullptr;
    while(std::getline(fin, line))
    {
        if(curr_size > 0 && line.size() != board[curr_size - 1].size())
        {
            delete[] board;
            std::cerr << "All lines must be same length" << std::endl;
            return nullptr;
        }

        for(size_t i = 0; i < line.length(); ++i) 
        {
            if (!(line[i] != DEAD || line[i] != ALIVE))
            {
                delete[] board;
                std::cerr << "Invalid Character" << std::endl;
                return nullptr;
            }
        }
        std::string* temp = new std::string[curr_size + 1];
        arr_copy(board, temp, curr_size);
        temp[curr_size++] = line;
        delete[] board;
        board = temp;
    }
    num_cols = board[curr_size - 1].size();
    num_rows = curr_size;
    return board;
}

void print_board(std::string board[], int num_rows)
{
    for (int i = 0; i < num_rows; ++i)
    {
        std::cout << board[i] << std::endl;
    }
}

int count_neigbors(std::string board[], int r, int c, int num_rows, int num_cols)
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
            if (r + i >= num_rows || c + j  >= num_cols) continue;
            if (board[r + i][c + j] == ALIVE) ++count;
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
    std::string* board;
    int num_rows = 0;
    int num_cols = 0;
    board = read_from_file(name.c_str(), num_rows, num_cols);
    if(!board) return;
    std::string choice = "l";
    std::string* next_gen = new std::string[num_rows];
    while(true)
    {
        arr_copy(board, next_gen, num_rows);
        for(int i = 0; i < num_rows; ++i)
        {
            for(int j = 0; j < num_cols; ++j)
            {
                int count = count_neigbors(board, i, j, num_rows, num_cols);
                char cell = board[i][j];
                if (cell == ALIVE && count < 2)
                    next_gen[i][j] = DEAD;
                else if (cell == ALIVE && count >= 4)
                    next_gen[i][j] = DEAD;
                else if(cell == DEAD && count == 3)
                    next_gen[i][j] = ALIVE;
                else
                    next_gen[i][j] = board[i][j];
            }
        }
        //Print board
        print_board(next_gen, num_rows);
        std::cout << "Press <Enter> to continue or type quit\n";
        std::cin.ignore();
        if (std::cin.peek() != '\n') break;
        arr_copy(next_gen, board, num_rows);
    }
    delete[] board;
    delete[] next_gen;
}


int main(int argc, char** argv)
{
    life();
    return 0;
}