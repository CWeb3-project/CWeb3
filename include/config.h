
#include <stdbool.h>
#pragma once

typedef struct {
    const char* host;
    int port;

    bool verbose;

    enum {TCP, UDP} protocol;
} CWeb3Config;
