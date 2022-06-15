#include "../include/http.h"
#include "../include/hashtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum HTTPConentType {
    contenttext,
    contentbytes,
    contentJSON,
    contentHtml
};

enum HTTPMethod {
    methodGET,
    MethodPOST,
    MethodDELETE,
    MethodPUT
};

struct Version {
    int major;
    int minor;
};

struct HTTPData
{
    struct Version version;
    enum HTTPConentType conentType;
    int codeNum;
};

struct HTTPRequest
{
    struct Version version;
    enum HTTPMethod method;
    Hashtable header;
    char* path;
    char* body;
};


char* table[511];


void CWeb3HttpRespond(CWeb3Socket clientSocket, char* body, struct HTTPData httpData) {
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

enum HTTPMethod _parseMethod(char* str, size_t* pPos) {
    char strMethod[10] = {0};

    while (str[*pPos] != ' ') {
        strMethod[*pPos] = str[*pPos];
        (*pPos)++;
    }

    if (!strcmp(strMethod, "GET")) return methodGET;
    else if (!strcmp(strMethod, "POST")) return MethodPOST;
    else if (!strcmp(strMethod, "DELETE")) return MethodDELETE;
    else if (!strcmp(strMethod, "PUT")) return MethodPUT;
    else return -1;
}

struct Version _parseVersion(char* str, size_t* pPos) {
    struct Version version= {0};
    version.major = str[*pPos] - '0';
    if (str[*pPos+1] == '.') {
        version.minor = str[*pPos + 2] - '0';
        *pPos += 1;
    }
    *pPos += 1;

    return version;
}
void _parseSkipWhiteSpace(char* str, size_t* pPos) {
    while (1)
    {
        switch (str[*pPos])
        {
        case ' ':
        case '\n':
        case '\r':
        case '\t':
            pPos += 1;
            break;
        default:
            return;
            break;
        }
    }  
}

char* _parseCopyUntil(char* str, size_t* pPos, char breakChar) {
    size_t end = *pPos;
    while (str[end] != breakChar)
        end++;
    size_t size = *pPos - end;
    char* slice = malloc(size +1);
    memcpy(slice, str+ *pPos, size);
    slice[size] = 0;
    *pPos = end;

    return slice;
}

HashItem _parseIntoItem(char* str, size_t* pPos, char breakChar) {
    HashItem item;
    size_t beforePos = *pPos;
    item.pItem = _parseCopyUntil(str, pPos, breakChar);
    item.size = *pPos-beforePos +1;
    return item;
}

int8_t _parseIsHeaderEnd(char* str, size_t pos) {
    char* template = "\r\n\r\n";
    if (!memcmp(str+pos, template, 4)) return 1;
    else return 0;

}

struct HTTPRequest parseHTTPRequest(char* str) {
    struct HTTPRequest parsedRequest = {0};
    parsedRequest.header = newHashtable(512);
    size_t len = strlen(str);
    size_t pos = 0;
    parsedRequest.method = _parseMethod(str, &pos);
    pos++;
    parsedRequest.path = _parseCopyUntil(str, &pos, ' ');
    pos += 6; // skip " HTTP/" part
    parsedRequest.version = _parseVersion(str, &pos);
    _parseSkipWhiteSpace(str, &pos);
    
    int isHeader = 1;
    while (isHeader)
    {
        HashPair pair;
        pair.key = _parseIntoItem(str, &pos, ':');
        _parseSkipWhiteSpace(str, &pos);
        pair.value = _parseIntoItem(str, &pos, '\n');
        size_t check = pair.value.size-2;

        // check if it's the end of the header 
        char* valueStr =  (char *)pair.value.pItem;
        if (valueStr[check] == '\r') {
            valueStr[check] = 0;
            pair.value.size = check+1; 
            // not a byte leak, because the memory is going to be freed soon
            if (_parseIsHeaderEnd(str, pos-1)) {
                isHeader = 0;
                pos += 3;
            }
        }

        addHashValue(parsedRequest.header, pair);
        freeHashPair(pair);
    }
    
    size_t bodySize = len - pos +1;
    parsedRequest.body = malloc(bodySize);
    memcpy(parsedRequest.body, str+pos, bodySize);

    return parsedRequest;
}