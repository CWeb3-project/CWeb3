
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/routes.h"

CWeb3Routes CWeb3_new_routes() {
    CWeb3Routes routes;

    routes.array = malloc(100 * sizeof(CWeb3Route));
    routes.used = 0;
    routes.size = 0;

    return routes;
}

CWeb3Routes CWeb3_add_route(CWeb3Routes routes, const char* _route, void (*callback)()) {
    // routes.used is the number of used entries, because routes.array[routes.used++] updates routes.used only *after* the array has been accessed.
    // Therefore routes.used can go up to routes.size
    CWeb3Route route;
    size_t strSize = strlen(_route)+1;
    route.route = malloc(strSize);
    memcpy(route.route, _route, strSize);
    route.callback = callback;

    if (routes.used == routes.size) {
        routes.size += 1;
        routes.array = realloc(routes.array, routes.size * sizeof(CWeb3Route));
    }

    routes.array[routes.used++] = route;
    return routes;
}

