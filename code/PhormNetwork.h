#pragma once

#include <thread>
#include <mutex>

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

struct UDPConnection
{
	SOCKET socketHandle;
	sockaddr_in server;
	sockaddr_in other;
	i32 port;

	UDPConnection(){}
	UDPConnection(i32 _port);
	void Close();
	i32 Recieve(char *buffer, u32 size);
};

void InitializeNetwork();
void CleanupNetwork();
