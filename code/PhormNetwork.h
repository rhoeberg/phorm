#pragma once

/* #include <sys/socket.h> */
/* #include <arpa/inet.h> */



#define NETWORK_BUFFER_SIZE 512
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5000

/* #include <winsock2.h> */
/* #pragma comment(lib,"ws2_32.lib") //Winsock Library */


struct NetworkState
{
	WSADATA wsaData;
	SOCKET socket;
	sockaddr_in server, other;


	/* UDPAdress address; */
	/* socket_desc socket; */
};

/* struct UDPAddress */
/* { */
/* 	String ip; */
/* 	i32 port; */
/* } */


void InitializeUDP();
bool UPDSend(u8 *buffer, u64 size);
bool UPDRecieve(u8 *buffer, u64 size);

global NetworkState *_networkState;
