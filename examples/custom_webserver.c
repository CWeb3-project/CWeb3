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
    server = newCWeb3Socket(config);
    if (!server.socket_fd){
        CWeb3CloseSocket(server);
        printf("err on server socket\n");
        exit(1);
    } 
    
    CWeb3Listen(server);
    while (1)
    {  
        // wait untill the client connects
        CWeb3Socket client = CWeb3Accept(server, -1); 

        // read the client message
        size_t messageSize;   
        char* messageBuffer = CWeb3Recv(client, &messageSize);

        /*
         * if the message is empty it will skip this part
         * THIS CHECK IS HERE BECAUSE OF EDGE
         * microsoft at it again
         * like why does edge create a connection
         * and then just send 0 bytes through the connection???
         */
        if (messageBuffer) { 
            printf("%s\n", messageBuffer);
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
                    
            // close client socket
            CWeb3CloseSocket(client);
            free(response);
        }
        CWeb3CloseSocket(client);
    }

    // close server socket
    CWeb3CloseSocket(server);
}