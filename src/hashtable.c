#include "../include/hashtable.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

Hashtable newHashtable(size_t bucketLen){
    Hashtable table;
    table.bucket = calloc(bucketLen ,sizeof(HashLinkedNode));
    table.bucketLen = bucketLen;
    return table;
}

size_t genHash(HashItem key, size_t buckeLen) {
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
    return item;
}

HashPair copyHashPair(HashPair original) {
    HashPair pair;
    pair.key = copyHashItem(original.key);
    pair.value = copyHashItem(original.value);
    return pair;
}

HashLinkedNode* getLastNode(HashLinkedNode* pNode) {
    if (pNode->pNext == 0) return pNode;
    else return getLastNode(pNode);
}

void addHashValue(Hashtable table, HashPair pair) {
    HashPair newPair = copyHashPair(pair);
    size_t hash = genHash(newPair.key, table.bucketLen);
    HashLinkedNode linkedNode;
    linkedNode.pNext = 0;
    linkedNode.pair = newPair;

    if (table.bucket[hash].pNext == 0) {
        if (table.bucket[hash].pair.key.pItem == 0) {
            table.bucket[hash] = linkedNode;
        } else {
            HashLinkedNode* pNode = malloc(sizeof(HashLinkedNode));
            memcpy(pNode, &linkedNode, sizeof(linkedNode));
            getLastNode(table.bucket+hash)->pNext = pNode;
        }
    } 
}

HashItem findInList(HashLinkedNode* pNode, HashItem key) {
    int t = 0;
    HashItem fail = {0};
    if  (pNode->pair.key.size == key.size) {
        if (!memcmp(pNode->pair.key.pItem, key.pItem, key.size)) return pNode->pair.value;
    } 
    if (pNode->pNext) {
        return findInList(pNode->pNext, key);
    } else return fail;
}

HashItem getHashValue(Hashtable table, HashItem key) {
    size_t hash = genHash(key, table.bucketLen);
    HashItem tableKey = table.bucket[hash].pair.key;

    return findInList(table.bucket+hash, key);
}

void freeHashItem(HashItem item) {
    free(item.pItem);
}

void freeHashPair(HashPair pair) {
    freeHashItem(pair.key);
    freeHashItem(pair.value);
}

void freeLinkedListNode(HashLinkedNode* pNode) {
    if (pNode->pNext) freeLinkedListNode(pNode->pNext);
    freeHashPair(pNode->pair);
    free(pNode);
}

void freeHashtable(Hashtable table) {
    for (size_t i = 0; i < table.bucketLen; i++) 
        freeLinkedListNode(table.bucket+i);
    free(table.bucket);
}