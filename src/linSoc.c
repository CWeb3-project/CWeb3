#ifdef __linux__

#include "../include/soc.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stddef.h>

CWeb3Socket newCWeb3Socket(CWeb3Config config){
    CWeb3Socket sock = {0};
    sock.config = config;

    int protocol = (config.protocol == TCP) ? IPPROTO_TCP : IPPROTO_UDP;
    int connectionType = (config.protocol == TCP) ? SOCK_STREAM : SOCK_DGRAM;

    sock.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

CWeb3Socket CWeb3Listen(CWeb3Socket socket) {
    if (listen(socket.socket, SOMAXCONN) == -1) {
        printf("ERROR\n");
        // error handling
    }

    struct sockaddr_in client = {0};
    int clientsize = sizeof(client);
    int clientSockI = accept(socket.socket, (struct sockaddr*)&client, (socklen_t *)&clientsize);
    if (clientSockI == -1)  {
        printf("ERROR\n");
        // error handling
    }

    CWeb3Socket clientSock = {0};
    clientSock.config.port = client.sin_port;
    clientSock.socket = clientSockI;

    return clientSock;
}

size_t CWeb3RecvChunk(CWeb3Socket clientSocket, char* buffer, size_t bufferSize) {
    return recv(clientSocket.socket, buffer, bufferSize, 0);
}

void CWeb3Send(CWeb3Socket clientSocket, char* buffer, size_t bufferSize) {
    send(clientSocket.socket, buffer, bufferSize, 0);
}

void CWeb3CloseSocket(CWeb3Socket socket) {
    shutdown(socket.socket, SHUT_WR);
}

#endif /* __linux__*/