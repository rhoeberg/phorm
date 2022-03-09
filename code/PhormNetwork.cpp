void InitializeUDP()
{
	// _networkState = (NetworkState*)malloc(sizeof(NetworkState));
	_networkState = new NetworkState();

	
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

	_networkState->udpListener.shouldStop = false;
	_networkState->udpListener.thread = std::thread(ListenUDP);
	// ListenUDP();
}

NetworkMessage GetLastMessage()
{
	return _networkState->lastMessage;
}

void ListenUDP()
{
	DebugLog("listening to port 5000");
	i32 msgLen = sizeof(_networkState->other);
	memset(_networkState->lastMessage.buffer, '\0', NETWORK_BUFFER_SIZE);
	while(!_networkState->udpListener.shouldStop) {
		_networkState->lastMessage.length = recvfrom(_networkState->socket, _networkState->lastMessage.buffer, NETWORK_BUFFER_SIZE, 0, (sockaddr*)&_networkState->other, &msgLen);
		if(_networkState->lastMessage.length == SOCKET_ERROR) {
			ErrorLog("network: failed to recieve package: %d", WSAGetLastError());
			continue;
		}

		// buffer should be filled with incomming data
		// do stuff with it
		DebugLog("recieved data:");
		DebugLog("%d, %s", _networkState->lastMessage.length, _networkState->lastMessage.buffer);

		//now reply the client with the same data
		// if (sendto(_networkState->socket, (char*)buffer, recvlen, 0, (sockaddr*)&_networkState->other, msgLen) == SOCKET_ERROR)
		// {
		// 	printf("network: sendto() failed with error code : %d" , WSAGetLastError());
		// }
	}
}

void CleanupNetwork()
{
	closesocket(_networkState->socket);
	_networkState->udpListener.shouldStop = true;
	_networkState->udpListener.thread.join();
	WSACleanup();
}
