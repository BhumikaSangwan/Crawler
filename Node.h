#ifndef NODE_H
#define NODE_H

template <typename K, typename T>
class Node {
    public:
    T data;
    K key;
    Node<K, T>* next;

    Node(K key, T val) : data(val), key(key), next(nullptr) {}
};

#endif