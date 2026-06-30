#pragma once
#include "Node.hpp"
#include <stdexcept>
template <typename T>
class Stack{
    private:
        Node<T>* topNode;

    public:
        Stack(){
            topNode = nullptr;
        }

        void push(T value){
            Node<T>* nuevo = new Node<T>(value);
            nuevo->next = topNode;
            topNode = nuevo;
        }

        void pop(){
            if (topNode==nullptr){return;}

            Node<T>* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }

        T top(){
            if (topNode == nullptr){
                throw std::runtime_error("Stack vacio");

            }
            return topNode->data;
        }

        bool isEmpty(){
            return topNode == nullptr;
        }
};