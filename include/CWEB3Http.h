#include "../include/soc.h"
#include "../include/hashtable.h"

struct CWEB3Version {
    int major;
    int minor;
};

enum CWEB3ConentType {
    contenttext,
    contentbytes,
    contentJSON,
    contentHtml
};

typedef struct  {
    struct CWEB3Version version;
    enum CWEB3ConentType conentType;
    int codeNum;
} CWEB3HTTPData;

enum CWEB3HTTPMethod {
    methodGET,
    MethodPOST,
    MethodDELETE,
    MethodPUT
};

typedef struct 
{
    struct CWEB3Version version;
    enum CWEB3HTTPMethod method;
    Hashtable header;
    char* path;
    char* body;
} CWEB3HTTPRequest;


void CWeb3HttpRespond(CWeb3Socket clientSocket, char* body, CWEB3HTTPData httpData);
CWEB3HTTPRequest CWEB3ParseRequest(char* str);