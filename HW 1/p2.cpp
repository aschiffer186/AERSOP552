#include<iostream>

void hanoi_helper(int n, char A, char B, char C);

void hanoi(int n)
{
    if (n <= 0)
        return;
    hanoi_helper(n, 'A', 'B', 'C');
}

void hanoi_helper(int n, char A, char B, char C)
{
    if(n == 1)
    {
        std::cout << "Make a move from " << A << " to " << C << std::endl;
        return;
    }
    hanoi_helper(n-1, A, C, B);
    std::cout << "Make a move from " << A << " to " << C << std::endl;
    hanoi_helper(n-1, B, A, C);
}

int main(int argc, char** argv)
{
    int n;
    do {
        std::cout << "Enter the number of disks (or -1 to quit): ";
        std::cin >> n;
        hanoi(n);
    } while (n >= 0);
    return 0;
}