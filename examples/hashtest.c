#include "../include/hashtable.h"
#include <stdio.h>
#include <string.h>

int main() {
    Hashtable table = newHashtable(2048);

    for (int i = 0; i < 5; i++) {
        HashPair p;
        char a[1024];
        printf("key %i:",i);
        fgets(a, 1024, stdin);
        p.key.size = strlen(a)+1;
        p.key.pItem = a;


        printf("value %i:",i);
        char b[1024];
        fgets(b, 1024, stdin);
        p.value.size = strlen(b)+1;
        p.value.pItem = b;
        addHashValue(table, p);

    }

    for (int i = 0; i < 2; i++) {
        HashItem key;
        char a[1024];
        printf("get value %i:",i);
        fgets(a, 1024, stdin);
        key.size = strlen(a)+1;
        key.pItem = a;

        HashItem tableValue = getHashValue(table, key);
        printf("< %s >\n", (char *)tableValue.pItem);
    }
    
}