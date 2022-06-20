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


/* 
 * i have no idea where i stole this algorithm from
 * update: Fowler–Noll–Vo hash function
 * 1a version
 * apparently this was the default hash function in python
 * it was changed bc some ddos attacks
 * which won't matter when it comes to implementing it here 
 */
size_t fnv_hash_1a_64 (int8_t *key, int len ) { 
    uint8_t* p = (uint8_t *)key;
    uint64_t h = 0xcbf29ce484222325ULL;
    for (int32_t i = 0; i < len; i++ )
      h = ( h ^ p[i] ) * 0x100000001b3ULL;
      
   return h;
}

size_t genHash(HashItem key, size_t buckeLen) {
    return fnv_hash_1a_64(key.pItem, key.size) % buckeLen;
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

void hashtableAdd(Hashtable table, HashPair pair) {
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
    HashItem fail = {0};
    if  (pNode->pair.key.size == key.size) {
        if (!memcmp(pNode->pair.key.pItem, key.pItem, key.size)) return pNode->pair.value;
    } 
    if (pNode->pNext) {
        return findInList(pNode->pNext, key);
    } else return fail;
}

HashItem hashtableGet(Hashtable table, HashItem key) {
    size_t hash = genHash(key, table.bucketLen);

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
    if (pNode->pNext) free(pNode->pNext);
}

void freeHashtable(Hashtable table) {
    for (size_t i = 0; i < table.bucketLen; i++) 
        freeLinkedListNode(table.bucket+i);
    free(table.bucket);
}