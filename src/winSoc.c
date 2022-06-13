#ifdef _WIN32   
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/soc.h"

#pragma comment (lib, "Ws2_32.lib")

char isWSAActive = 0;



CWeb3Socket newCWeb3Socket(CWeb3Config config) {
	CWeb3Socket sock;
	WSADATA wsaData;
	int iResult;

	if (!isWSAActive) {
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult) {
			CWeb3Socket sock = { 0 };
			return sock;
		}
		isWSAActive = 1;
	}
	
	char port[12] = { 0 };
	itoa(config.port, port, 10);
	struct addrinfo hints;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = (sock.config.protocol == TCP) ? IPPROTO_TCP : IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo* result;
	iResult = getaddrinfo(NULL, port, &hints, &result);
	if (iResult) {
		WSACleanup();
		CWeb3Socket sock = { 0 };
		return sock;
	}

	SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		CWeb3Socket sock = { 0 };
		return sock;
	}

	sock.socket = ListenSocket;

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		CWeb3Socket sock = { 0 };
		return sock;
	}
	freeaddrinfo(result);
	


}

int CWeb3Listen(CWeb3Socket sock, char* buffer, size_t bufferSize) {
	int iResult = listen(sock.socket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		closesocket(sock.socket);
		WSACleanup();
		return -1;
	}
}

#endif /* _WIN32 */   