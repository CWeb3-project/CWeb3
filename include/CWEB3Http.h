#include "../include/soc.h"
#include "../include/hashtable.h"

struct CWeb3Version {
    int major;
    int minor;
};

enum CWeb3ConentType {
    contenttext,
    contentbytes,
    contentJSON,
    contentHtml,
    contentNone
};

typedef struct  {
    struct CWeb3Version version;
    enum CWeb3ConentType conentType;
    int codeNum;
} CWeb3HTTPData;

enum CWeb3HTTPMethod {
    methodGET,
    MethodPOST,
    MethodDELETE,
    MethodPUT
};

typedef struct 
{
    struct CWeb3Version version;
    enum CWeb3HTTPMethod method;
    Hashtable header;
    char* path;
    char* body;
} CWeb3HTTPRequest;


void CWeb3HttpRespond(CWeb3Socket clientSocket, char* body, CWeb3HTTPData httpData);
CWeb3HTTPRequest CWeb3ParseRequest(char* str);
void freeCWeb3HTTPRequest(CWeb3HTTPRequest request);