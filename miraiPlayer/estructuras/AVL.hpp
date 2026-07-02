#pragma once
#include <string>
#include "LinkedList.hpp"

template <typename T>
struct AVLNode {
    T data;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(T value) : data(value), left(nullptr), right(nullptr), height(1) {}
};

template <typename T>
class AVL {
private:
    AVLNode<T>* root;
    
    int getHeight(AVLNode<T>* node) const {
        return (node == nullptr) ? 0 : node->height;
    }
    
    int getBalance(AVLNode<T>* node) const {
        return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
    }
    
    void updateHeight(AVLNode<T>* node) {
        if (node != nullptr) {
            int leftH = getHeight(node->left);
            int rightH = getHeight(node->right);
            node->height = 1 + (leftH > rightH ? leftH : rightH);
        }
    }
    
    AVLNode<T>* rotateRight(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }
    
    AVLNode<T>* rotateLeft(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        updateHeight(x);
        updateHeight(y);
        
        return y;
    }
    
    AVLNode<T>* insertNode(AVLNode<T>* node, const T& value) {
        if (node == nullptr) {
            return new AVLNode<T>(value);
        }
        
        // Comparar strings para inserción ordenada
        if (value->nombre < node->data->nombre) {
            node->left = insertNode(node->left, value);
        } else if (value->nombre > node->data->nombre) {
            node->right = insertNode(node->right, value);
        } else {
            return node; // Duplicado
        }
        
        updateHeight(node);
        
        int balance = getBalance(node);
        
        // Left Left
        if (balance > 1 && value->nombre < node->left->data->nombre) {
            return rotateRight(node);
        }
        
        // Right Right
        if (balance < -1 && value->nombre > node->right->data->nombre) {
            return rotateLeft(node);
        }
        
        // Left Right
        if (balance > 1 && value->nombre > node->left->data->nombre) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Right Left
        if (balance < -1 && value->nombre < node->right->data->nombre) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    void inorderTraversal(AVLNode<T>* node, LinkedList<T>& result) const {
        if (node == nullptr) return;
        inorderTraversal(node->left, result);
        result.insertEnd(node->data);
        inorderTraversal(node->right, result);
    }
    
    void deleteTree(AVLNode<T>* node) {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    
public:
    AVL() : root(nullptr) {}
    
    ~AVL() {
        deleteTree(root);
    }
    
    void insert(const T& value) {
        root = insertNode(root, value);
    }
    
    void inorder(LinkedList<T>& result) const {
        // Limpiar resultado
        while (!result.isEmpty()) {
            result.removeFirst();
        }
        inorderTraversal(root, result);
    }
    
    bool isEmpty() const {
        return root == nullptr;
    }
    
    void clear() {
        deleteTree(root);
        root = nullptr;
    }
};
