#ifndef TREE_H
#define TREE_H

#include <iostream>

class Tree 
{
    private:
        struct tree 
        {
            int _M_datum;
            tree* _M_left;
            tree* _M_right;
        };
    
    public:
        //Creates an empty tree.
        //Complexity: O(1)
        Tree();

        //Constructs this tree with the contents of the specified tree. 
        //Performs a deep copy and preserves the structure of the specified tree.
        //Complexity: O(log(n))
        //@param other the tree used to construct *this
        Tree(const Tree& other);

        //Replaces the contents of *this with the contents of the specified tree.
        //Performs a deep copy and preserves the structure of the specified tree. 
        //Complexity: O(log(n))
        //@param other the tree to be assigned into *this
        Tree& operator=(const Tree& other);

        //Destroys the contents of *this.
        //Complexity: O(log(n))
        ~Tree();

        //Inserts the specified integer into the tree preserving the ordering 
        //property of a binary search tree. If i is already present in the tree, 
        //does nothing. 
        //Complexity: O(log(n))
        //@param i the value to be inserted into the tree
        void insert(int i);
        
        //Removes the specified integer from the tree preserving the ordering 
        //property of a binary search tree. If i s not pressent in the tree, 
        //does nothing. If the tree is empty, does nothing. 
        //Complexity: O(log(n))
        //@param i the value to be replaced from the tree
        void erase(int i);

        //Returns the number of elements in the tree. 
        //Complexity: O(log(n))
        //@return the number of elements in the tree
        int size() const;

        //Determines if the specified integer is in the tree. 
        //Complexity: O(log(n))
        //@param i the element to be searched for 
        //@return true if the element is in the tree
        bool member(int i);

        //Converts the contents of the tree into a dynamically allocated 
        //array. The elements in the array are stored in increasing order. 
        //If the tree is empty, returns nullptr. 
        //Complexity: O(log(n))
        //@return an array containing the elements
        int* toarray();
    private:
        //Recursive implementation of toarray()
        void toarray_impl(tree* n, int* arr, int& index);
        //Recursive implementation of member()
        bool member_impl(tree* n, int i);
        //Recursive implemention of insert()
        tree* insert_impl(tree* n, int i);
        //Recursive implementation of size()
        int size_impl(tree* n) const;
        //Recursively delete nodes of *this
        void delete_nodes(tree* root);
        //Copy tree rooted at root, preserving structure
        tree* copy_nodes(tree* root);
        //Recursive implementation of erase()
        tree* erase_impl(tree* n, int i);
        //Finds the largest node smaller than n
        tree* inorder_predecessor(tree* n);
        //Recursive implementation of operator <<
        static std::ostream& print_nodes(std::ostream& os, tree* n);
    private:
        friend std::ostream& operator<<(std::ostream& os, const Tree& t);
    private:
        tree* _M_root;
};

 //Determines the set union of the elements of two trees. 
 //Complexity: if n is b.size() then O(nlog(n))
 //@param a one of the sets 
 //@param b the other set
 //@return the set union of a and b
 Tree treeUnion(Tree a, Tree b);

 //Determines the set intersection of the elements of two trees.
 //Complexity if 1 tree is empty than O(1). Otherwise O(nlog(n))
 //@param a one of the sets 
 //@param b the other set 
 //@return the set intersection
 Tree intersection(Tree a, Tree b);

#endif