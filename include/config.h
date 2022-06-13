
#include <stdbool.h>
#pragma once

typedef struct {
    const char* host;
    int port;

    bool verbose;

    int protocol;
} CWeb3Config;
