#include "config.h"
#include "routes.h"
#pragma once

typedef struct {
    CWeb3Routes* routes;
    CWeb3Config config;
} CWeb3Server;

CWeb3Server* CWeb3_create_server();
void CWeb3_server_merge_routes(CWeb3Server* server, CWeb3Routes* routes);
void CWeb3_server_start(CWeb3Server* server);
