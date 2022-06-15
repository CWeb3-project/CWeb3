#include "../include/hashtable.h"
#include <stdio.h>
#include <string.h>

int main() {
    HashItem key;
    char a[1024];
    fgets(a, 1024, stdin);
    key.size = strlen(a)+1;
    key.pItem = a;

    HashItem value;
    char b[1024];
    fgets(b, 1024, stdin);
    value.size = strlen(b)+1;
    value.pItem = b;

    HashPair p;
    p.key = key;
    p.value = value;
    
    printf("%zu\n", genHash(key, 2048));
    Hashtable table = newHashtable(2048);
    addHashValue(table, p);

    HashItem tableValue = getHashValue(table, key);
    printf("< %s >\n", (char*)tableValue.pItem);
}