#ifndef HASH_H
#define HASH_H

#include "Node.h"
using namespace std;

template <typename K, typename T>
class Hash {
    private :
        int size;
        int threshold;
        int insertedCount;
        bool allowResize;
        Node<K, T>** table;
        int getHashIdx(int val);
        int getHashIdx(float val);
        int getHashIdx(std::string val);
        int getHashIdx(char* val);
        void resizeTable();
        void deallocateTable();
    public : 
        Hash();
        Hash(int size);
        Hash(const Hash<K, T>& obj);
        ~Hash();
        void insert(K key, T val);
        int insert(K key);
        void remove(K key);
        Node<K, T>* search(K key);
        void display();
};

#include "Hash.tpp"

#endif