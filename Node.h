#ifndef NODE_H
#define NODE_H

template <typename T, typename K>
class Node {
    public:
    T data;
    K key;
    Node<T, K>* next;

    Node(T val, K key) : data(val), key(key), next(nullptr) {}
};

#endif