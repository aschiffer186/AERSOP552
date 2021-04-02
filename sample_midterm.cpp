#include <iostream>

struct intlist {
    int head;
    intlist* next;
};

intlist* clone(intlist* l)
{
    if(!l) return nullptr;
    intlist* node = new intlist;
    node->head = l->head;
    node->next = clone(l->next);
    return node;
}

intlist* add(intlist* l, int val)
{
    if(l == nullptr)
        return new intlist{val, nullptr};
    l->next = add(l->next, val);
    return l;
}


intlist* sort(intlist* l)
{
    //Quicksort but wwith linkedlist
    if(!l)
        return nullptr;
    intlist* new_list = clone(l);
    int pivot = l->head;
    intlist* left = nullptr;
    intlist* right = nullptr;
    //Partition lists
    while(new_list)
    {
        if(new_list->head < pivot)
        {
            left = add(left, new_list->head);
        }
        else if (new_list->head > pivot)
        {
            right = add(right, new_list->head);
        }
        intlist* dead = new_list;
        new_list = new_list->next;
        delete dead;
    }
    //Sort left half
    left = sort(left);
    //Sort right half
    right = sort(right);
    //Reassemble
    intlist* sorted = nullptr;
    while(left)
    {
        sorted = add(sorted, left->head);
        intlist* dead = left;
        left = left->next;
        delete dead;
    }
    sorted = add(sorted, pivot);
    while(right)
    {
        sorted = add(sorted, right->head);
        intlist* dead = right;
        right = right->next;
        delete dead;
    }
    return sorted;
}

//O(n^2*logn)

void free_list(intlist* l)
{
    while(l)
    {
        intlist* dead = l;
        l = l->next;
        delete l;
    }
}
  

void print_list(intlist* l)
{
    while(l)
    {
        std::cout << l->head << " ";
        l = l->next;
    }
}

int main() 
{
    intlist* l = nullptr;
    l = add(l, 6);
    l = add(l, 1);
    l = add(l, 2);
    l = add(l, -6);
    l = add(l, 3);
    l = add(l, 21);
    intlist* l2 = sort(l);
    print_list(l2);

    free_list(l);
    free_list(l2);
    return 0;
}

