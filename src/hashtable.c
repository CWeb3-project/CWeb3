#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct {
    void* pItem;
    size_t size;
} HashItem;

typedef struct 
{
    HashItem key;
    HashItem value;
} HashPair;

typedef struct {
    HashPair pair;
    HashLinkedList* pNext;
} HashLinkedList;

typedef struct {
    size_t bucketLen;
    HashLinkedList* bucket;
} Hastable;

Hastable newHashtable(size_t bucketLen){
    Hastable table;
    table.bucket = malloc(sizeof(HashLinkedList) * bucketLen);
    table.bucketLen = bucketLen;
}

size_t hashtableHash(HashItem key, size_t buckeLen) {
    uint64_t hash = 0;
    int8_t* charKey = (int8_t *)key.pItem;

    for (size_t i = 0; i < key.size; i++ ) {
        hash *= charKey[i]*charKey[i];
    }
    return hash % buckeLen;
}

HashItem copyHashItem(HashItem original) {
    HashItem item;
    item.pItem = malloc(original.size);
    memcpy(item.pItem, original.pItem, original.size);
    item.size = original.size;
    return HashItem;
}

HashPair copyHashPair(HashPair original) {
    HashPair pair;
    pair.key = copyHashItem(original.key);
    pair.value = copyHashItem(original.value);
}

HashLinkedList* getLastNode(HashLinkedList* pNode) {
    if (pNode->pNext == 0) return pNode;
    else return getLastNode(pNode);
}

void addHashValue(Hastable table, HashPair pair) {
    HashPair newPair = copyHashPair(pair);
    size_t hash = hashtableHash(newPair.key, table.bucketLen);
    HashLinkedList linkedNode;
    linkedNode.pNext = 0;
    linkedNode.pair = newPair;

    if (table.bucket[hash].pNext == 0) {
        if (table.bucket[hash].pair.key.pItem == 0) {
            table.bucket[hash] = linkedNode;
        } else {
            HashLinkedList* pNode = malloc(sizeof(HashLinkedList));
            memcpy(pNode, &linkedNode, sizeof(linkedNode));
            getLastNode(table.bucket[hash])->pNext = pNode;
        }
    } 
}

HashItem findInList(HashLinkedList* pNode, HashItem key) {
    int t = 0;
    HashItem fail = {0};
    if  (pNode->pair.size == key.size) {
        if (!memcmp(pNode->pair.pItem, key.pItem, key.size)) return pNode->pair.value;
    } 
    if (pNode->pNext) {
        return findInList(pNode->pNext, key);
    } else return fail;
}

HashItem getHashValue(Hastable table, HashItem key) {
    size_t hash = hashtableHash(key, table.bucketLen);
    HashItem tableKey = table.bucket[hash].pair.key;

    return findInList(table.bucket+hash, key)
}