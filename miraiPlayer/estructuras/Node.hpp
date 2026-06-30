#pragma once

template <typename T> //nodo generico

class Node {
    public:
        T data;
         Node<T>* next;

         Node(T data){
            this->data = data;
            this->next = nullptr;
         }
};