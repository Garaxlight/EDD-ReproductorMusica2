#pragma once

template <typename T>
class Heap {
private:
    T* elements;
    int size;
    int capacity;
    
    
    int compare(const T& a, const T& b) const {
        if (a->reproducciones != b->reproducciones) {
            return a->reproducciones > b->reproducciones ? 1 : -1;
        }
        
        if (a->nombre != b->nombre) {
            return a->nombre < b->nombre ? 1 : -1;
        }
        // Por artista
        return a->artista < b->artista ? 1 : -1;
    }
    
    int getParentIndex(int i) const {
        return (i - 1) / 2;
    }
    
    int getLeftChildIndex(int i) const {
        return 2 * i + 1;
    }
    
    int getRightChildIndex(int i) const {
        return 2 * i + 2;
    }
    
    void expandCapacity() {
        if (capacity == 0) capacity = 10;
        else capacity *= 2;
        
        T* newElements = new T[capacity];
        for (int i = 0; i < size; i++) {
            newElements[i] = elements[i];
        }
        if (elements != nullptr) delete[] elements;
        elements = newElements;
    }
    
    void heapifyUp(int i) {
        while (i > 0 && compare(elements[i], elements[getParentIndex(i)]) > 0) {
            T temp = elements[i];
            elements[i] = elements[getParentIndex(i)];
            elements[getParentIndex(i)] = temp;
            i = getParentIndex(i);
        }
    }
    
    void heapifyDown(int i) {
        int maxIndex = i;
        int left = getLeftChildIndex(i);
        int right = getRightChildIndex(i);
        
        if (left < size && compare(elements[left], elements[maxIndex]) > 0) {
            maxIndex = left;
        }
        
        if (right < size && compare(elements[right], elements[maxIndex]) > 0) {
            maxIndex = right;
        }
        
        if (maxIndex != i) {
            T temp = elements[i];
            elements[i] = elements[maxIndex];
            elements[maxIndex] = temp;
            heapifyDown(maxIndex);
        }
    }
    
public:
    Heap() : elements(nullptr), size(0), capacity(0) {}
    
    ~Heap() {
        if (elements != nullptr) {
            delete[] elements;
            elements = nullptr;
        }
    }
    
    void insert(T element) {
        if (size >= capacity) {
            expandCapacity();
        }
        elements[size] = element;
        size++;
        heapifyUp(size - 1);
    }
    
    T extractMax() {
        if (size == 0) return nullptr;
        
        T max = elements[0];
        size--;
        
        if (size > 0) {
            elements[0] = elements[size];
            heapifyDown(0);
        }
        
        return max;
    }
    
    T getMax() const {
        if (size == 0) return nullptr;
        return elements[0];
    }
    
    bool isEmpty() const {
        return size == 0;
    }
    
    int getSize() const {
        return size;
    }
    
    void clear() {
        size = 0;
    }
    void rebuild(){
        for (int i = size / 2 - 1; i >= 0; i--){
            heapifyDown(i);
        }
    }

    Heap(const Heap<T>& other){
        size = other.size;
        capacity = other.capacity;

        if (capacity > 0){
            elements = new T[capacity];

            for (int i = 0; i < size; i++){
                elements[i] = other.elements[i];
            }
        } else {
            elements = nullptr;
        }
    }
};
