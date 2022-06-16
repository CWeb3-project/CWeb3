
#include "../include/routes.h"
#include "../include/server.h"
#include "../include/soc.h"
#include <stdio.h>

void index() {

}

int main() {

    CWeb3Config config;
    config.host = "127.0.0.1";
    config.port = 8000;
    config.protocol = TCP;
    config.verbose = 1;

    CWeb3Routes* routes = CWeb3_new_routes();
    CWeb3_add_route(routes, "/", index);

    CWeb3Server server = CWeb3_create_server(config);
    CWeb3_server_merge_routes(server, routes);

    CWeb3_server_start(server);
}