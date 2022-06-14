#pragma once
#include "config.h"
#include <inttypes.h>
#include <stddef.h>


typedef struct {
    CWeb3Config config;
    int socket;
} CWeb3Socket;

CWeb3Socket newCWeb3Socket(CWeb3Config config);
CWeb3Socket CWeb3Listen(CWeb3Socket sock);
size_t CWeb3RecvChunk(CWeb3Socket clientSocket, char* buffer, size_t bufferSize);
void CWeb3Send(CWeb3Socket clientSocket, char* buffer);