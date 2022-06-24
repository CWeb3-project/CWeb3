#include <stddef.h>
#include "request.h"
#include "response.h"

#ifndef __CWEB3_ROUTES_H_
#define __CWEB3_ROUTES_H_

typedef struct {
    const char* route;
    void (*callback)(CWeb3Request*, CWeb3Response*);

} CWeb3Route;

typedef struct
{
    CWeb3Route *array;

    size_t size;
    size_t used;
} CWeb3Routes;

CWeb3Routes CWeb3_new_routes();
void CWeb3_add_route(CWeb3Routes* pRoutes, const char* _route, void (*callback)());

#endif /* __CWEB3_ROUTES_H_ */
