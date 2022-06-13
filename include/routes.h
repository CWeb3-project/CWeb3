
#include "response.h"

#ifndef __CWEB3_CONFIGURATION_H_
#define __CWEB3_CONFIGURATION_H_

typedef struct {
    const char* route;
    CWeb3Response (*callback)();

} CWeb3Route;

#endif /* __CWEB3_CONFIGURATION_H_ */
