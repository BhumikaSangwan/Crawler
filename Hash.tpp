#include <iostream>
#include <type_traits>
#include "Hash.h"
using namespace std;

template <typename K, typename T>
Hash<K, T>::Hash()
{
    size = 5;
    insertedCount = 0;
    allowResize = true;
    threshold = static_cast<int>(0.75 * size);
    table = new Node<K, T> *[size];
    for (int i = 0; i < size; i++)
    {
        table[i] = nullptr;
    }
}

template <typename K, typename T>
Hash<K, T>::Hash(int size)
{
    this->size = size;
    insertedCount = 0;
    allowResize = true;
    threshold = static_cast<int>(0.75 * size);
    table = new Node<K, T> *[size];
    for (int i = 0; i < size; i++)
    {
        table[i] = nullptr;
    }
}

template <typename K, typename T>
Hash<K, T>::Hash(const Hash<K, T> &obj)
{
    size = obj.size;
    allowResize = false;
    insertedCount = 0;
    threshold = static_cast<int>(0.75 * size);
    table = new Node<K, T> *[size];
    for (int i = 0; i < size; i++)
    {
        table[i] = nullptr;
        Node<K, T> *current = obj.table[i];
        while (current != nullptr)
        {
            insert(current->key, current->data);
            current = current->next;
        }
    }
    allowResize = true;
}

template <typename K, typename T>
Hash<K, T>::~Hash()
{
    deallocateTable();
}

template <typename K, typename T>
void Hash<K, T>::resizeTable()
{
    int prevSize = size;
    size *= 2;
    insertedCount = 0;
    threshold = static_cast<int>(0.75 * size);
    Node<K, T> **newTable = new Node<K, T> *[size];
    Node<K, T> **prevTable = table;
    for (int i = 0; i < size; i++)
    {
        newTable[i] = nullptr;
    }
    table = newTable;

    for (int i = 0; i < prevSize; i++)
    {
        if (prevTable[i] != nullptr)
        {
            Node<K, T> *currNode = prevTable[i];
            while (currNode != nullptr)
            {
                int idx = getHashIdx(currNode->key);

                Node<K, T> *nextNode = newTable[idx];
                newTable[idx] = currNode;
                newTable[idx]->next = nextNode;

                currNode = currNode->next;
            }
        }
    }

    delete[] prevTable;
}

template <typename K, typename T>
void Hash<K, T>::deallocateTable()
{
    for (int i = 0; i < size; i++)
    {
        Node<K, T> *curr = table[i];
        while (curr != nullptr)
        {
            Node<K, T> *next = curr->next;
            delete curr;
            curr = next;
        }
    }
    delete[] table;
    table = nullptr;
}

template <typename K, typename T> 
int Hash<K, T> :: getHashIdx(int val) {
    return (val % size + size) % size;
}

template <typename K, typename T>
int Hash<K, T> :: getHashIdx(float val) {
    int idx = static_cast<int>(val);
    return (idx % size + size) % size;
}

template <typename K, typename T> 
int Hash<K, T> :: getHashIdx(char* val) {
    int idx = 0;
    for (int i = 0; val[i] != '\0'; i++)
    {
        idx = idx * 31 + static_cast<unsigned char>(val[i]);
    }
    int finalIdx = (idx % size + size) % size;
    return finalIdx;
}

template <typename K, typename T> 
int Hash<K, T> :: getHashIdx(std::string val) {
    int idx = 0;
    for (size_t i = 0; i < val.length(); i++)
    {
        idx = idx * 31 + static_cast<unsigned char>(val[i]);
    }
    int finalIdx = (idx % size + size) % size;
    return finalIdx;
}

template <typename K, typename T>
void Hash<K, T>::insert(K key, T val)
{
    if (search(key) != nullptr)
        return;
    int idx = getHashIdx(key);

    if (table[idx] == nullptr)
    {
        table[idx] = new Node<K, T>(key, val);
    }
    else
    {
        Node<K, T> *newNode = new Node<K, T>(key, val);
        newNode->next = table[idx];
        table[idx] = newNode;
    }
    insertedCount++;
    if (allowResize && insertedCount > threshold)
    {
        resizeTable();
    }
}

template <typename K, typename T>
void Hash<K, T>::remove(K key)
{
    int idx = getHashIdx(key);

    if (table[idx] == nullptr)
    {
        return;
    }
    if (table[idx]->key == key)
    {
        Node<K, T> *temp = table[idx];
        table[idx] = table[idx]->next;
        delete temp;
    }
    else
    {
        Node<K, T> *prevNode = nullptr;
        Node<K, T> *currNode = table[idx];
        while (currNode != nullptr && currNode->key != key)
        {
            prevNode = currNode;
            currNode = currNode->next;
        }
        if (currNode == nullptr)
        {
            return;
        }
        Node<K, T> *tempNode = currNode;
        if (prevNode != nullptr)
        {
            prevNode->next = currNode->next;
        }
        delete tempNode;
    }
}

template <typename K, typename T>
Node<K, T> *Hash<K, T>::search(K key)
{
    int idx = getHashIdx(key);

    if (table[idx] == nullptr)
    {
        return nullptr;
    }
    else
    {
        Node<K, T> *currNode = table[idx];
        while (currNode != nullptr && currNode->key != key)
        {
            currNode = currNode->next;
        }
        return currNode;
    }
}

template <typename K, typename T>
void Hash<K, T>::display()
{
    for (int i = 0; i < size; i++)
    {
        Node<K, T> *currNode = table[i];
        cout << i << " : ";
        while (currNode != nullptr)
        {
            cout << currNode->data << " -> ";
            currNode = currNode->next;
        }
        cout << endl;
    }
}