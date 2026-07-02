#pragma once
#include "Node.hpp"
#include <stdexcept>

template <typename T>

class LinkedList {
    private:
        Node<T>* head;

    public:
        LinkedList(){
            head = nullptr;
        }

        //metodos para insertar, remover, obtener, etc
        void insertEnd(T data){
            Node<T>* nuevo = new Node<T>(data);

            if (head == nullptr){
                head = nuevo;
                return;
            }

            Node<T>* actual = head;

            while(actual->next != nullptr){
                actual = actual->next;
            }

            actual->next = nuevo;
        }

        void insertFront(T data){
            Node<T>* nuevo = new Node<T>(data);
            nuevo->next = head;
            head = nuevo;
        }


        void removeFirst(){
            if (head == nullptr) return;

            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }

        void removeAt(int index){
            if (head == nullptr){
                throw std::runtime_error("Lista vacia");
            }

            //casos:
            
            if (index == 0 ){
                Node<T>* temp = head;
                head = head->next;
                delete temp;
                return;
            }

            Node<T>* actual = head;
            int i = 0;

            while (actual->next != nullptr && i < index - 1){
                actual = actual->next;
                i++;
            }

            if (actual->next == nullptr) {
                throw std::runtime_error("Indice fuera de rango");
            }

            Node<T>* temp = actual->next;
            actual->next = temp->next;
            delete temp;
        }

        T getAt (int index){
            Node<T>* actual = head;
            int i = 0;

            while(actual != nullptr){
                if (i == index ) return actual->data;
                actual = actual->next;
                i++;
            }
            throw std::out_of_range("Indice fuera de rango");
        }
        int size(){
            int count = 0;
            Node<T>* actual = head;

            while(actual != nullptr){
                count++;
                actual = actual->next;
            }
            return count;
        }

        bool isEmpty() const{
            return head == nullptr;
        }

        void clear(){
            while(head != nullptr){
                removeFirst();
            }
        }

        Node<T>* getHead(){
            return head;
        }

        const Node<T>* getHead() const{
            return head;
        }

        void insertAt(int index, T data){
            if (index == 0) {
                insertFront(data);
                return;
            }

            Node<T>* actual = head;
            int i = 0;

            while (actual->next != nullptr && i < index - 1){
                actual = actual->next;
                i++;
            }

            if (i != index - 1) {
                throw std::out_of_range("Indice fuera de rango");
            }

            Node<T>* nuevo = new Node<T>(data);
            nuevo->next = actual->next;
            actual->next = nuevo;
        }
};