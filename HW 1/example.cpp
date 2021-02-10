//Read from the file into int a character array 
//char**
#include <fstream>
#include <cstring>

void life(const char* filename)
{
    std::ifstream file(filename);

    //Create a character array with a lot space (10,000 possible characters)
    //use file.getline method to read file 
    //char temp[] = {'a', 'b', 'c', .... , '\0'}
    char temp[100000];
    file.getline(temp, 100000);

    ///Keep track of how many rows of you've seen 
    //Keep track of how many characters are in the current row
    //Keep track of the characters in the previous row
    int i = 0;
    //variable to store current row 
    //variable to store previous row 
    while(file.getline(temp, 10000))
    {
        //char str[]  = {'1', '2', '3', '\0'}
        int len = std::strlen(temp);
        //Check if num characters in current row == num characters in previous row
        //If not, print an error message and terminate the function 
        
        //Iterate of every character of the row and if it isn't ' ' or 'O', you have to print 
        //for (int i = 0; i < num_cols; ++i)
        //  temp[i]
        //an error message and terminate the program. 
        //previous row = current row
    }
    //After this loop, you should know 
    //1) the dimensions of the board 
    //2) that the board is a valid
    //useful standard library functions (you will need to read the documentation to figure out what arguments to use for file.seekg())
    //file.close();
    //file.clear();
    //file.seekg();
    //Look up documentation for ifstream

    //char** board = new char*[num_rows];
    // board == ptr1 | ptr2 | ptr3 | ptr4 ......
    // for(int i = 0; i < num_rows; ++i)
    //  dynamically allocate an array for the line (board[i] = new char[num_cols + 1])
    //  file.getline(board[i], num_cols + 1);
    //
    //After the second loop, you should have a 2D char array containing the board

    //When everything is done (after the user says quit)
    //delete each row 
    //delete the board
}