#ifndef BSTNODE_H
#define BSTNODE_H

#include <iostream>

template <typename T>
class Node {

public:

    T Key;
    int Height;

    // Constructor
    Node();
    Node(T value);
    Node(T value, Node<T> *parent);

    // Destructor
    //~Node();
    
    // Left, Right, Parent
    Node *Left;
    Node *Right;
    Node *Parent;
    
    int max_depth() const {
        const int left_depth = Left ? Left->max_depth() : 0;
        const int right_depth = Right ? Right->max_depth() : 0;
        return (left_depth > right_depth ? left_depth : right_depth) + 1;
    }
};

template <typename T>
Node<T>::Node(T value, Node<T> *parent) {

    Key    = value;
    Left   = NULL;
    Right  = NULL;
    Parent = parent;
}

template <typename T>
Node<T>::Node() {

    Left   = NULL;
    Right  = NULL;
    Parent = NULL;
}

template <typename T>
Node<T>::Node(T value) {

    Key    = value;
    Left   = NULL;
    Right  = NULL;
    Parent = NULL;
}

// Destructor
/*template <typename T>
Node<T>::~Node() {
    
    delete Left;
    delete Right;
    delete Parent;
}*/

#endif
