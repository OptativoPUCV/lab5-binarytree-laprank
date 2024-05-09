#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}

    //new->lower_than = lower_than


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL || value == NULL) return;
    if (tree->root == NULL){
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
    }
    else{
        TreeNode * aux = tree->root;
        TreeNode * parent = NULL;
        while (aux != NULL){
            parent = aux;
            if (is_equal(tree, key, aux->pair->key) == 1) return;
            if (tree->lower_than(key, aux->pair->key) == 1) aux = aux->left;
            else aux = aux->right;
        }
        TreeNode * new = createTreeNode(key, value);
        new->parent = parent;
        if (tree->lower_than(key, parent->pair->key) == 1) parent->left = new;
        else{
            parent->right = new;
        }
        tree->current = new;
    }
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL) x = x->left;
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL) return;

    // El nodo es una hoja (no tiene hijos)
    if (node->left == NULL && node->right == NULL){
        if (node == node->parent->left) node->parent->left = NULL;
        else node->parent->right = NULL;
        free(node);
        return;
    }

    // El nodo tiene un solo hijo
    if (node->left != NULL && node->right == NULL) {
        // El nodo solo tiene un hijo izquierdo
        if (node == node->parent->left) {
            node->parent->left = node->left;
        } else {
            node->parent->right = node->left;
        }
        node->left->parent = node->parent;
        free(node);
        return;
    } else if (node->left == NULL && node->right != NULL) {
        // El nodo solo tiene un hijo derecho
        if (node == node->parent->left) {
            node->parent->left = node->right;
        } else {
            node->parent->right = node->right;
        }
        node->right->parent = node->parent;
        free(node);
        return;
    }

    // El nodo tiene dos hijos
    if (node->left != NULL && node->right != NULL) {
        // El nodo tiene dos hijos
        TreeNode *successor = minimum(node->right);  // Asume que 'minimum' devuelve el nodo más pequeño en un subárbol
        node->pair->key = successor->pair->key;
        node->pair->value = successor->pair->value;
        removeNode(tree, successor);
        return;
    }
}
void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;
    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}



Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    while (aux != NULL){
        if (is_equal(tree, key, aux->pair->key) == 1){
            tree->current = aux;
            return aux->pair;
        }
        if (tree->lower_than(key, aux->pair->key) == 1) aux = aux->left;
        else aux = aux->right;
    }
    tree->current = NULL;
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    TreeNode * ub_node = NULL;
    while (aux != NULL){
        if (is_equal(tree, key, aux->pair->key) == 1){
            tree->current = aux;
            return aux->pair;
        }
        if (tree->lower_than(key, aux->pair->key) == 1){
            ub_node = aux;
            aux = aux->left;
        }
        else{
            aux = aux->right;
        }
    }
    if (ub_node != NULL) {
        tree->current = ub_node;
        return ub_node->pair;
    } else {
        return NULL;
    }
}


Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    while (aux->left != NULL){
        aux = aux->left;
    }
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;
    TreeNode * aux = tree->current;
    if (aux->right != NULL){
        aux = aux->right;
        while (aux->left != NULL){
            aux = aux->left;
        }
        tree->current = aux;
        return aux->pair;
    }
    TreeNode * parent = aux->parent;
    while (parent != NULL && aux == parent->right){
        aux = parent;
        parent = parent->parent;
    }
    tree->current = parent;
    if (parent != NULL) return parent->pair;
    return NULL;
}
