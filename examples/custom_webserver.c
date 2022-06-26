#include "../include/soc.h"
#include "../include/CWEB3Http.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define flag printf("%s:%i\n", __FILE__, __LINE__);
#define flag(x) printf("ID : %i, FILE : %s:%i\n", x, __FILE__, __LINE__);

char* readFile(const char* path, uint64_t* pLength) {
	FILE* file = fopen(path, "rb");
	fseek(file, 0, SEEK_END);
	 *pLength = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = (char *)malloc((*pLength) + 1);
	fread(buffer, 1 , *pLength, file);
	fclose(file);
	buffer[*pLength] = 0;

	return buffer;
}

CWeb3Socket server;

int main(int argc, char** argv) {
    CWeb3Config config;
    config.host = "127.0.0.1";

    config.port = atoi(argv[1]);
    config.protocol = TCP;
    config.verbose = 1;


    CWeb3Socket server = newCWeb3Socket(config);
    if (!server.socket) printf("err on serv sock");
    while (1)
    {
        // wait till the client connects
        CWeb3Socket client = CWeb3Listen(server);
        if (!client.socket) printf("err on clin sock");

        // read the client message
        size_t messageSize;   
        char* messageBuffer = CWeb3Recv(client, &messageSize);
        CWeb3HTTPRequest request = CWeb3ParseRequest(messageBuffer);

        char user[] = "User-Agent";
        HashItem key;
        key.pItem = user;
        key.size = sizeof(user);

        // general debug
        printf("path: %s\n", request.path);
        printf("user: %s\n", hashtableGet(request.header, key).pItem);
        freeCWeb3HTTPRequest(request);

        // making the response message
        uint64_t len;
        char* File = readFile("index.html", &len);

        // send response
        CWeb3HTTPData data;
        data.code = "200 OK"; // 200 OK
        data.conentType = "text/html";
        data.version.major = 1; // Http 1.1
        data.version.minor = 1;

        void* response = CWeb3HttpRespond(client, File, len, data);// it can only be cleaned after socket it closed
        free(File);

        // close client socket
        CWeb3CloseSocket(client);
        free(response);
    }
    // close server socket
    CWeb3CloseSocket(server);
}