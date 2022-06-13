

#ifndef __CWEB3_CONFIGURATION_H_
#define __CWEB3_CONFIGURATION_H_

struct CWeb3Config {
    int port;
    const char* host = "localhost";

    bool verbose = true;
};

#endif /* __CWEB3_CONFIGURATION_H_ */

