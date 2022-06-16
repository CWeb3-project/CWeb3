#include "../include/server.h"
#include "../include/config.h"

#include <stdlib.h>
#include <stdio.h>

CWeb3Server* CWeb3_create_server(CWeb3Config config) {
    CWeb3Server* s = malloc(sizeof(CWeb3Server));
    s->config = config;
    return s;
}

void CWeb3_server_merge_routes(CWeb3Server* server, CWeb3Routes* routes) {
    server->routes = routes;
}

void CWeb3_server_start(CWeb3Server* server) {

}
