
#include <stdbool.h>
#pragma once

typedef struct {
    const char* host;
    int port;

    bool verbose; // bool is just a macro for _Bool, _Bool is a typedef of char

    enum {TCP, UDP} protocol;
} CWeb3Config;
