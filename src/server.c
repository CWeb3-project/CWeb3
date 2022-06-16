#include "../include/server.h"
#include "../include/CWEB3Http.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

CWeb3Server CWeb3_create_server(CWeb3Config config) {
    CWeb3Server s;
    s.config = config;
    return s;
}

void CWeb3_server_merge_routes(CWeb3Server server, CWeb3Routes* routes) {
    server.routes = realloc(routes, sizeof(CWeb3Routes));
}

void CWeb3_server_start(CWeb3Server p_server) {
    CWeb3Socket server = newCWeb3Socket(p_server.config);

    if (!server.socket) printf("err on serv sock\n");
    while (1)
    {
        // wait till the client connects
        CWeb3Socket client = CWeb3Listen(server);
        if (!client.socket) printf("err on clin sock\n");

        // read the client message
        size_t messageSize;
        char* messageBuffer = CWeb3Recv(client, &messageSize);
        CWeb3HTTPRequest request = CWeb3ParseRequest(messageBuffer);
        free(messageBuffer);

        // general debug
        freeCWeb3HTTPRequest(request);

        // Calling the function
        for (int i = 0; i < p_server.routes->used; i++) {
            // TODO: better logging
            // printf("request: %s", )

            if (strcmp(p_server.routes->array[i].route, request.path) == 0) {
                printf("FOUND!\n");
            }
        }
        
        // send response
        CWeb3HTTPData data;
        data.codeNum = 200; // 200 OK
        data.conentType = contentHtml;
        data.version.major = 1; // Http 1.1
        data.version.minor = 1;

        void* response = CWeb3HttpRespond(client, "hello", 5, data);

        // close client socket
        CWeb3CloseSocket(client);
        free(response);
    }

    // close server socket
    CWeb3CloseSocket(server);
}
