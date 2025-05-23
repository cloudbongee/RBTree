#include "RBTree.h"

// RBTree library by Jaime Meyer Beilis Michel.
// Last edit: 18 May 2025
// with a small headache.

// Since it makes use of the constexpr
// This is only available for C++ 17


// NODE CODE!! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename K, typename V>
typename Red_black_tree<K, V>::Node* Red_black_tree<K, V>::NIL = new Node(K{}, V{}, NULL, NULL, false);

// Constructor overload 1:
// Creates the default red key value pair node.
template<typename K, typename V>
::Red_black_tree<K, V>::Node::Node(const K &k, const V &v) {
    key = k;
    val = v;
    left = NIL;
    right = NIL;
    color = red;
}

// Constructor overload 2:
// creates the red key value pair node, with specified children.
template<typename K, typename V>
::Red_black_tree<K, V>::Node::Node(const K& k, const V& v, Node* l, Node* r) {
    key = k;
    val = v;
    left = l;
    right = r;
    color = red;
}


// Constructor overload 3
// creates a node with all fields specified.
template<typename K, typename V>
::Red_black_tree<K, V>::Node::Node(const K& k, const V& v, Node* l, Node* r, bool c) {
    key = k;
    val = v;
    left = l;
    right = r;
    color = c;
}

// Deletion method
// destroy node, associated key value pair, and all associated subtree.
template<typename K, typename V>
::Red_black_tree<K,V>::Node::~Node() {


    if constexpr (is_pointer_K && is_destructible_K) {
        delete key;
    }
    if constexpr (is_pointer_V && is_destructible_V) {
        delete val;
    }

    // this will recursively delete all other nodes
    // by calling deletion method on both sides.
    delete left;
    delete right;
}

// to_string method:
// ~ returns the representation (k,v), where k and v are the strings
// ~ attributed to key and value (whatever they are)
template<typename K, typename V>
std::string Red_black_tree<K,V>::Node::to_string() {
    return "(" + std::to_string(key) + "," + std::to_string(val) + ")";
}


// RED BLACK TREE CODE! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<typename K, typename V>
std::string Red_black_tree<K,V>::to_string(order print) {
    if (print == INORDER) {
        return inorder_print(root);
    }
    if (print == PREORDER) {
        return preorder_print(root);
    }
    return postorder_print(root);

}

template<typename K, typename V>
std::string Red_black_tree<K,V>::inorder_print(Node* N) {
    if (N == NIL) {
        return "NIL";
    }
    return "[" + inorder_print(N->left) + "," + N->to_string() +"," + inorder_print(N->right) + "]";
}

template<typename K, typename V>
std::string Red_black_tree<K,V>::preorder_print(Node* N) {
    if (N == NIL) {
        return "NIL";
    }
    return "["+N->to_string() + preorder_print(N->left)+preorder_print(N->right)+"]";
}

template<typename K, typename V>
std::string Red_black_tree<K,V>::postorder_print(Node* N) {
    if (N == NIL) {
        return "NIL";
    }
    return "[" + postorder_print(N->left) + postorder_print(N->right) +  N->to_string() + "]";
}


// Initializer for the red black tree
// makes the node immediately NIL
template<typename K, typename V>
Red_black_tree<K, V>::Red_black_tree() {
    root = NIL;
}

// Deletion for the red black tree
// recursively calls deletion on all nodes in the tree
template<typename K, typename V>
Red_black_tree<K, V>::~Red_black_tree() {
    delete root;
}


// find:
// ~ the find operation retrieves the associated value to
// ~ given key k (Implemented iteratively)
template<typename K, typename V>
V Red_black_tree<K, V>::find(K k) {
    Node* n = root;
    // while node isn't at the end of the tree
    while (n != NIL) {
        // otherwise, if key is smaller, then turn left way
        if (k < n->key) {
            n = n->left;
        }
        // otherwise turn right way
        else if (k > n->key) {
            n = n->right;
        }else {
            return n->val;
        }
    }
    // BUT THE POINTER SAFETY!!! (I am not sorry)
    return NULL;
}


// balancing subsection of the functions
// all this section of codes starting here corresponds
// to functions and utils that implement the balancing part
// of the left right tree.


// rotate_right:
// makes a node rotation, interchanging
// left node to the root
template<typename K, typename V>
typename Red_black_tree<K, V>::Node* Red_black_tree<K, V>::rotate_right(Node* N) {
                            // assuming N as current root.
    Node* w = N->left;       // hold left node
    N->left = w->right;     // make current root's left, left node's right subtree
    w->right = N;           // make held node's right our current root
    return w;               // return held node as rooting tooting root
}

// rotate_left:
// makes a node rotation, interchanging
// right node to the root
template<typename K, typename V>
typename Red_black_tree<K, V>::Node* Red_black_tree<K, V>::rotate_left(Node* N) {
    // inverse process as right is applied
    Node* w = N->right;
    N->right = w->left;
    w->left = N;
    return w;
}

// flip_color:
// if red make N black, else make N red
template<typename K, typename V>
void Red_black_tree<K, V>::flip_color(Node *N) {
    N->color = !N->color;
}

// interchange_left_color:
// left child exchange
template<typename K, typename V>
void Red_black_tree<K, V>::interchange_left_color(Node *N) {
    bool c = N->color;
    N->color = N->left->color;
    N->left->color = c;
}

// interchange_right_color:
// right child exchange
template<typename K, typename V>
void Red_black_tree<K, V>::interchange_right_color(Node *N) {
    bool c = N->color;
    N->color = N->right->color;
    N->right->color = c;
}

// interchange_both_children_color:
// left and right child exchange
template<typename K, typename V>
void Red_black_tree<K, V>::interchange_both_children_color(Node *N) {
    bool c = N->color;
    N->color = N->left->color;
    N->left->color = c;
    N->right->color = c;
}

// balance_insertion:
// Taking N as a grandparent node, ensures invariance is kept.
// 10 insertion balance cases, let the new node be N:
//      1) No parent of N, therefore N is the root, nothing to do, root is automatically black by insertion call
//      2) Parent is black, therefore the current insertion is balanced, nothing is to be done
//      3) Parent is red left, uncle is red, grandparent is black, N is left
//      4) Parent is red left, uncle is red, grandparent is black, N is right
//              Cases 3,4 are analyzed from the grandparent. Which is flipped
//              with parent and uncle to maintain balance. Then imbalance is solved upwards.
//      5) Parent is red left, uncle is black (maybe NIL), N is left
//      6) Parent is red left, uncle is black (maybe NIL), N is right
//              Only one recolor operation is done upwards for 5,6, corresponding to N's insertion side
//      7, 8, 9, 10) Parent is right for 3, 4, 5, and 6
// balance method taken from my professor Dr. Kececioglu.
// Not without studying them before. I promise Dr. K!
template<typename K, typename V>
typename Red_black_tree<K, V>::Node* Red_black_tree<K, V>::balance_insertion(Node *N, const K& k) {
    // if grandparent color is black but not NIL
    if (N->color == black && N != NIL) {
       // cases 3,4,5,6: parent is left
       if(k < N->key) {

            Node* parent = N->left;

            // if parent is red
            if(parent->color == red) {
                // and both second generation are red
                if(N->right->color == red){
                    // cases 3,4 if either child inserted is right or left and the correspondent
                    // added node is red.
                    if( (k < parent->key && parent->left->color == red) ||
                        (k > parent->key && parent->right->color == red)) {
                        // make parent and uncle black. Pass any red imbalance
                        // to the grandparent.
                        interchange_both_children_color(N);
                    }
                }
                // case 5 is completed if uncle is black, k was added to the left
                else if( k < parent->key && parent->left->color == red){
                    // then we make parent black
                    interchange_left_color(N);
                    // and we rotate grandparent to the right
                    N = rotate_right(N);
                }
                // case 6 is completed if uncle is black, k was added to the right
                else if( k < parent->key && parent->right->color == red){
                    // then we make parent black
                    interchange_right_color(N);
                    // and we rotate grandparent to the left
                    N = rotate_left(N);
                }
            }

       }
       // cases 7,8,9,10: parent is right
       else {

            Node* parent = N->right;
            // all the rest are symmetric cases.
            if(parent->color == red) {
                if(N->left->color == red){
                    if( (k < parent->key && parent->left->color == red)
                     || (k > parent->key && parent->right->color == red)) {
                        interchange_both_children_color(N);
                     }
                }
                else if(k > parent->key && parent->right->color == red){
                    interchange_right_color(N);
                    N = rotate_left(N);
                }
                else if( k < parent->key && parent->left->color == red){
                    // this is a way to mimic previous case balancing!
                    N->right = rotate_right(N->right);
                    interchange_right_color(N);
                    N = rotate_left(N);
                }
            }
       }
    }
    return N;
}

template<typename K, typename V>
typename Red_black_tree<K, V>::Node* Red_black_tree<K, V>::balance_removal(Node* P, Node* C) {
    return NIL; // TODO (I am sleepy)
}
// end of balancing subsection of the functions


// ~~~~~~~~~~~~~~~ Insertion and deletion method of the function:


// insert method:
// The insert method creates a node from key K (type) k and value V (type) v
// then proceeds to ensure tree balance.
template<typename K, typename V>
void Red_black_tree<K,V>::insert(const K& k, const V& v) {
    root = insert(root, k, v);
    root->color = black;
}

// insert method: (private overload)
// Follows the typical insertion routine to a binary tree, then returns such result
// as based on the balancing function.
template<typename K, typename V>
typename Red_black_tree<K, V>::Node* Red_black_tree<K, V>::insert(Node* N, const K& k, const V& v) {
    if(N == NIL){
        return new Node(k, v);
    }

    if(N->key < k){
        N->right = insert(N->right, k, v);
    }
    else if (N->key > k){
        N->left = insert(N->left, k, v);
    }
    else {
        N->val = v;
        return N;
    }

    return balance_insertion(N, k);
}

// remove method:
// calls for removal of a node in the tree.
template<typename K, typename V>
void Red_black_tree<K, V>::remove(const K& k) {
    // flag non defective tree by default
    defective_tree = false;
    // call the remove helper
    root = remove(root, k);
    // balance root black invariance
    root->color = black;
}

// remove method: (private overload):
// follows typical removal procedure of a node in a tree
template<typename K, typename V>
typename Red_black_tree<K, V>::Node* Red_black_tree<K, V>::remove(Node* N, const K& k) {
    // if node is NIL return NIL
    if (N == NIL) {
        return NIL;
    }
    // traverse, replace accordingly (right)
    if (N->key < k) {
        N->right = delete(N->right, k);
        return balance_remove(N, N->right);
    }
    // traverse, replace accordingly (left)
    if (N->key > k) {
        N->left = delete(N->left, k);
        return balance_remove(N, N->left);
    }

    // Else case, key is equal and only right child exists
    if (N->left == NIL) {
        return remove_only_child_parent(N, N->right);
    }

    // Else case, key is equal and only left child exists
    if (N->right == NIL) {
        return remove_only_child_parent(N, N->left);
    }

    // Else case, both children exist, such that we call a subcase
    // that accounts for both.
    N->right = remove_stressed_parent(N->right, N);
    return balance_remove(N, N->right);

}

// removes a node with at most one child
template<typename K, typename V>
typename Red_black_tree<K, V>::Node* Red_black_tree<K,V>::remove_only_child_parent(Node* P, Node* C) {
    return NIL; // TODO (i am sleepy)
}

// removes a node with both children
template<typename K, typename V>
typename Red_black_tree<K, V>::Node* Red_black_tree<K,V>::remove_stressed_parent(Node* P, Node* C) {
    return NIL; // TODO (i am sleepy)
}