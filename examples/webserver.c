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
        char buf[3000] = {0};
        size_t messageSize = CWeb3RecvChunk(client, buf, 3000);
        printf("message size: %zu\n\n", messageSize);
        printf("%s\n", buf);

        // making the response message 
        uint64_t len;
        char* File = readFile("index.html", &len);
         
        char responseBuffer[30000] = "HTTP/1.1 200 OK\r\nContent-Type:text/html\nContent-Lenght:";
        char lenStr[20] = {0};
        sprintf(lenStr,"%"PRIu64, len);
        strcat(responseBuffer, lenStr);
        strcat(responseBuffer, "\r\n\r\n");
        strcat(responseBuffer, File);
        free(File);
        
        // send response message
        CWeb3Send(client, responseBuffer);
        
        // close client socket
        shutdown(client.socket, SHUT_WR);
    }
    // close server socket
    shutdown(server.socket, SHUT_WR);
}