#ifdef _WIN32   
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/soc.h"

#pragma comment (lib, "Ws2_32.lib")

char isWSAActive = 0;



CWeb3Socket newCWeb3Socket(CWeb3Config config) {
	CWeb3Socket sock = {0};
	sock.config = config;
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
	sprintf(port, "%i", config.port);
	struct addrinfo hints = { 0 };

	hints.ai_family = AF_INET; // AF_INE6 for ipv6
	hints.ai_socktype = (sock.config.protocol == TCP) ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = (sock.config.protocol == TCP) ? IPPROTO_TCP : IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;


	struct addrinfo* result = NULL;
	iResult = getaddrinfo(sock.config.host, port ,&hints,&result);
	if (iResult != 0) {
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

	return sock;
}

CWeb3Socket CWeb3Listen(CWeb3Socket socket) {
	CWeb3Socket clientSocket = { 0 };
	int iResult = listen(socket.socket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		closesocket(socket.socket);
		WSACleanup();
		CWeb3Socket soc = { 0 };
		return soc;
	}
	SOCKET ClientSocketI = accept(socket.socket, NULL, NULL);
	if (ClientSocketI == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(socket.socket);
		WSACleanup();
		CWeb3Socket soc = { 0 };
		return soc;
	}
	clientSocket.socket = ClientSocketI;
	return clientSocket;
}

size_t CWeb3RecvChunk(CWeb3Socket clientSocket, char* buffer, size_t bufferSize) {
	return recv(clientSocket.socket, buffer, (int)bufferSize, 0);
}

void CWeb3Send(CWeb3Socket clientSocket, char* buffer, size_t bufferSize) {
	send(clientSocket.socket, buffer, (int)bufferSize, 0);
}

void CWeb3CloseSocket(CWeb3Socket socket) {
	closesocket(socket.socket);
}

#endif /* _WIN32 */   