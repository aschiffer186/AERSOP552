#include <iostream>
#include <cstdlib>

struct foo
{
    foo(int i)
    {
        std::cout << "Constructor called!" << std::endl;
        _M_ptr = new int(i);
    }

    ~foo()
    {
        std::cout << "Destructor called!" << std::endl;
        delete _M_ptr;
    }

    int get_value() const 
    {
        return *_M_ptr;
    }

    private:
        int* _M_ptr;
};

int main(int argc, char** argv)
{
    //Using new and delete together is how it should be 
    //New allocates enough memory to store an object of 
    //type "foo," then initializes it using foo's constructor.
    //It then returns a pointer to the space in memory containing 
    ///foo.
    foo* ptr = new foo(5);
    //Foo has been initialized and all of its members have been initialized. 
    //They are allocated in memory as needed and contain known values. You 
    //can safely use the object pointed to by ptr. 
    //
    //In this case, we know the value of foo has been initialized, so calling 
    //get value will work.
    std::cout << "Calling ptr->get_value()" << ptr->get_value() << std::endl;
    //Calls the destructor of the object that ptr is pointing too, safely 
    //deallocating its dynamically allocated pointer. Then the memory for 
    //the object that foo is pointing too is deallocated.
    delete ptr;
    //Using new and delete together is the right way to management memory. 
    //It ensures all objects are initialized when the memory for them is allocated 
    //and safelty deallocates memory. 

    //If you call delete on a pointer, you cannot call delete again. This line is 
    //bad and could cause an error (if you're lucky).
    delete ptr;

    //You can allocate another object of type foo in memory and store it in the same 
    //pointer we used previous.
    ptr = new foo(6);
    //This is ok because we made ptr point to another dynamically allocated object.
    delete ptr;

    ptr = new foo(6);
    //This line is bad. You should never call free on a pointer created by new. It could 
    //cause an error. Even if it doesn't, it will not call the destructor on the object 
    //pointed to by ptr. Since this object holds a dynamically allocate object, you get 
    //a memory leak. 
    free(ptr);

    //This allocates memory for an object of type foo, but it doesn't initialize it or 
    //call its constructor. This means its stored pointer won't be initialized and will 
    //instead point to a junk address (which could hold anything)!
    ptr = static_cast<foo*>(malloc(sizeof(foo)));
    //Remember, the pointer stored in the object pointed to by ptr could point to anything!
    //Calling get_value() attempts to dereference it; this is undefined behavior and is bad.
    std::cout << ptr->get_value() << std::endl;
    //This calls the destructor of the object pointed to by ptr but that destructor is now 
    //trying to call delete on an unitialized ptr! This is bad and could cause an error 
    //(if you're lucky)
    delete ptr;

    ptr = static_cast<foo*>(malloc(sizeof(foo)));
    //If you call malloc, use free
    free(ptr);

    //In general, AVOID malloc and free in C++ unless you have a very good reason for doing so.
    //malloc and free are used in C, new and delete are used in C++ (the language this HW uses)

    return 0;
}