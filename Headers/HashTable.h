#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <vector>
#include "BST.h"
#include "Log.h"
using namespace std;

class HashTable
{
private:
    vector<BSTNode *> *table;
    int size;

    int hashFunction(int key)
    {
        return key % size;
    }

public:
    HashTable(int tableSize)
    {
        size = tableSize;
        table = new vector<BSTNode *>[size];
    }

    HashTable()
    {
        size = 10; // -> it was 100
        table = new vector<BSTNode *>[size];
    }

    void insert(BSTNode *node)
    {
        int index = hashFunction(node->courseId);
        table[index].push_back(node);
        PrintLog("Inserted course with ID: " + to_string(node->courseId) + " into hash table\n");
    }

    BSTNode *search(int courseId)
    {
        int index = hashFunction(courseId);
        for (BSTNode *node : table[index])
            if (node->courseId == courseId)
            {
                PrintLog("Found course with ID: " + to_string(courseId) + " in hash table\n");
                return node;
            }
        PrintLog("Course with ID: " + to_string(courseId) + " not found in hash table\n");
        return NULL;
    }
};
#endif