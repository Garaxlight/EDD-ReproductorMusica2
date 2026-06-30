#pragma once
#include "Node.hpp"
#include <stdexcept>

template <typename T>
class Queue{
    private:
        Node<T>* head;
        Node<T>* back;

    public:
    Queue(){
        head = nullptr;
        back = nullptr;
    }

    void enqueue(T value){
        Node<T>* nuevo = new Node<T>(value);
        if(back == nullptr){
            head = back = nuevo;
        }else{
            back->next = nuevo;
            back = nuevo;
        
        }
    }

    void dequeue(){
        if (head == nullptr) return;

        Node<T>* temp = head;
        head = head->next;
        if (head == nullptr){
            back = nullptr;
        }
        delete temp;
    }


    T front(){
        if (head == nullptr){
            throw std::runtime_error("Queue vacia");
        }
        return head->data;
    }

    bool isEmpty(){
        return head == nullptr;
    }

    void pushFront(T value){
        Node<T>* nuevo = new Node<T>(value);
        if(head == nullptr){
            head = back = nuevo;
        }else{
            nuevo->next = head;
            head = nuevo;
        }
    }

    Node<T>* getHead(){
        return head;
    }

};