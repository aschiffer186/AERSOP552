#include<iostream>
using namespace std;

//There's nothing wrong with doing this, but since you don't
//actually use anything from the cmath library, it isn't needed
//and wastes memory.
#include<cmath>

//The recursive function will stop when n is exactly 1, but what 
//happens if n is less than one. For example, what if I call hanoi(0)?
//
//Then n = 0. Since n != 1, the else clause is triggered. So hanoi_int 
//is called again with n = -1. Since n != 1, the else clause is triggered.
//so hanoi_int is called again with n = -2. And so on and so forth.
//
//I don't know if they will be testing with n <= 0, but it's something to keep in mind. 
//
//On Piazza he said not to use string in p3, my guess is that applies here too,
//try replacing the string with char.
void hanoi_int(int n, string source, string dest, string aux) {
    if (n==1) {
        cout<<"Make a move from "+source+" to "+dest<<endl;
    }
    else {
        hanoi_int(n-1, source, aux, dest);
        cout<<"Make a move from "+source+" to "+dest<<endl;
        hanoi_int(n-1, aux, dest, source);
    }
    }

void hanoi(int n){
    //If you replace string with char, you'll need to change this to 'A', 'B', 'C'
    hanoi_int(n, "A", "C", "B");
}

int main(){
    //Make sure you should additional test cases.
    hanoi(3);
    return 0;
}