#pragma once
#include "config.h"


typedef struct {
    CWeb3Config config;
    int socket;
} CWeb3Socket;

CWeb3Socket newCWeb3Socket(CWeb3Config config, int protocol)