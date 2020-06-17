#ifndef __BST_CPP
#define __BST_CPP
#include "bst.h"
#include <iostream>
#include <queue>

template <class T>
node<T>* searchHelper(T key, node<T>* p){
    if (p == NULL) //recursion stopping condition
        return NULL;

    if (p->key > key){ //current node's key is > than key being searched, recurse left
        searchHelper(key, p->left);
    }
    else if (p->key < key){ //current node's key is < than key being searched, recurse right
        searchHelper(key, p->right);
    }
    else if (p->key == key){ //key being searched matches the current node's key
        //node found
        return p;
    }
}

// HELPER FUNCTIONS HERE...
template <class T>
BST<T>::BST(){
	root = NULL;
}

template <class T>
BST<T>:: ~BST(){
    // your destructor code goes here
    while (root != NULL){
        delete_node(root->key);
    }
}

template <class T>
int BST<T> :: height (node<T>* p){
   // your find height code goes here
   if (p == NULL){ //stopping condition
       return 0; //height is 0 for an empty tree, NULL contributes nothing
   }
   else{
       //int left_subtree_height = 0, right_subtree_height = 0;
       int left_subtree_height = height(p->left); //every node contributes its height recursively (after traceback is done)
       int right_subtree_height = height(p->right); //every node contributes its height recursively (after traceback is done)
       
       //maximum height from the heights of the left and right subtrees (with +1 added) gives height of current node
       if (left_subtree_height > right_subtree_height)
          p->height = left_subtree_height + 1;
       else //if they are equal, returning any would suffice
          p->height = right_subtree_height + 1;

       return p->height; //height of tree = height of root
   }
}

template <class T>
int BST<T> :: balanceFactor(node<T>* p){
    // you can use this to find balance factor.
    // difference in heights of subtrees (right-left), left-heavy -> bf = -1, right-heavy -> bf = +1, equal -> bf = 0
    if (p != NULL){
        int right_subtree_height = 0, left_subtree_height = 0; //0 by default (NULL)
        
        if (p->left != NULL)
            left_subtree_height = p->left->height;
        
        if (p->right != NULL)
            right_subtree_height = p->right->height;

        return right_subtree_height-left_subtree_height;
    }
    else{
        return 0; // return 0 for NULL (no node exists)
    }
}

template <class T>
void BST<T> :: fixHeight(node<T>* p){
    // use this function to fix height of a node after insertion or deletion.
    if (balanceFactor(p) != 0)
        p->height = height(p);
}

// ROTATING HELPERS
template <class T>
node<T>* BST<T> :: rotateleft(node<T>* p){
    // Rotate left code goes here.
    /* 
    0,1 nodes -> nothing happens
    2,3 nodes -> parent becomes left child of its right child, and right child replaces parent (drags its own right child along)
    
    generally =>
    IF right child has a left child then that becomes the parent's right child else NULL
    right child becomes new parent (left child is old parent, right child becomes old right child IF IT EXISTED, no adjustments needed)
    */

    if (p == NULL){
        return NULL; //nothing happens
    }
    else if (p->left == NULL && p->right == NULL){ //root node only node present
        return p; //cannot rotate a single node, return back
    }
    else { //rotate left
        node<T>* p_old_right = p->right;
        //if parent's old right child had a left child it becomes the parent's right child else null
        p->right = p_old_right->left; 
        //parent becomes its old right child's new left child
        p_old_right->left = p;
        //no need to adjust the old right's right child, it stays where it is if it exists
        
        //fix heights for the nodes that shifted
        fixHeight(p);
        fixHeight(p_old_right);
        return p_old_right; //the parent's old right child is now the new parent of the subtree
    }
}

template <class T>
node<T>* BST<T> :: rotateright(node<T>* p){
    // Rotate right code goes here
    if (p == NULL){
        return NULL; //nothing happens
    }
    else if (p->left == NULL && p->right == NULL){ //root node only node present
        return p; //cannot rotate a single node, return back
    }
    else { //rotate right
        node<T>* p_old_left = p->left;
        //if parent's old left child had a right child it becomes the parent's left child else null
        p->left = p_old_left->right; 
        //parent becomes its old left child's new right child
        p_old_left->right = p;
        //no need to adjust the old left's left child, it stays where it is if it exists
        
        //fix heights for the nodes that shifted
        fixHeight(p);
        fixHeight(p_old_left);
        return p_old_left; //the parent's old left child is now the new parent of the subtree
    }
}

template <class T>
node<T>* BST<T> :: balance(node<T>* p){
    // Balancing code goes here. You might need to find balance factor and call appropriate rotations.
    /*
    if (p == NULL){ //empty tree, condition not needed
        return NULL;
    }
    */

    int p_bf = balanceFactor(p); //accepted bf for p is -1/1/0

    //Left-heavy 
    if (p_bf < -1){
        // left sub-branch weighs more to the left
        int p_left_bf = balanceFactor(p->left);
        if (p_left_bf < 0){ //LL case -- rotateright(unbalancedNode)
            p = rotateright(p);    
        }
        // left sub-branch weighs more to the right
        else if (p_left_bf > 0){ //LR case -- rotateleft(unbalancedNode's left) -- rotateright(unbalancedNode)
            p->left = rotateleft(p->left);
            p = rotateright(p);
        }
    }

    //Right-heavy, similarly
    else if (p_bf > 1){
        int p_right_bf = balanceFactor(p->right);
        if (p_right_bf > 0){  //RR case -- rotateleft(unbalancedNode)
                p = rotateleft(p);    
        }
        else if (p_right_bf < 0){  //RL case -- rotateright(unbalancedNode's right) -- rotateleft(unbalancedNode)
            p->right = rotateright(p->right);
            p = rotateleft(p);
        }
    }

    return p;
}


// CLASS FUNCTIONS HERE
template <class T>
void BST<T> :: insert(string value,T k){
    root = insertHelper(value,k,root);
}

// insertion helper
template <class T>
node<T>* BST<T> :: insertHelper(string value,T k, node<T> *p) { // note we need a helper because we need recursive calls
    // Insertion code goes here.
    //*p is the root
    // empty tree, make a new node to become the root node
    if (p == NULL){
         //instantiate new node
        return new node<T>(k, value); //this new node is now the root
    }
    //check current node's value, if > than key, recurse left, if < than key, recurse right
    else if (p->key > k){
        p->left = insertHelper(value, k, p->left); //current node's left subtree will be equal to the new returned subtree from the last function call during traceback
    }
    else if (p->key < k){
        p->right = insertHelper(value, k, p->right);
        //current node's right subtree will be equal to the new returned subtree from the last function call during traceback
    }

    
    fixHeight(p);
    if (balanceFactor(p) > 1 || balanceFactor(p) < -1)
        return balance(p); //balance every subtree/node during traceback
    else
        return p;
        
}

template<class T>
node<T>* BST<T> :: search(T key){
    // Search code goes here.
    return searchHelper(key, root);
}

// DELETE HELPER FUNCTIONS
template<class T>
node<T>* BST<T> :: findmin(node<T> *p){
    // This function finds the min node of the tree.
    // minimum value node == left-most leaf of the tree
    if (p == NULL){ //empty tree
        return NULL;
    }
    else{
        while (p->left != NULL){ //on reaching the last left-most node (leaf node)
        p = p->left;
        }
        return p;
    }
}

template<class T>
node<T>* BST<T>::removemin(node<T>* p) {
    if (p == NULL){ //empty tree, no node to remove
        return NULL;
    }
    else if (p->left != NULL){
        p->left = removemin(p->left);    
    }
    else if (p->left == NULL){
        delete p;
        if (p->right != NULL){ //case with 1 node on right
            return p->right; //its right child will replace it   
        }
        else{
            return NULL; //otherwise just return null (leaf node deleted)
        }
    }
    
    fixHeight(p);
    return balance(p); //balanced for AVL
    // This function recursively finds the min node and deletes it.
}

template<class T>
void BST<T>::delete_node(T k){
    root = remove(root, k);
}

template<class T>
node<T>*  BST<T>::remove(node<T>* p, T k) // k key deletion from p tree
{	
    /*
    Find node to be deleted,
    apply cases: 
    1. leaf node to be deleted, 
    2. node with 1 child to be deleted, 
    3. node with 2 children to be deleted
    */

    if (p == NULL){ //stopping condition and for empty tree
        return NULL;
    }
    else if (p->key > k){
        p->left = remove(p->left, k);
    }
    else if (p->key < k){
        p->right = remove(p->right, k); //if node connected to all right subtree is returned then it is linked to right ptr as child of current node (1 recursive call up) during traceback
    }
    else if (p->key == k){ //current node's key matches key for node to be deleted 
        if (p->right != NULL && p->left != NULL){  //3. node to be deleted has 2 children
            node<T>* right_subtree = p->right;
            node<T>* right_min_node = findmin(right_subtree); //get minimum node from right subtree of current node 
            //copy contents of that minimum key value node we stored to the current node
            p->key = right_min_node->key;
            p->value = right_min_node->value;
            
            //now that the swap (copy) is complete, delete the (redundant) minimum node of the right subtree
            p->right = removemin(right_subtree); // remember, removemin will return the new right subtree of p with the deleted min node so assign it to p's right
        }
        else if (p->left != NULL){
            //2a. current node to be deleted has 1 child (left)
             // we delete the node itself, and return ITS left child to become the new replacement for ITSELF (new left child of current node's parent) 
            node<T>* p_replacement = p->left;
            delete p;
            return p_replacement;
        }
        else if (p->right != NULL){
            //2b. current node to be deleted has 1 right child only
            //same as left (now the parent's right ptr points to the delete node's right child as its old child does not exist)
            node<T>* p_replacement = p->right;
            delete p;
            return p_replacement;         
        }
        else { //both null then 1. leaf node or only existing root node
            delete p;   //simply delete
            return NULL; //now where that node was supposed to be it will return NULL (to the pointer to it)
        }
    }

    //return the current node p, since this same p's subtrees may have been modified
    fixHeight(p);
    return balance(p); //balanced for AVL
}

template<class T>
node<T>* BST<T>::getRoot(){
    return root;
}
#endif
