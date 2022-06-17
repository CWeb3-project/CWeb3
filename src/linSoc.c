#ifdef __linux__

#include "../include/soc.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

const int __MAXDESC = SOMAXCONN;

CWeb3ServerSocket newCWeb3Socket(CWeb3Config config){
    CWeb3ServerSocket server = {0};
    server.socket.config = config;
    server.poll_fd = epoll_create(__MAXDESC);

    int protocol = 0;
    int connectionType = 0;

    switch (config.protocol)
    {
    case TCP:
        protocol = IPPROTO_TCP;
        connectionType = SOCK_STREAM;
        break;
    case UDP:
        protocol = IPPROTO_UDP;
        connectionType = SOCK_DGRAM;
        break;
    default:
        break;
    }

    server.socket.socket_fd = socket(AF_INET, connectionType, protocol);
    if (server.socket.socket_fd == -1) {
        CWeb3ServerSocket s = {0};
        s.socket.socket_fd = -1;
        return s;
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET; // IPV4, use AF_INET6 for IPV6
    serverAddr.sin_port = htons(server.socket.config.port);
    serverAddr.sin_addr.s_addr = inet_addr(server.socket.config.host);

    if (bind(server.socket.socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        shutdown(server.socket.socket_fd, SHUT_WR);
        CWeb3ServerSocket s = {0};
         s.socket.socket_fd = -1;
        return s;
    }

    struct epoll_event event = {0};
    event.events |= EPOLLIN;
    event.events |= EPOLLOUT;
    event.data.fd = server.socket.socket_fd;

    if (epoll_ctl(server.poll_fd, EPOLL_CTL_ADD, server.socket.socket_fd, &event) == -1) {
        shutdown(server.socket.socket_fd, SHUT_WR);
        CWeb3ServerSocket s = {0};
        s.socket.socket_fd = -1;
        return s;
    }
    return server;
}

void CWeb3Listen(CWeb3ServerSocket socket) {
   if (listen(socket.socket.socket_fd, __MAXDESC) == -1) {
        // error handling
    } 
}

CWeb3Socket CWeb3WaitReady(CWeb3ServerSocket socket) {
    
    struct epoll_event event;
    int ret = epoll_wait(socket.poll_fd, &event, __MAXDESC, -1);
    if (ret < 1) {
        CWeb3Socket s = {0};
        s.socket_fd = ret;
        return s;
    }

    if (event.events == EPOLLHUP) {
        printf("shutdown of %i\n", event.data.fd);
        epoll_ctl(socket.poll_fd, EPOLL_CTL_DEL, event.data.fd, 0);
        shutdown(event.data.fd, SHUT_WR);
        return CWeb3WaitReady(socket);
    }

    int clientSockI;
    if (event.data.fd == socket.socket.socket_fd) {
        struct sockaddr_in client = {0};
        int clientsize = sizeof(client);
        clientSockI = accept(socket.socket.socket_fd, (struct sockaddr*)&client, (socklen_t *)&clientsize);
        if (clientSockI == -1)  {
            CWeb3Socket s = {0};
            s.socket_fd = -1;
            return s;
        }

        struct epoll_event event = {0};
        event.events |= EPOLLIN;
        event.events |= EPOLLOUT;
        event.events |= EPOLLHUP;
        event.data.fd = clientSockI;
        
        if (epoll_ctl(socket.poll_fd, EPOLL_CTL_ADD, clientSockI, &event) == -1) {
            shutdown(clientSockI, SHUT_WR);
            CWeb3Socket s = {0};
            s.socket_fd = -1;
            return s;
        }
    }
    else {
        clientSockI = event.data.fd;
    }

    CWeb3Socket clientSock = {0};
    clientSock.socket_fd = clientSockI;
    return clientSock;
}

size_t CWeb3RecvChunk(CWeb3Socket clientSocket, char* buffer, size_t bufferSize) {
    return recv(clientSocket.socket_fd, buffer, bufferSize, 0);
}

void CWeb3Send(CWeb3Socket clientSocket, char* buffer, size_t bufferSize) {
    send(clientSocket.socket_fd, buffer, bufferSize, 0);
}


void closeCWeb3Socket(CWeb3ServerSocket server, CWeb3Socket socket) {
    epoll_ctl(server.poll_fd, EPOLL_CTL_DEL, socket.socket_fd, 0);
    shutdown(socket.socket_fd, SHUT_WR);
    close(socket.socket_fd);
}

void closeCWeb3Server(CWeb3ServerSocket server) {
    shutdown(server.poll_fd, SHUT_RDWR);
    shutdown(server.socket.socket_fd, SHUT_WR);
    close(server.socket.socket_fd);
}

#endif /* __linux__*/