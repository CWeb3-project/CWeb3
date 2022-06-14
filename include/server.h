
#include "routes.h"
#pragma once

typedef struct {

} CWeb3Server;

CWeb3Server* CWeb3_create_server();
void CWeb3_server_add_routes(CWeb3Server*, CWeb3Routes* routes);
CWeb3Server* CWeb3_server_start(CWeb3Server*);
