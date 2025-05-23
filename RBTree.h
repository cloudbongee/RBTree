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
    void remove(const K& k);                                                                                // TODO

    // join:
    // ~ given two Trees T1, T2, with T1 having strictly lesser keys than
    // ~ T2. Outputs one tree containing both's keys
    static Red_black_tree<K, V> join(const Red_black_tree<K, V>* T1, const Red_black_tree<K, V>* T2);       // TODO

    // split:
    // Given a tree T and a key K, returns two trees T1, T2, where T1 keys are
    // lesser or equal than k, but T2 keys are strictly greater than k
    static Red_black_tree<K, V> split(const Red_black_tree<K, V>* T, const K& k);                                       // TODO

    // MINIMUM AND MAXIMUM SEARCH FUNCTIONS
    // Theorem: leftmost and rightmost node are respectively the least and greatest keys from a
    // set of keys inserted in the tree.
    // proof by contradiction. (k is leftmost -> k is least)
    // Assume k is leftmost but then it is not the least.
    // Assume it is placed in any binary tree T.
    // WLOG, assume we have placed all the
    V val_at_min();

    V val_at_max();

    K min_key();

    K max_key();

    // previous:
    // previous(T,k) = max{ h | h < k, h in T }
    V previous(const K &k);                                                                          // TODO

    // previous:
    // previous(T,k) = min{ h | h > k, h in T }
    V next(const K &k);                                                                              // TODO

    // find:
    // ~ the find operation retrieves the associated value to
    // given key k
    V find(K k);

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

        // defective_tree flags the possibility of the tree not being built correctly
        // it also is a tool in balancing cases where the tree has to be balanced up to down
        // or in deletion cases.
        bool defective_tree;

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


        // POOL is a temporary storage and retrieval for
        // node deletion.
        Node* POOL = new Node[64];

        // dump_pool method:
        // dumps the pool content by permanent deletion
        void dump_pool();

        // retrieve pool method:
        // reinserts the pool content
        void retrieve_pool();

        // retrieve_inidividual method:
        // saves last node from the pool's content
        void retrieve_individual();

        // NIL node represents end of the tree: null value reference.
        static Node* NIL;

        // root node to the tree
        Node* root;

        // TODO (I am sleepy)
        // returns node at minimum position from node N
        static Node* minNode(Node* N);

        // TODO (I am sleepy)
        // returns node at maximum position from node N
        static Node* maxNode(Node* N);


        // insert method:
        // This overload is a helper function for the original insertion
        static Node* insert(Node* N, const K &k, const V &v);

        // balance_insertion:
        // This is a helper function to the insert helper method
        // permits preserving the color invariance of the nodes
        static Node* balance_insertion(Node* N, const K& k);

        // remove:
        // This is a helper function to the original remove method
        static Node* remove(Node* N, const K &k);   // TODO

        // balance_removal:
        // this is a helper function to the remove helper function
        // maintains color imbalance after having removed one node
        static Node* balance_removal(Node* N, Node* M);  // TODO

        // remove_only_child_parent:
        // deletes one node with at most one child
        static Node* remove_only_child_parent(Node* P, Node* C);

        // remove_stressed_parent:
        // deletes one node with two children
        static Node* remove_stressed_parent(Node* P, Node* C);


        // join: TODO (I am sleepy)
        // this overload to the public join function permits for the
        // conjoining of nodes instead of trees.
        static Red_black_tree<K, V> join(const Node* T1, const Node* T2);

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