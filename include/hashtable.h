#pragma once
#include <stddef.h>

typedef struct {
    void* pItem;
    size_t size;
} HashItem;

typedef struct 
{
    HashItem key;
    HashItem value;
} HashPair;

typedef struct HashLinkedNode {
    HashPair pair;
    struct HashLinkedNode* pNext;
} HashLinkedNode;

typedef struct {
    size_t bucketLen;
    HashLinkedNode* bucket;
} Hashtable;

Hashtable newHashtable(size_t bucketLen);
size_t genHash(HashItem key, size_t buckeLen);
void addHashValue(Hashtable table, HashPair pair);
HashItem getHashValue(Hashtable table, HashItem key);
void freeHashtable(Hashtable table) ;