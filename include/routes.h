
#include "response.h"
#include <stddef.h>

#ifndef __CWEB3_ROUTES_H_
#define __CWEB3_ROUTES_H_

typedef struct {
    const char* route;
    void (*callback)();

} CWeb3Route;

typedef struct
{
    CWeb3Route *array;

    size_t size;
    size_t used;
} CWeb3Routes;

CWeb3Routes CWeb3_new_routes();
CWeb3Routes CWeb3_add_route(CWeb3Routes routes, const char* route, void (*callback)());

#endif /* __CWEB3_ROUTES_H_ */
