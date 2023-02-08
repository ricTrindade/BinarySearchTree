#ifndef BST_H
#define BST_H

#include "Node.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>

using std::vector;
using std::string;
using std::cout;

template <typename T>
class BST {

private:
    Node<T> *root;

protected:

    // Methods
    int numOfElements;
    Node<T> *Insert       (Node<T> *node, T key);
    Node<T> *Remove       (Node<T> *node, T key);
    Node<T> *Search       (Node<T> *node, T key);
    void PrintTreeInOrder (Node<T> *node);
    T FindMin             (Node<T> *node);
    T FindMax             (Node<T> *node);
    T Successor           (Node<T> *node);
    T Predecessor         (Node<T> *node);

public:

    // Constructor
    BST();
    
    // Destructor
    ~BST();

    // Getters
    int getNumOfElements() {return numOfElements;}
    Node<T> *getRoot()     {return root;}

    // Methods
    T* ConvertToArray();
    void PrintTreeInOrder();
    void Insert      (T key);
    void Remove      (T v);
    bool Search      (T key);
    T  Successor     (T key);
    T  Predecessor   (T key);
    T  FindMin();
    T  FindMax();
    
    // Extra Methods (Not mine)
    int get_max_depth() const { return root ? root->max_depth() : 0; }
    void clear() { delete root; root = nullptr; }
    struct cell_display {
        string   valstr;
        bool     present;
        cell_display() : present(false) {}
        cell_display(std::string valstr) : valstr(valstr), present(true) {}
    };
    
    using display_rows = vector<vector<cell_display>>;

    // The text tree generation code below is all iterative, to avoid stack faults.

    // get_row_display builds a vector of vectors of cell_display structs
    // each vector of cell_display structs represents one row, starting at the root
    display_rows get_row_display() const {
        // start off by traversing the tree to
        // build a vector of vectors of Node pointers
        vector<Node<T>*> traversal_stack;
        vector< std::vector<Node<T>*> > rows;
        if(!root) return display_rows();
    
        Node<T> *p = root;
        const int max_depth = root->max_depth();
        rows.resize(max_depth);
        int depth = 0;
        for(;;) {
            // Max-depth Nodes are always a leaf or null
            // This special case blocks deeper traversal
            if(depth == max_depth-1) {
                rows[depth].push_back(p);
                if(depth == 0) break;
                --depth;
                continue;
            }

            // First visit to node?  Go to left child.
            if(traversal_stack.size() == depth) {
                rows[depth].push_back(p);
                traversal_stack.push_back(p);
                if(p) p = p->Left;
                ++depth;
                continue;
            }
        
            // Odd child count? Go to right child.
            if(rows[depth+1].size() % 2) {
                p = traversal_stack.back();
                if(p) p = p->Right;
                ++depth;
                continue;
            }

            // Time to leave if we get here

            // Exit loop if this is the root
            if(depth == 0) break;

            traversal_stack.pop_back();
            p = traversal_stack.back();
            --depth;
        }

        // Use rows of Node pointers to populate rows of cell_display structs.
        // All possible slots in the tree get a cell_display struct,
        // so if there is no actual Node at a struct's location,
        // its boolean "present" field is set to false.
        // The struct also contains a string representation of
        // its Node's value, created using a std::stringstream object.
        display_rows rows_disp;
        std::stringstream ss;
        for(const auto& row : rows) {
            rows_disp.emplace_back();
            for(Node<T>* pn : row) {
                if(pn) {
                    ss << pn->Key;
                    rows_disp.back().push_back(cell_display(ss.str()));
                    ss = std::stringstream();
                } else {
                    rows_disp.back().push_back(cell_display());
        }   }   }
        return rows_disp;
    }

    // row_formatter takes the vector of rows of cell_display structs
    // generated by get_row_display and formats it into a test representation
    // as a vector of strings
    vector<string> row_formatter(const display_rows& rows_disp) const {
        using s_t = string::size_type;

        // First find the maximum value string length and put it in cell_width
        s_t cell_width = 0;
        for(const auto& row_disp : rows_disp) {
            for(const auto& cd : row_disp) {
                if(cd.present && cd.valstr.length() > cell_width) {
                    cell_width = cd.valstr.length();
        }   }   }

        // make sure the cell_width is an odd number
        if(cell_width % 2 == 0) ++cell_width;

        // allows leaf nodes to be connected when they are
        // all with size of a single character
        if(cell_width < 3) cell_width = 3;

        // formatted_rows will hold the results
        vector<string> formatted_rows;
    
        // some of these counting variables are related,
        // so its should be possible to eliminate some of them.
        s_t row_count = rows_disp.size();

        // this row's element count, a power of two
        s_t row_elem_count = 1 << (row_count-1);

        // left_pad holds the number of space charactes at the beginning of the bottom row
        s_t left_pad = 0;
    
        // Work from the level of maximum depth, up to the root
        // ("formatted_rows" will need to be reversed when done)
        for(s_t r=0; r<row_count; ++r) {
            const auto& cd_row = rows_disp[row_count-r-1]; // r reverse-indexes the row
            // "space" will be the number of rows of slashes needed to get
            // from this row to the next.  It is also used to determine other
            // text offsets.
            s_t space = (s_t(1) << r) * (cell_width + 1) / 2 - 1;
            // "row" holds the line of text currently being assembled
            string row;
            // iterate over each element in this row
            for(s_t c=0; c<row_elem_count; ++c) {
                // add padding, more when this is not the leftmost element
                row += string(c ? left_pad*2+1 : left_pad, ' ');
                if(cd_row[c].present) {
                    // This position corresponds to an existing Node
                    const string& valstr = cd_row[c].valstr;
                    // Try to pad the left and right sides of the value string
                    // with the same number of spaces.  If padding requires an
                    // odd number of spaces, right-sided children get the longer
                    // padding on the right side, while left-sided children
                    // get it on the left side.
                    s_t long_padding = cell_width - valstr.length();
                    s_t short_padding = long_padding / 2;
                    long_padding -= short_padding;
                    row += string(c%2 ? short_padding : long_padding, ' ');
                    row += valstr;
                    row += string(c%2 ? long_padding : short_padding, ' ');
                } else {
                    // This position is empty, Nodeless...
                    row += string(cell_width, ' ');
                }
            }
            // A row of spaced-apart value strings is ready, add it to the result vector
            formatted_rows.push_back(row);
        
            // The root has been added, so this loop is finsished
            if(row_elem_count == 1) break;

            // Add rows of forward- and back- slash characters, spaced apart
            // to "connect" two rows' Node value strings.
            // The "space" variable counts the number of rows needed here.
            s_t left_space  = space + 1;
            s_t right_space = space - 1;
            for(s_t sr=0; sr<space; ++sr) {
                string row;
                for(s_t c=0; c<row_elem_count; ++c) {
                    if(c % 2 == 0) {
                        row += string(c ? left_space*2 + 1 : left_space, ' ');
                        row += cd_row[c].present ? '/' : ' ';
                        row += string(right_space + 1, ' ');
                    } else {
                        row += string(right_space, ' ');
                        row += cd_row[c].present ? '\\' : ' ';
                    }
                }
                formatted_rows.push_back(row);
                ++left_space;
                --right_space;
            }
            left_pad += space + 1;
            row_elem_count /= 2;
        }

        // Reverse the result, placing the root node at the beginning (top)
        std::reverse(formatted_rows.begin(), formatted_rows.end());
    
        return formatted_rows;
    }

    // Trims an equal number of space characters from
    // the beginning of each string in the vector.
    // At least one string in the vector will end up beginning
    // with no space characters.
    static void trim_rows_left(vector<string>& rows) {
        if(!rows.size()) return;
        auto min_space = rows.front().length();
        for(const auto& row : rows) {
            auto i = row.find_first_not_of(' ');
            if(i==string::npos) i = row.length();
            if(i == 0) return;
            if(i < min_space) min_space = i;
        }
        for(auto& row : rows) {
            row.erase(0, min_space);
    }   }

    // Dumps a representation of the tree to cout
    void PrintBST() const {
        const int d = get_max_depth();
    
        // If this tree is empty, tell someone
        if(d == 0) {
            cout << " <empty tree>\n";
            return;
        }

        // This tree is not empty, so get a list of node values...
        const auto rows_disp = get_row_display();
        // then format these into a text representation...
        auto formatted_rows = row_formatter(rows_disp);
        // then trim excess space characters from the left sides of the text...
        trim_rows_left(formatted_rows);
        // then dump the text to cout.
        for(const auto& row : formatted_rows) {
            std::cout << ' ' << row << '\n';
        }
    }
};

// Constructor
template <typename T>
BST<T>::BST() {

    numOfElements = 0;
    root = NULL;
    numOfElements = 0;
}

// Destructor
template <typename T>
BST<T>::~BST() {

    delete root;
}

// Protected: Insert
template <typename T>
Node<T> *BST<T>::Insert(Node<T> *node, T key) {

    // If BST doesn't exist
    // create a new node as root
    // or it's reached when
    // there's no any child node
    // so we can insert a new node here
    if(node == NULL) {

        node         = new Node<T>();
        node->Key    = key;
        node->Left   = NULL;
        node->Right  = NULL;
        node->Parent = NULL;
    }

    // If the given key is greater than
    // node's key then go to right subtree
    else if(node->Key < key) {

        node->Right = Insert(node->Right, key);
        node->Right->Parent = node;
    }

    // If the given key is smaller than
    // node's key then go to left subtree
    else {
        
        node->Left = Insert(node->Left, key);
        node->Left->Parent = node;
    }
    return node;
}

// Public: Insert
template <typename T>
void BST<T>::Insert(T key) {

    // Invoking Insert() function
    // and passing root node and given key
    root = Insert(root, key);
    numOfElements++;
}

// Protected: PrintTreeInOrder
template <typename T>
void BST<T>::PrintTreeInOrder(Node<T> *node) {

    // Stop printing if no node found
    if(node == NULL)
        return;

    // Get the smallest key first
    // which is in the left subtree
    PrintTreeInOrder(node->Left);

    // Print the key
    std::cout << node->Key << " ";

    // Continue to the greatest key
    // which is in the right subtree
    PrintTreeInOrder(node->Right);
}

// Public: PrintTreeInOrder
template <typename T>
void BST<T>::PrintTreeInOrder() {

    // Traverse the BST
    // from root node
    // then print all keys
    PrintTreeInOrder(root);
    std::cout << std::endl;
}

// Protected: Search
template <typename T>
Node<T> * BST<T>::Search(Node<T> *node, T key) {

    // The given key is
    // not found in BST
    if (node == NULL)
        return NULL;
    // The given key is found
    else if(node->Key == key)
        return node;
    // The given is greater than
    // current node's key
    else if(node->Key < key)
        return Search(node->Right, key);
    // The given is smaller than
    // current node's key
    else
        return Search(node->Left, key);
}

// Public: Search
template <typename T>
bool BST<T>::Search(T key) {

    // Invoking Search() operation
    // and passing root node
    Node<T> * result = Search(root, key);

    // If key is found, returns TRUE
    // otherwise returns FALSE
    return result == NULL ? false : true;
}

// Protected: FindMin
template <typename T>
T BST<T>::FindMin(Node<T> * node) {

    if(node == NULL)
        return NULL;
    else if(node->Left == NULL)
        return node->Key;
    else
        return FindMin(node->Left);
}

// Public: FindMin
template <typename T>
T BST<T>::FindMin() {

    return FindMin(root);
}

// Protected: FindMax
template <typename T>
T BST<T>::FindMax(Node<T> * node) {

    if(node == NULL)
        return NULL;
    else if(node->Right == NULL)
        return node->Key;
    else
        return FindMax(node->Right);
}

// Public: FindMax
template <typename T>
T BST<T>::FindMax() {

    return FindMax(root);
}

// Protected: Successor
template <typename T>
T BST<T>::Successor(Node<T> *node) {

    // The successor is the minimum key value
    // of right subtree
    if (node->Right != NULL) {

        return FindMin(node->Right);
    }
    // If no any right subtree
    else {

        Node<T> *parentNode  = node->Parent;
        Node<T> *currentNode = node;

        // If currentNode is not root and
        // currentNode is its right children
        // continue moving up
        while ((parentNode != NULL) &&
            (currentNode == parentNode->Right)) {

            currentNode = parentNode;
            parentNode = currentNode->Parent;
        }

        // If parentNode is not NULL
        // then the key of parentNode is
        // the successor of node
        return parentNode == NULL ?
            -1 :
            parentNode->Key;
    }
}

// Public: Successor
template <typename T>
T BST<T>::Successor(T key) {

    // Search the key's node first
    Node<T> *keyNode = Search(root, key);

    // Return the key.
    // If the key is not found or
    // successor is not found,
    // return -1
    return keyNode == NULL ? NULL : Successor(keyNode);
}

// Protected: Predecessor
template <typename T>
T BST<T>::Predecessor(Node<T> *node) {

    // The predecessor is the maximum key value
    // of left subtree
    if (node->Left != NULL) {

        return FindMax(node->Left);
    }
    // If no any left subtree
    else {

        Node<T> * parentNode  = node->Parent;
        Node<T> * currentNode = node;

        // If currentNode is not root and
        // currentNode is its left children
        // continue moving up
        while ((parentNode != NULL) &&
            (currentNode == parentNode->Left)) {

            currentNode = parentNode;
            parentNode  = currentNode->Parent;
        }

        // If parentNode is not NULL
        // then the key of parentNode is
        // the predecessor of node
        return parentNode == NULL ? NULL : parentNode->Key;
    }
}

// Public: Predecessor
template <typename T>
T BST<T>::Predecessor(T key) {

    // Search the key's node first
    Node<T> *keyNode = Search(root, key);

    // Return the key.
    // If the key is not found or
    // predecessor is not found,
    // return -1
    return keyNode == NULL ? NULL : Predecessor(keyNode);
}

// Protected: Remove
template <typename T>
Node<T> * BST<T>::Remove( Node<T> *node, T key) {

    // The given node is
    // not found in BST
    if (node == NULL)
        return NULL;

    // Target node is found
    if (node->Key == key) {

        // If the node is a leaf node
        // The node can be safely removed
        if (node->Left == NULL && node->Right == NULL)
            node = NULL;
        // The node have only one child at right
        else if (node->Left == NULL && node->Right != NULL) {

            // The only child will be connected to
            // the parent's of node directly
            node->Right->Parent = node->Parent;

            // Bypass node
            node = node->Right;
        }
        // The node have only one child at left
        else if (node->Left != NULL && node->Right == NULL) {

            // The only child will be connected to
            // the parent's of node directly
            node->Left->Parent = node->Parent;

            // Bypass node
            node = node->Left;
        }
        // The node have two children (left and right)
        else {

            // Find successor or predecessor to avoid quarrel
            int successorKey = Successor(key);

            // Replace node's key with successor's key
            node->Key = successorKey;

            // Delete the old successor's key
            node->Right = Remove(node->Right, successorKey);
        }
    }
    // Target node's key is smaller than
    // the given key then search to right
    else if (node->Key < key)
        node->Right = Remove(node->Right, key);
    // Target node's key is greater than
    // the given key then search to left
    else
        node->Left = Remove(node->Left, key);

    // Return the updated BST
    return node;
}

// Public: Remove
template <typename T>
void BST<T>::Remove(T key) {

    root = Remove(root, key);
    numOfElements--;
}

// Public: ConvertToArray
template <typename T>
T* BST<T>::ConvertToArray() {

    static T arr[100];
    Node<T> *node = Search(root, FindMin(root));
    int i=0;
    while (node != NULL) {

        arr[i] = node->Key;
        node = Search(root,Successor(node));
        i++;
    }
    return arr;
}


#endif
