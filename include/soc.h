#pragma once
#include "config.h"
#include <inttypes.h>
#include <stddef.h>

typedef struct {
    CWeb3Config config;
    uint64_t socket;
} CWeb3Socket;

CWeb3Socket newCWeb3Socket(CWeb3Config config);
CWeb3Socket CWeb3Listen(CWeb3Socket socket);
size_t CWeb3RecvChunk(CWeb3Socket clientSocket, char* buffer, size_t bufferSize);
void CWeb3Send(CWeb3Socket clientSocket, char* buffer, size_t bufferSize);
char* CWeb3Recv(CWeb3Socket clientSocket, size_t* pBufferSize);
void CWeb3CloseSocket(CWeb3Socket socket);