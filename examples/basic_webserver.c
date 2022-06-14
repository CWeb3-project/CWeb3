
#include "../include/routes.h"
#include "../include/soc.h"
#include <stdio.h>

void index() {

}

int main() {
    printf("DBG");

    CWeb3Config config;
    config.host = "127.0.0.1";
    config.port = 10003;
    config.protocol = TCP;
    config.verbose = 1;
    printf("HELLO");

    CWeb3Routes* routes = CWeb3_new_routes();
    CWeb3_add_route(routes, "/", index);

    // printf("new route: %s", routes->array[0].route);
}