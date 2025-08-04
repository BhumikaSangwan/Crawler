#ifndef LinkedList_H
#define LinkedList_H

#include "Node.h"
#include <iostream>

template <typename T, typename K>
class LinkedList {
    private: 
        Node<T, K>* head;
        Node<T, K>* tail;
        Node<T, K>* newNode(T data, K key); 
        Node<T, K>* getNodeAddressAtIdx(int index);

    public: 
        LinkedList();
        ~LinkedList();

        void prepend(T data, K key);
        void insert(T data, K key, int index);
        void append(T data, K key);
        void removeFromIndex(int index);
        void pop();
        void print();

};

#include "LinkedList.tpp"

#endif