#include "../include/server.h"
#include "../include/CWEB3Http.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

CWeb3Socket __CWeb3_current_server;

CWeb3Server CWeb3_create_server(CWeb3Config config) {
    CWeb3Server s;
    s.config = config;
    return s;
}

void CWeb3_server_merge_routes(CWeb3Server* pServer, CWeb3Routes routes) {
    pServer->routes = routes;
}

void handle(int code) {
    printf("\nRecieved SIGINT, quiting...\n");
    CWeb3CloseSocket(__CWeb3_current_server);
    exit(1);
}

void CWeb3_server_start(CWeb3Server p_server) {
    CWeb3Socket server = newCWeb3Socket(p_server.config);
    __CWeb3_current_server = server;

    signal(SIGINT, handle);

    if (!server.socket) printf("err on serv sock\n");

    if (p_server.config.verbose) printf("[CWeb3]: server listening at http://%s:%d\n", p_server.config.host, p_server.config.port);
    while (1)
    {
        // wait till the client connects
        CWeb3Socket client = CWeb3Listen(server);
        if (!client.socket) printf("err on clin sock\n");

        // read the client message
        size_t messageSize;
        char* messageBuffer = CWeb3Recv(client, &messageSize);
        CWeb3HTTPRequest req = CWeb3ParseRequest(messageBuffer);
        free(messageBuffer);

        if (p_server.config.verbose) {
            printf("[CWeb3]: Request (%s): \"%s\"\n", req.path, getHTTPMethod(req.method));
        }

        // Calling the function
        CWeb3Request request;

        request.version = req.version;
        request.method  = req.method;
        request.headers = req.header;
        request.path    = req.path;
        request.body    = req.body;

        CWeb3Response response;

        response.data = "";
        response.code = 200;
        response.content_type = "text/html";

        for (int i = 0; i < p_server.routes.used; i++) {
            // TODO: better logging
            if (strcmp(p_server.routes.array[i].route, req.path) == 0) {
                p_server.routes.array[i].callback(&request, &response);
            }
        }


        // send response
        CWeb3HTTPData data;
        data.code = "200 OK"; // 200 OK
        data.version.major = 1; // Http 1.1
        data.version.minor = 1;
        data.conentType = response.content_type;

        void* res = CWeb3HttpRespond(client, response.data, strlen(response.data), data);

        // free
        freeCWeb3HTTPRequest(req);

        // close client socket
        CWeb3CloseSocket(client);
        free(res);
    }

    // close server socket
    CWeb3CloseSocket(server);
}
