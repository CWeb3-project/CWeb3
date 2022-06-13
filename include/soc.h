#include "config.h"

#ifndef __CWEB3_H_
#define __CWEB3_H_
#pragma once

#ifdef __linux__ 

typedef struct {
    CWeb3Config config;
    int scoketID;
} CWeb3Socket;

CWeb3Socket CWeb3_newSocket(CWeb3Config conf, int protocol);

#endif /* __linux__ */

#endif /* __CWEB3_H_ */
