#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h>
#include<WS2tcpip.h>
#include <process.h>

#pragma comment(lib,"Ws2_32.lib")
#define BUFF_SIZE 2048
#define MAX_CLIENT 1024

//echoThread - Thread to receive the message from client and echo
unsigned __stdcall echoThread(void *param) {
	char buff[BUFSIZ];
	int ret;
	SOCKET connectedSocket = (SOCKET)param;
	ret = recv(connectedSocket, buff, BUFF_SIZE, 0);
	if (ret < 0) {
		printf("Socket closed.\n");
	}
	else
	{
		ret = send(connectedSocket, buff, ret, 0);
		if (ret < 0) {
			printf("Socket closed.\n");
		}
	}

	closesocket(connectedSocket);
	return 0;
}


int main(int argc, char **argv) {
	unsigned short port_number;			/* Port number to use */

										//step 1: Initiate Winsock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Version is not supported\n");
	}


	//step 2: Construct socket
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//step 3: Bind assress to socket
	port_number = atoi(argv[1]);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port_number);
	serverAddr.sin_addr.s_addr = htond(INADDR_ANY);

	if (bind(listenSock, (sockaddr *)&serverAddr, sizeof(serverAddr)))
	{
		printf("Error! Cannot bind this address.");
		_getch();
		return 0;
	}

	//step 4: Listen request from client
	if (listen(listenSock, 10)) {
		printf("Error! Cannot listen.");
		_getch();
		return 0;
	}

	printf("Server started!\n");

	//step 5: Communicate with  client
	SOCKET connSock;
	sockaddr_in clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	int i;
	while (true)
	{
		connSock = accept(listenSock, (sockaddr *)& clientAddr, &clientAddrLen); //new connection
		printf("connSoc: %d", connSock);
		_beginthreadex(0, 0, echoThread, (void *)connSock, 0, 0); //start thread
	}
	closesocket(listenSock);
	WSACleanup();
	_getch();
	return 0;
}

