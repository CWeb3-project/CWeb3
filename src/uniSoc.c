#include "../include/soc.h"
#include <stdlib.h>
#include <stdio.h>

char* CWeb3Recv(CWeb3Socket clientSocket, size_t* pBufferSize) {

    size_t size = 3072;
    const size_t step = 2048;
    size_t offset = 0;
    size_t messageSize = size;
    char* buffer = malloc(size); // malloc so realloc could be used later

    while (1) {
        messageSize = CWeb3RecvChunk(clientSocket, buffer+offset, size-offset);

        if (messageSize == size-offset) {
            offset = size;
            size += step;
            buffer = realloc(buffer, size);
        } else break;

    }
    printf("BUF: %s\n", buffer);

    size_t bufferSize = offset+messageSize;
    buffer = realloc(buffer, bufferSize+1);
    buffer[bufferSize] = '\0';
    bufferSize++;
    *pBufferSize = bufferSize;
    return buffer;
}