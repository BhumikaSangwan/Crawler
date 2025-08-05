#include <iostream>
#include "Node.h"
using namespace std;

template <typename T, typename K>
LinkedList<T, K>::LinkedList() : head(nullptr), tail(nullptr) {}

template <typename T, typename K>
LinkedList<T, K>::~LinkedList()
{
    Node<T, K> *node = head;
    while (node != nullptr)
    {
        Node<T, K> *next = node->next;
        delete node;
        node = next;
    }
}

template <typename T, typename K>
Node<T, K> *LinkedList<T, K>::newNode(T data, K key)
{
    return new Node<T, K>(data, key);
}

template <typename T, typename K>
Node<T, K> *LinkedList<T, K>::getNodeAddressAtIdx(int index)
{
    if (index == 0)
    {
        return head;
    }
    Node<T, K> *tempNode = head;
    for (int i = 0; i < index && tempNode != nullptr; i++)
    {
        tempNode = tempNode->next;
    }
    return tempNode;
}

template <typename T, typename K>
void LinkedList<T, K>::prepend(T data, K key)
{
    Node<T, K> *node = newNode(data, key);
    if (head == nullptr)
    {
        head = node;
        tail = node;
        return;
    }
    node->next = head;
    head = node;
}

template <typename T, typename K>
void LinkedList<T, K>::insert(T data, K key, int index)
{
    if (index == 0)
    {
        prepend(data, key);
        return;
    }
    Node<T, K> *node = newNode(data, key);
    Node<T, K> *prevNode = getNodeAddressAtIdx(index - 1);
    node->next = prevNode->next;
    prevNode->next = node;
}

template <typename T, typename K>
void LinkedList<T, K>::append(T data, K key)
{
    Node<T, K> *node = newNode(data, key);
    if (head == nullptr)
    {
        head = node;
        tail = node;
    }
    else
    {
        tail->next = node;
        tail = tail->next;
    }
}

template <typename T, typename K>
void LinkedList<T, K>::removeFromIndex(int index)
{
    Node<T, K> *tempNode = nullptr;
    if (index == 0)
    {
        tempNode = head;
        head = head->next;
    }
    else
    {
        Node<T, K> *prevNode = getNodeAddressAtIdx(index - 1);
        tempNode = prevNode->next;
        prevNode->next = prevNode->next->next;
    }
    delete tempNode;
}

template <typename T, typename K>
void LinkedList<T, K>::pop()
{
    if (!head)
        return;

    if (head == tail)
    {
        delete head;
        head = nullptr;
        tail = nullptr;
        return;
    }

    Node<T, K> *node = head;
    while (node->next != tail)
    {
        node = node->next;
    }

    delete tail;
    tail = node;
    tail->next = nullptr;
}

template <typename T, typename K>
void LinkedList<T, K>::deleteKey(K key)
{
    if (!head)
        return;

    Node<T, K> *node = head;
    if (node->key == key)
    {
        removeFromIndex(0);
        return;
    }

    Node<T, K> *prev = nullptr;
    while (node != nullptr && node->key != key)
    {
        prev = node;
        node = node->next;
    }
    if (node)
    {
        prev->next = node->next;
        delete node;
    }
}

template <typename T, typename K>
void LinkedList<T, K>::print()
{
    Node<T, K> *node = head;
    int len = 0;
    while (node != nullptr)
    {
        std::cout << node->data << ", ";
        if (node->key != nullptr)
            std::cout << node->key;
        else
            std::cout << "NULL";
        std::cout << std::endl;
        node = node->next;
    }
}

template <typename T, typename K>
Node<T, K> *LinkedList<T, K>::search(K key)
{
    Node<T, K> *node = head;
    while (node != nullptr)
    {
        if (node->key == key)
        {
            return node;
        }
        node = node->next;
    }
    return nullptr;
}