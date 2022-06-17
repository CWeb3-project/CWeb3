#ifdef __linux__

#include "../include/soc.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

const int __MAXDESC = SOMAXCONN;

CWeb3Socket newCWeb3Socket(CWeb3Config config){
    CWeb3Socket sock = {0};
    sock.config = config;
    sock.poll_fd = epoll_create(__MAXDESC);

    
    int protocol = (config.protocol == TCP) ? IPPROTO_TCP : IPPROTO_UDP; 
    int connectionType = (config.protocol == TCP) ? SOCK_STREAM : SOCK_DGRAM;

    sock.socket_fd = socket(AF_INET, connectionType, protocol);
    if (sock.socket_fd == -1) {
        CWeb3Socket s = {0};
        return s;
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET; // IPV4, use AF_INET6 for IPV6
    serverAddr.sin_port = htons(sock.config.port);
    serverAddr.sin_addr.s_addr = inet_addr(sock.config.host);
    
    if (bind(sock.socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        CWeb3Socket s = {0};
        return s;
    }

    struct epoll_event event = {0};

    event.events |= EPOLLIN;
    event.events |= EPOLLOUT;
    event.data.u64 = 0;
    
    if (epoll_ctl(sock.poll_fd, EPOLL_CTL_ADD, sock.socket_fd, &event) == -1) {
        CWeb3Socket s = {0};
        return s;
    }

    return sock;
}

void CWeb3Listen(CWeb3Socket socket) {
   if (listen(socket.socket_fd, __MAXDESC) == -1) {
        // error handling
    }
}

CWeb3Socket CWeb3Accept(CWeb3Socket socket) {
    struct epoll_event events;

    int s = epoll_wait(socket.poll_fd, &events, 1, -1);
    printf("%i\n",s);

    struct sockaddr_in client = {0};
    int clientsize = sizeof(client);
    int clientSockI = accept(socket.socket_fd, (struct sockaddr*)&client, (socklen_t *)&clientsize);
    if (clientSockI == -1)  {
        // error handling
    }

    CWeb3Socket clientSock = {0};
    clientSock.config.port = client.sin_port;
    clientSock.socket_fd = clientSockI;

    return clientSock;
}

size_t CWeb3RecvChunk(CWeb3Socket clientSocket, char* buffer, size_t bufferSize) {
    return recv(clientSocket.socket_fd, buffer, bufferSize, 0);
}

void CWeb3Send(CWeb3Socket clientSocket, char* buffer, size_t bufferSize) {
    send(clientSocket.socket_fd, buffer, bufferSize, 0);
}

void CWeb3CloseSocket(CWeb3Socket socket) {
    shutdown(socket.socket_fd, SHUT_WR);
}

#endif /* __linux__*/