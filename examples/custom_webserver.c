#include "../include/soc.h"
#include "../include/CWEB3Http.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main() {
    CWeb3Config config;
    config.host = "127.0.0.1";
    config.port = 8005;
    config.protocol = TCP;
    config.verbose = 1;
    CWeb3Socket server = newCWeb3Socket(config);
    if (!server.socket_fd) printf("err on serv sock");

    CWeb3Listen(server);
    while (1)
    {
        // wait untill the client connects
        CWeb3Socket client = CWeb3Accept(server);
        if (!client.socket_fd) printf("err on clin sock");

        // read the client message
        size_t messageSize;
        char* messageBuffer = CWeb3Recv(client, &messageSize);
        CWeb3HTTPRequest request = CWeb3ParseRequest(messageBuffer);
        free(messageBuffer);

        // general debug
        printf("path: %s\n", request.path);
        freeCWeb3HTTPRequest(request);

        // making the response message 
        uint64_t len;
        char* File = readFile("index.html", &len);
            
        // send response
        CWeb3HTTPData data;
        data.codeNum = 200; // 200 OK
        data.conentType = contentHtml;
        data.version.major = 1; // Http 1.1
        data.version.minor = 1;

        void* response = CWeb3HttpRespond(client, File, len, data);// it can only be cleaned after socket it closed
        free(File);
            
        // close client socket_fd
        CWeb3CloseSocket(client);
        free(response);

    }

    // close server socket_fd
    CWeb3CloseSocket(server);
}