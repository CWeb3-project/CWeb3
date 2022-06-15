#include "../include/soc.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

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
    config.port = 10003;
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
        printf("message size: %zu\n\n", messageSize);
        printf("%s\n", messageBuffer);

        // making the response message 
        uint64_t len;
        char* File = readFile("index.html", &len);
         
        char responseBuffer[30000] =  {0}; 
        sprintf(responseBuffer ,
<<<<<<< HEAD
            "HTTP/1.1 200 OK\r\n\
            Content-Type:text/html\n\
            Content-Lenght:%i\r\n\r\n\
            %s",
            len,
            File
=======
        "HTTP/1.1 200 OK\r\n\
        Content-Type:text/html\n\
        Content-Lenght:%"PRIu64"\r\n\r\n\
        %s",
        len,
        File
>>>>>>> f33dfe2193b27107b7974e9426d17c7f26abf724
        );

        free(File);
        
        // send response message
        CWeb3Send(client, responseBuffer);
        
        // close client socket
        shutdown(client.socket, SHUT_WR);
    }
    // close server socket
    shutdown(server.socket, SHUT_WR);
}