
#include "hashtable.h"
#include "CWEB3Http.h"

#pragma once
typedef struct
{
    struct CWeb3Version version;
    enum CWeb3HTTPMethod method;
    Hashtable headers;
    char* path;
    char* body;
} CWeb3Request;