#include "../include/soc.h"
#include <stdlib.h>

char* CWeb3Recv(CWeb3Socket clientSocket, size_t* bufferSize) {
    const size_t size = 3000;
    char* buffer = malloc(size); // malloc so realloc could be used later

    size_t messageSize = CWeb3RecvChunk(clientSocket, buffer, size);
    size_t newBufferSize = messageSize+1;
    buffer = realloc(buffer, newBufferSize); // resize the array and add single byte for 0 

    if (size < messageSize) {
        CWeb3RecvChunk(clientSocket, buffer+size, messageSize-size); // adjust the array so the second part is concataned to the end of the first part
    }
    buffer[messageSize] = '\0';
    *bufferSize = newBufferSize;
    return buffer;
}
