#pragma once
#include "config.h"
#include <inttypes.h>
#include <stddef.h>

typedef struct {
    CWeb3Config config;
    uint64_t socket_fd;
} CWeb3Socket;

typedef struct {
    CWeb3Socket socket;
    uint64_t poll_fd;
} CWeb3ServerSocket;



CWeb3ServerSocket newCWeb3Socket(CWeb3Config config);
void CWeb3Listen(CWeb3ServerSocket socket);
CWeb3Socket CWeb3WaitReady(CWeb3ServerSocket socket);
size_t CWeb3RecvChunk(CWeb3Socket clientSocket, char* buffer, size_t bufferSize);
void CWeb3Send(CWeb3Socket clientSocket, char* buffer, size_t bufferSize);
void closeCWeb3Socket(CWeb3ServerSocket server, CWeb3Socket socket);
void closeCWeb3Server(CWeb3ServerSocket server);
char* CWeb3Recv(CWeb3Socket clientSocket, size_t* pBufferSize);