#include "../include/CWEB3Http.h"
#include "../include/hashtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void* CWeb3HttpRespond(CWeb3Socket clientSocket, char* body, size_t bodySize, CWeb3HTTPData httpData) {
    size_t bodyLen = bodySize;
    size_t len = bodyLen + 2048;

    char minorStr[12] = {0};
    if (httpData.version.minor != 0)
        sprintf(minorStr, ".%i", httpData.version.minor);

    char* response = malloc(len);
    memset(response, 0, len);
    sprintf(response,
"HTTP/%i%s %s\r\n\
Content-Type:%s\r\n\
Content-Lenght:%zu\r\n\r\n",
    httpData.version.major, minorStr, httpData.code,
    httpData.conentType,
    bodyLen
    );
    size_t headSize = strlen(response);
    memcpy(response+headSize, body, bodySize);
    CWeb3Send(clientSocket, response, headSize+bodySize);
    return response;
}

enum CWeb3HTTPMethod _parseMethod(char* str, size_t* pPos) {
    char strMethod[10] = {0};
    size_t pos = 0;

    while (str[pos + *pPos] != ' ') {
        strMethod[pos] = str[pos + *pPos];
        pos++;
    } 
    *pPos = pos + *pPos;

    if (!strcmp(strMethod, "GET")) return methodGET;
    else if (!strcmp(strMethod, "POST")) return methodPOST;
    else if (!strcmp(strMethod, "DELETE")) return methodDELETE;
    else if (!strcmp(strMethod, "PUT")) return methodPUT;
    else return -1;
}

struct CWeb3Version _parseVersion(char* str, size_t* pPos) {
    struct CWeb3Version version= {0};
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
            *pPos += 1;
            break;
        default:
            return;
            break;
        }
    }  
}

char* _parseCopyUntil(char* str, size_t* pPos, char breakChar) {
    size_t end = *pPos;
    size_t start = *pPos;
    while (str[end] != breakChar)
        end++;
    size_t size = end - start;

    char* slice = malloc(size +1);
    memcpy(slice, str+start, size);
    slice[size] = 0;
    *pPos = end;
    return slice;
}

HashItem _parseIntoItem(char* str, size_t* pPos, char breakChar) {
    HashItem item;
    size_t beforePos = *pPos;
    item.pItem = _parseCopyUntil(str, pPos, breakChar);
    item.size = (*pPos)-beforePos +1;
    return item;
}

int8_t _parseIsHeaderEnd(char* str, size_t pos) {
    char* template = "\r\n\r\n";
    if (!memcmp(str+pos, template, 4)) return 1;
    else return 0;

}

CWeb3HTTPRequest CWeb3ParseRequest(char* str) {
    CWeb3HTTPRequest parsedRequest = {0};
    size_t len = strlen(str);
    if (len < 22) {
        CWeb3HTTPRequest err = {0};
        return err;
    }
    
    parsedRequest.header = newHashtable(512);
    size_t pos = 0;
    parsedRequest.method = _parseMethod(str, &pos);
    pos++;
    parsedRequest.path = _parseCopyUntil(str, &pos, ' ');
    pos += 6; // skip " HTTP/" part
    parsedRequest.version = _parseVersion(str, &pos);
    pos++;
    _parseSkipWhiteSpace(str, &pos);

    int isHeader = 1;
    while (isHeader)
    {
        HashPair pair;
        pair.key = _parseIntoItem(str, &pos, ':');
        pos++;
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
                pos += 2;
            }
        }
        pos++;
        hashtableAdd(parsedRequest.header, pair);
        freeHashPair(pair);
    }

    if (pos < len) {
        size_t bodySize = len - pos +1;
        parsedRequest.body = malloc(bodySize);
        memcpy(parsedRequest.body, str+pos, bodySize);
    }
    else parsedRequest.body = 0;

    return parsedRequest;
}

void freeCWeb3HTTPRequest(CWeb3HTTPRequest request) {
    freeHashtable(request.header);
    free(request.path);
    if (request.body != 0) free(request.body);
}

char* getHTTPMethod(enum CWeb3HTTPMethod method) {

    #define RETURN_IF_METHOD(meth, ret) case meth: return ret;
    switch (method)
    {
        RETURN_IF_METHOD(methodGET, "GET")
        RETURN_IF_METHOD(methodPOST, "POST")
        RETURN_IF_METHOD(methodPUT, "PUT")
        RETURN_IF_METHOD(methodDELETE, "DELETE")

        default:
            printf("[CWeb3]: unknown method");
            exit(0);
    }
    #undef RETURN_IF_METHOD
}
