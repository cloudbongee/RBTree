#ifndef RBTREE_LIB_H
#define RBTREE_LIB_H

#include <iostream>
#include <string>
// RBTree library by Jaime Meyer Beilis Michel.
// Last edit: 18 May 2025
// in a very sweaty summer.

// Red_black_tree <K,V>:
// K (data type) : With an ordering and equivalence (<, >, ==) defined,
// K is utilized for comparing the keys in the red black tree
// V (data type) : Stores the values of the red black tree
//
// The red-black tree data structure consists of a self-balancing
// binary tree, which ensures a log(n) runtime for analysis operations
template <typename K, typename V> struct Red_black_tree {

    // constructor:
    // ~ initializes an empty tree
    Red_black_tree();

    // destructor:
    // ~ deletes the tree
    ~Red_black_tree();

    // insert:
    // ~ the insert operation adds the key value pair k,v
    // to the tree
    void insert(const K& k, const V& v);

    // remove:
    // ~ the remove operation takes the node attributed to
    // the key value pair specified by a given k.
    void remove(const K& k);

    // find:
    // ~ the find operation retrieves the associated value to
    // given key k
    V find(const K& k);

    enum order { INORDER, PREORDER, POSTORDER};
    // to_string:
    // returns representation based on given argument
    std::string to_string(order print);


    private:

        // Color invariance of a red black tree
        // the following properties need to be met:
        // > All nodes must have a color, red or black
        // > Root is strictly black
        // > NIL is strictly black
        // > Red nodes cannot have red children
        // > Black nodes can have any color children
        // > From any fixed node to NIL, there is the same amount of black nodes
        // > All new nodes are automatically red until balancing
        static constexpr bool red   = true;
        static constexpr bool black = false;

        static constexpr bool is_pointer_K = std::is_pointer<K>::value;
        static constexpr bool is_pointer_V = std::is_pointer<V>::value;
        static constexpr bool is_destructible_K = std::is_destructible<K>::value;
        static constexpr bool is_destructible_V = std::is_destructible<V>::value;
        // Node struct:
        // ~ key : used as identifier of information
        struct Node {

            // Default constructor
            Node(const K& k, const V& v);

            // overloads 1, 2, make from specified fields.
            Node(const K& k, const V& v, Node* left, Node* right);
            Node(const K &k, const V &v, Node *left, Node *right, bool color);

            // recursive deletion
            ~Node();

            // to_string method
            std::string to_string();

            // Key value pair fields
            K key;
            V val;

            // Left and right node pointers
            Node* right;
            Node* left;

            // color invariant
            bool color;


        };

        // NIL node represents end of the tree: null value reference.
        static Node* NIL;

        // root node to the tree
        Node* root;

        static Node* insert(Node* N, const K &k, const V &v);

        static Node* balance_insertion(Node* N, const K& k);

        // rotate_right:
        // makes a node rotation, interchanging
        // left node to the root
        static Node* rotate_right(Node* N);

        // rotate_left:
        // makes a node rotation, interchanging
        // right node to the root
        static Node* rotate_left(Node* N);

        // flip_color:
        // if red make N black, else make N red
        static void flip_color(Node* N);

        // interchange_right_color:
        // right child exchange
        static void interchange_right_color(Node* N);

        // interchange_left_color:
        // left child exchange
        static void interchange_left_color(Node* N);

        // interchange_both_children_color:
        // left and right child exchange
        static void interchange_both_children_color(Node* N);

        // private printing functions
        static std::string preorder_print(Node* N);
        static std::string postorder_print(Node* N);
        static std::string inorder_print(Node* N);
};

#include "RBTree.impl.h"
#endif //RBTREE_LIB_H