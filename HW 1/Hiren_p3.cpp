#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int countNeighbors(char** lines, int i, int j, int n, int m) {
   int neighbors = 0;
   // Top left corner
   if ( (i == 0) && (j == 0) ) {
      if ( lines[i+1][j] == 'O' ) {
         neighbors++;
      }
      if ( lines[i][j+1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j+1] == 'O' ) {
         neighbors++;
      }
   }
   // Top right corner
   else if ( (i == 0) && (j == m-1) ) {
      if ( lines[i+1][j] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i][j-1] == 'O' ) {
         neighbors++;
      }
   }
   // Bottom left corner
   else if ( (i == n-1) && (j == 0) ) {
      if ( lines[i-1][j] == 'O' ) {
         neighbors++;
      }
      if ( lines[i-1][j+1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i][j+1] == 'O' ) {
         neighbors++;
      }
   }
   // Bottom right corner
   else if ( (i == n-1) && (j == m-1) ) {
      if ( lines[i-1][j] == 'O' ) {
         neighbors++;
      }
      if ( lines[i-1][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i][j-1] == 'O' ) {
         neighbors++;
      }
   }
   // Left side
   else if (j == 0) {
      if ( lines[i-1][j] == 'O' ) {
         neighbors++;
      }
      if ( lines[i-1][j+1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i][j+1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j+1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j] == 'O' ) {
         neighbors++;
      }
   }
   // Top side
   else if (i == 0) {
      if ( lines[i][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j+1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i][j+1] == 'O' ) {
         neighbors++;
      }
   }
   // Right side
   else if (j == m-1) {
      if ( lines[i-1][j] == 'O' ) {
         neighbors++;
      }
      if ( lines[i-1][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j] == 'O' ) {
         neighbors++;
      }
   }
   // Bottom side
   else if (i == n-1) {
      if ( lines[i][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i-1][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i-1][j] == 'O' ) {
         neighbors++;
      }
      if ( lines[i-1][j+1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i][j+1] == 'O' ) {
         neighbors++;
      }
   }
   else {
      if ( lines[i][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i-1][j-1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i-1][j] == 'O' ) {
         neighbors++;
      }
      if ( lines[i-1][j+1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i][j+1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j+1] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j] == 'O' ) {
         neighbors++;
      }
      if ( lines[i+1][j-1] == 'O') {
         neighbors++;
      }
   }
   return neighbors;
}


char** updateGrid(char** lines, int n, int m) {
   char **tmp = new char*[n];
   for (int i = 0; i < n; i++) {
      tmp[i] = new char[m];
      for (int j = 0; j < m; j++) {
         tmp[i][j] = lines[i][j];
      } 
   }
   for(int i = 0; i < n; i++) {
      for(int j = 0; j < m; j++) {
         int neighbors = 0;
         bool isAlive = true;
         if (lines[i][j] == ' ') {
            isAlive = false;
         }
         neighbors = countNeighbors(lines, i, j, n, m);
         if ( (neighbors == 3) && (!isAlive) ) {
            tmp[i][j] = 'O';
         }
         if ( (neighbors >= 4) && (isAlive) ) {
            tmp[i][j] = ' ';
         }
         if ( (neighbors < 2) && (isAlive) ) {
            tmp[i][j] = ' ';
         }
      }
   }
   for(int i = 0; i < n; i++) {
      delete[] lines[i];
   }
   delete[] lines;
   return tmp;
}


void life(const char *filename = "life.dat") {
   ifstream file(filename);
   int n = 0;
   int m = 0;
   char **lines;
   char* row = new char[10000];
   int tmp_m;
   while(file.getline(row, 10000)) {
      if (n == 0) {
         m = strlen(row);
      } 
      else {
         tmp_m = strlen(row);
         if (tmp_m != m) {
            cout << "Invalid pattern! Rows not same size." << endl;
            return;
         }
         for (int i = 0; i < m; i++) {
            if ( (row[i] != 'O') && (row[i] != ' ') ) {
               cout << "Invalid pattern! Invalid character." << endl;
            }
         }
      }
      n++;
   }
   file.close();
   file.clear();
   file.seekg(0, file.beg);
   delete[] row;
   file.open(filename);
   lines = new char*[n];
   for(int i = 0; i < n; i++) {
      lines[i] = new char[m+1];
      file.getline(lines[i], 10000); 
   }
   file.close();
   char userinput[1000] = "\n";
   while (strcmp(userinput, "quit") != 0) {
      lines = updateGrid(lines, n, m);
      for(int i = 0; i < n; i++) {
         for(int j = 0; j < m; j++) {
            cout << lines[i][j];
         }
         cout << endl;
      }
      cout << "Press <Enter> to continue or type quit" << endl;
      cin.getline(userinput, 1000);
   } 
   for(int i = 0; i < n; i++) {
      delete[] lines[i];
   }
   delete[] lines;
}

int main(int, char**argv) {
   life(argv[1]); 
   //Test Cases:
   //char filename[] = "life1.dat";
   //char filename[] = "life2.dat";
   //char filename[] = "life3.dat";
   //char filename[] = "life4.dat";
   //char filename[] = "life5.dat"; //<- Tests all corners.
   //life(filename); //<- Include for each test case.
   return 0;
}
