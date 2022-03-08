void InitializeUDP()
{
	_networkState = (NetworkState*)malloc(sizeof(NetworkState));

	
	// Init winsock
	i32 result = WSAStartup(MAKEWORD(2,2), &_networkState->wsaData);
	if(result != 0) {
		ErrorLog("network: could not initalize WSAData: %d", result);
		return;
	}

	// init socket
	_networkState->socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(_networkState->socket == INVALID_SOCKET) {
		ErrorLog("network: could not initalize socket: %d", result);
		return;
	}

	// prepare sockaddr and bind
	_networkState->server.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	_networkState->server.sin_family = AF_INET;
	_networkState->server.sin_port = htons(SERVER_PORT);

	if(bind(_networkState->socket, (sockaddr*)&_networkState->server, sizeof(_networkState->server)) == SOCKET_ERROR) {
		ErrorLog("network: could not bind UDP socket to address");
		return;
	}

	// char buffer[NETWORK_BUFFER_SIZE];
	char *buffer = (char*)malloc(NETWORK_BUFFER_SIZE);
	i32 recvlen = 0;
	i32 msgLen = sizeof(_networkState->other);
	while(1) {
		memset(buffer, '\0', NETWORK_BUFFER_SIZE);
		recvlen = recvfrom(_networkState->socket, buffer, NETWORK_BUFFER_SIZE, 0, (sockaddr*)&_networkState->other, &msgLen);
		if(recvlen == SOCKET_ERROR) {
			ErrorLog("network: failed to recieve package: %d", WSAGetLastError());
			continue;
		}

		// buffer should be filled with incomming data
		// do stuff with it
		DebugLog("recieved data:");
		DebugLog("%s", buffer);

		//now reply the client with the same data
		// if (sendto(_networkState->socket, (char*)buffer, recvlen, 0, (sockaddr*)&_networkState->other, msgLen) == SOCKET_ERROR)
		// {
		// 	printf("network: sendto() failed with error code : %d" , WSAGetLastError());
		// }
	}

	
}

// void NetworkListen()
// {
// }

void CleanupNetwork()
{
	closesocket(_networkState->socket);
	WSACleanup();
}
