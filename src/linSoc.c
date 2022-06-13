#include "../include/soc.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

CWeb3Socket newCWeb3Socket(CWeb3Config config){
    CWeb3Socket sock = {0};
    sock.config = config;

    sock.socket = socket(AF_INET, SOCK_STREAM, config.protocol);
    if (sock.socket == -1) {
        CWeb3Socket s = {0};
        return s;
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET; // IPV4, use AF_INET6 for IPV6
    serverAddr.sin_port = htons(sock.config.port);
    serverAddr.sin_addr.s_addr = inet_addr(sock.config.host);
    
    if (bind(sock.socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        CWeb3Socket s = {0};
        return s;
    }

    return sock;
}

int CWeb3Listen(CWeb3Socket sock, char* buffer, size_t bufferSize) {
    if (listen(sock.socket, 1) == -1) {
        // error handling
        return -1;
    } else return 0;

    struct sockaddr_in client = {0};
    int clientsize = sizeof(client);
    int clientSock = accept(sock.socket, (struct sockaddr*)&client, (socklen_t *)&clientsize);
}

void CWeb3RecvChunk(CWeb3Socket sock, char* buffer, size_t bufferSize) {

}
