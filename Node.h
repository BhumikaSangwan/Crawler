#ifndef NODE_H
#define NODE_H

template <typename T, typename K>
class Node {
    public:
    T data;
    K key;
    Node<T, K>* next;

    Node(K key, T val) : data(val), key(key), next(nullptr) {}
};

#endif