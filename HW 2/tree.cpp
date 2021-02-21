#include "tree.h"
#include <iostream>

    Tree::Tree() 
    : _M_root(nullptr)
    {

    }

   Tree::Tree(const Tree& other)
    {
        _M_root = copy_nodes(other._M_root);
    }

    Tree& Tree::operator=(const Tree& other)
    {
        if(this != &other)
        {
            delete_nodes(_M_root);
            _M_root = copy_nodes(other._M_root);
        }
        return *this;
    }

    Tree::~Tree()
    {
        delete_nodes(_M_root);
        _M_root = nullptr;
    }
    
    void Tree::insert(int i)
    {
        if(!_M_root)
            _M_root = new Tree::tree{i, nullptr, nullptr};
        else if (i < _M_root->_M_datum)
            _M_root->_M_left = insert_impl(_M_root->_M_left, i);
        else if (i > _M_root->_M_datum)
            _M_root->_M_right = insert_impl(_M_root->_M_right, i);
    }
    
    void Tree::erase(int i)
    {
        if(!_M_root)
            return;
        _M_root = erase_impl(_M_root, i);
    }

    int Tree::size() const 
    {
        if(!_M_root)
            return 0;
        return size_impl(_M_root);
    }

    bool Tree::member(int i)
    {
        if(!_M_root)
            return false;
        return member_impl(_M_root, i);
    }

    int* Tree::toarray()
    {
        int curr_size = size();
        if (curr_size == 0)
            return nullptr;
        int* arr = new int[curr_size];
        int index = 0;
        toarray_impl(_M_root, arr, index);
        return arr;
    }

    Tree Tree::treeUnion(Tree a, Tree b)
    {
        Tree tree_union(a);
        int* b_arr = b.toarray();
        int size = b.size();
        for(int i = 0; i < size; ++i)
        {
            tree_union.insert(b_arr[i]);
        }
        delete[] b_arr;
        return tree_union;
    }

    Tree Tree::intersection(Tree a, Tree b)
    {
        Tree tree_intersection;
        int* short_begin,*short_end;
        int* long_begin, *long_end;

        int a_size = a.size();
        int b_size = b.size();

        int* a_arr = a.toarray();
        int* b_arr = b.toarray();

        if (a_size > b_size)
        {
            short_begin = b_arr;
            short_end = short_begin + b_size;
            long_begin = a_arr;
            long_end = long_begin + a_size;
        }
        else 
        {
            short_begin = a_arr;
            short_end = short_begin + a_size;
            long_begin = b_arr;
            long_end = long_begin + b_size;
        }

        for(; long_begin != long_end;)
        {
            if(short_begin == short_end)
                break;
            if(*long_begin == *short_begin)
            {
                tree_intersection.insert(*long_begin);
                ++short_begin;
                ++long_begin;
            }
            else if(*long_begin < *short_begin)
                ++long_begin;
            else 
                ++short_begin;
        }
        delete[] a_arr;
        delete[] b_arr;
        return tree_intersection;
    }

    void Tree::toarray_impl(Tree::tree* n, int* arr, int& index) const
    {
        if(n)
        {
            toarray_impl(n->_M_left, arr, index);
            arr[index++] = n->_M_datum;
            toarray_impl(n->_M_right, arr, index);
        }
    }

    bool Tree::member_impl(Tree::tree* n, int i)
    {
        if(n)
        {
            if(n->_M_datum == i)
                return true;
            return member_impl(n->_M_left, i) || member_impl(n->_M_right, i);
        }
        return false;
    }

    Tree::tree* Tree::insert_impl(Tree::tree* n, int i)
    {
        if(!n)
            return new Tree::tree{i, nullptr, nullptr};
        if(i < n->_M_datum)
            n->_M_left = insert_impl(n->_M_left, i);
        else if(i > n->_M_datum)
            n->_M_right = insert_impl(n->_M_right, i);
        return n;
    }

    int Tree::size_impl(Tree::tree* n) const
    {
        if(!n)
            return 0;
        return 1 + size_impl(n->_M_left) + size_impl(n->_M_right);
    }

    void Tree::delete_nodes(Tree::tree* root)
    {
        if(root)
        {
            delete_nodes(root->_M_left);
            delete_nodes(root->_M_right);
            delete root;
        }
    }

    Tree::tree* Tree::copy_nodes(Tree::tree* root) 
    {
        if(!root)
            return nullptr;
        Tree::tree* new_node = new Tree::tree;
        new_node->_M_left = copy_nodes(root->_M_left);
        new_node->_M_right = copy_nodes(root->_M_right);
        new_node->_M_datum = root->_M_datum;
        return new_node;
    }

    Tree::tree* Tree::erase_impl(Tree::tree* n, int i)
    {
        if(!n)
            return n;
        if(i < n->_M_datum)
        {
            n->_M_left = erase_impl(n->_M_left, i);
            return n;
        } 
        else if (i > n->_M_datum)
        {
            n->_M_right = erase_impl(n->_M_right, i);
            return n;
        }
        else 
        {
            if(!n->_M_left && !n->_M_right)
            {
                delete n;
                return nullptr;
            }
            else if (!n->_M_right)
            {
                Tree::tree* ret_val = n->_M_left;
                delete n;
                return ret_val;
            } 
            else if (!n->_M_left) 
            {
                Tree::tree* ret_val = n->_M_right;
                delete n;
                return ret_val;
            } else 
            {
                Tree::tree* predecessor = inorder_predecessor(n);
                int predecessor_datum = predecessor->_M_datum;
                erase_impl(n, predecessor->_M_datum);
                n->_M_datum = predecessor_datum;
                return n;
            }   
        }  
    }

    Tree::tree* Tree::inorder_predecessor(Tree::tree* n)
    {
        if(!n->_M_right)
            return n;
        return inorder_predecessor(n->_M_right);
    }

    std::ostream& Tree::print_nodes(std::ostream& os, Tree::tree* n)
    {
        if(n)
        {
            print_nodes(os, n->_M_left);
            os << n->_M_datum << " ";
            print_nodes(os, n->_M_right);
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Tree& t)
    {
        Tree::print_nodes(os, t._M_root);
        return os;
    }

    