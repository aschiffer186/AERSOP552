int main(int argc, char** argv)
{
    //C++ supports arrays with multiple dimensions (i.e. matrices).
    //These arrays are implemented as a pointer to a pointer. 
    //
    //This array looks like char** arr or char*[] arr or char[][] arr
    //
    //In memory the array might look something like this 
    // {
    //    {'a', 'b', 'c', 'd', '\0'},
    //    {'d', 'e', 'f', 'g', '\0'},
    //    {''h', 'i', 'j', 'k;, '\0'}
    // }
    //
    // This is an array with three elements that also arrays
    //
    //We can iterate over this array using two loops. The first loop
    //will iterate over each array in the outer array. When looking at 
    //a particular array-element, the inner loop\can iterate over each 
    //element of that array. 

    //Declaraing a two-dimensional array 
    char arr[5][5];

    //We can use index notation to access a particular element just like 
    //a regular array. When indexing into two-dimensional arrays, it's helpful 
    //to think of them as matrices and you are using a row column index. 
    //
    //Indexing looks like this 
    arr[1]/*the first index is the row*/[2]/*the second index is the column*/ = 'a';

    //Two-dimensional arrays can also be dynamically allocated. Note that they must 
    //be allocated as a pointer to a pointer.
    int** arr2 = new int*[5];
    //arr2 is a dynamically allocated array of char-arrays.
    //However each array is current unitialized. 

    //We will now initialize each array 
    for(int i = 0; i < 5; ++i)
    {
        arr2[i] = new int[4];
    }
    //Each pointer in arr2 is dynamically allocated. Before deleting arr2 you must delete 
    //each dynamically allocated array in arr2 as well, or you will create a memory leak. 
    for(int i = 0; i < 5; ++i)
    {
        delete[] arr2[i]; //Since each element is an array, you must call delete[]
    }

    //Delete arr2
    delete[] arr2;

    return 0;
}