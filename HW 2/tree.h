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
        Tree();

        Tree(const Tree& other);

        Tree& operator=(const Tree& other);

        ~Tree();

        void insert(int i);
        
        void erase(int i);

        int size() const;

        bool member(int i);

        int* toarray();

        static Tree treeUnion(Tree a, Tree b);

        static Tree intersection(Tree a, Tree b);

    private:

        void toarray_impl(tree* n, int* arr, int& index) const;

        bool member_impl(tree* n, int i);

        tree* insert_impl(tree* n, int i);

        int size_impl(tree* n) const;

        void delete_nodes(tree* root);

        tree* copy_nodes(tree* root);

        tree* erase_impl(tree* n, int i);

        tree* inorder_predecessor(tree* n);

        static std::ostream& print_nodes(std::ostream& os, tree* n);
    private:
        friend std::ostream& operator<<(std::ostream& os, const Tree& t);
    private:
        tree* _M_root;
};

 Tree treeUnion(Tree a, Tree b);

 Tree intersection(Tree a, Tree b);

#endif