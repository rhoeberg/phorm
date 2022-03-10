#pragma once

#include <thread>
#include <mutex>

/* #include <sys/socket.h> */
/* #include <arpa/inet.h> */



#define NETWORK_BUFFER_SIZE 512
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5000

/* #include <winsock2.h> */
/* #pragma comment(lib,"ws2_32.lib") //Winsock Library */

struct NetworkMessage
{
	char buffer[NETWORK_BUFFER_SIZE];
	i32 length;
};

struct NetworkUDPListener
{
	std::thread thread;
	bool shouldStop;
};

struct NetworkState
{
	WSADATA wsaData;
	SOCKET socket;
	sockaddr_in server, other;
	NetworkMessage lastMessage;
	NetworkUDPListener udpListener;
	std::mutex msgLock;
	bool messageConsumed;


	/* UDPAdress address; */
	/* socket_desc socket; */
};

/* struct UDPAddress */
/* { */
/* 	String ip; */
/* 	i32 port; */
/* } */


void InitializeUDP();
void ListenUDP();
bool UPDSend(u8 *buffer, u64 size);
bool UPDRecieve(u8 *buffer, u64 size);

global NetworkState *_networkState;
