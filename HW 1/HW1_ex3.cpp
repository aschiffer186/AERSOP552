#include<iostream>
using namespace std;
#include<fstream>
#include<cmath>
#include<cstring>

//A couple things need to be changed to eliminate bugs
//Once they are changed, everything should work

void life() {
    ifstream file("life.dat");
    char temp[10000];
    file.getline(temp, 10000); //<--get rid of this line
    int num_rows = 0;
    int prev_row = 0;
    int curr_row = 0;
    int num_cols = 0;
    //Check the dimensions of the file and if it is valid
    while (file.getline(temp, 10000)) {
        int len = strlen(temp);
        curr_row = len;
        if (num_rows!=0) {
            if (curr_row!=prev_row) {
                cout<<"Error"<<endl;
                return ;
            }
        }

        num_cols=len;

        for (int l = 0; l < num_cols; ++l) {
                if (temp[l]!=' ' & temp[l]!='O') { //should be temp[l] != ' ' && temp[l] != 'O'
                    cout<<"Value error"<<endl;
                    return ;
                }
        }

        prev_row = curr_row;
        ++num_rows;
    }

    file.close();
    file.clear();
    file.seekg(0, file.beg);
    file.open("life.dat"); //filename instead of "life.dat"

    //Copy the data from the file into an array
    char** board = new char*[num_rows];
    for (int i = 0; i < num_rows; ++i) {
        board[i] = new char[num_cols + 1];
        file.getline(board[i], num_cols + 1);
    }

    //Enter/quit condition
    //Can't use string, need to user char user_input[1000] = "";
    string user_input = "";

    while (user_input != "quit") {
        char** nextGeneration = new char*[num_rows]; // creates a copy of the original board to work on
        for (int row = 0; row < num_rows; ++row) {
            nextGeneration[row]=new char[num_cols + 1];
            for (int col = 0; col < num_cols + 1; ++col) {
                nextGeneration[row][col] = board[row][col];
            }
        }

        // conditions for the neighbours
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_cols; ++j) {
                int sum = 0;
                for (int k = i-1; k <= i+1; ++k) {
                    for (int l = j-1; l <= j+1; ++l) {
                        if (l >= 0 && k >= 0 && k < num_rows && l < num_cols && (k != i || l != j)) {
                            if (board[k][l] == 'O') {
                                sum++;
                            }
                        }
                    }
                }
                // conditions of life and death
                if (board[i][j]=='O') {
                    if (sum < 2 || sum > 3) {
                        nextGeneration[i][j]=' ';
                    } else if (sum == 2 || sum == 3) {
                        nextGeneration[i][j]='O';
                    }
                } if (board[i][j]==' ') {
                    if (sum == 3) {
                        nextGeneration[i][j]='O';
                    }
                    else {
                        nextGeneration[i][j]=board[i][j];
                    }
                }
            }
        }
        for (int row = 0; row < num_rows; ++row) {
            cout<<nextGeneration[row]<<endl;
        }

        cout<<"Press <Enter> to continue or type quit"<<endl;
        //Need to use cin.getline(user_input, 1000);
        getline(cin, user_input);

        for (int row = 0; row < num_rows; ++row) {
            for (int col = 0; col < num_cols + 1; ++col) {
                board[row][col] = nextGeneration[row][col];
            }
        }
    
        for(int row = 0; row < num_rows; ++row) {
            delete[] nextGeneration[row];
        }
        delete[] nextGeneration;
    }

    for(int i = 0; i < num_rows; ++i) {
        delete[] board[i];
    }
    delete[] board;
}

int main() {
    life();
    return 0;
    }