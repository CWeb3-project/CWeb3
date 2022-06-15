#include "../include/http.h"
#include "../include/hashtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum ConentType {
    contentText,
    contentBytes,
    contentJSON,
    contentHtml
};

struct Version {
    int major;
    int minor;
};

struct HTTPData
{
    struct Version version;
    enum ConentType conentType;
    int codeNum;
};

char* table[511];


void CWeb3HttpRespond(CWeb3Socket clientSocket ,char* body, struct HTTPData httpData) {
    size_t bodyLen = strlen(body);
    size_t len = bodyLen + 2048;

    char minorStr[12] = {0};
    sprintf(minorStr, ".%i", httpData.version.minor);

    char codeInfoStr[] = "OK"; // todo: add all the other codes, int -> char* lookup
    char contentStr[] = "text/html"; // todo: enum -> char* lookup

    char* response = malloc(len);
    memset(response, 0, len);
    sprintf(response,
    "HTTP/%i%s %i %s\r\n\
    Content-Type:%s\n\
    Content-Lenght:%zu\r\n\r\n\
    %s",
    httpData.version.major, minorStr, httpData.codeNum, codeInfoStr,
    contentStr,
    bodyLen,
    body
    );

    CWeb3Send(clientSocket, response);

}