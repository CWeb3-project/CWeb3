#pragma once
#include "config.h"
#include <inttypes.h>
#include <stddef.h>

typedef struct {
    CWeb3Config config;
    int socket;
} CWeb3Socket;

CWeb3Socket newCWeb3Socket(CWeb3Config config);
int CWeb3Listen(CWeb3Socket sock);
void CWeb3RecvChunk(int clientSocket, char* buffer, size_t bufferSize);
void CWeb3Send(int clientSocket, char* buffer);