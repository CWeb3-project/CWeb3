#include "../include/server.h"
#include "../include/CWEB3Http.h"


#include <stdlib.h>
#include <stdio.h>

CWeb3Server* CWeb3_create_server(CWeb3Config config) {
    CWeb3Server* s = malloc(sizeof(CWeb3Server));
    s->config = config;
    return s;
}

void CWeb3_server_merge_routes(CWeb3Server* server, CWeb3Routes* routes) {
    server->routes = routes;
}

void CWeb3_server_start(CWeb3Server* p_server) {
    CWeb3Socket server = newCWeb3Socket(p_server->config);

    if (!server.socket) printf("err on serv sock\n");
    while (1)
    {
        // wait till the client connects
        CWeb3Socket client = CWeb3Listen(server);
        printf("HELLO\n");
        if (!client.socket) printf("err on clin sock\n");

        // read the client message
        size_t messageSize;
        char* messageBuffer = CWeb3Recv(client, &messageSize);
        CWeb3HTTPRequest request = CWeb3ParseRequest(messageBuffer);
        free(messageBuffer);

        char user[] = "User-Agent";
        HashItem key;
        key.pItem = user;
        key.size = sizeof(user);

        // general debug
        printf("path: %s\n", request.path);
        printf("user: %s\n", getHashValue(request.header, key).pItem);
        freeCWeb3HTTPRequest(request);

        // making the response message
        uint64_t len;

        // send response
        CWeb3HTTPData data;
        data.codeNum = 200; // 200 OK
        data.conentType = contentHtml;
        data.version.major = 1; // Http 1.1
        data.version.minor = 1;

        CWeb3HttpRespond(client, (char*)"hello", data);

        // close client socket
        CWeb3CloseSocket(client);
    }
    // close server socket
    CWeb3CloseSocket(server);
}
