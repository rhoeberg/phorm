void InitializeNetwork()
{
	DebugLog("Network: initialize");
	WSADATA wsaData;
	i32 result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(result != 0) {
		ErrorLog("network: could not initalize WSAData: %d", result);
		return;
	}
}

UDPConnection::UDPConnection(i32 _port)
{
	DebugLog("Initializing UDP connection");

	port = _port;

	socketHandle = INVALID_SOCKET;

	// init socket
	socketHandle = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketHandle == INVALID_SOCKET) {
		ErrorLog("UDPConnection: could not initalize socket");
		return;
	}

	// prepare sockaddr and bind
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if(bind(socketHandle, (struct sockaddr*)&server, sizeof( sockaddr)) != 0) {
		ErrorLog("network: could not bind UDP socket to address");
		return;
	}

	//-------------------------
	// Set the socket I/O mode: In this case FIONBIO
	// enables or disables the blocking mode for the 
	// socket based on the numerical value of iMode.
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	u_long iMode = 1;
	i32 iResult = ioctlsocket(socketHandle, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
		printf("ioctlsocket failed with error: %ld\n", iResult);

	DebugLog("Done Initializing UDP connection");
}

i32 UDPConnection::Recieve(char *buffer, u32 size)
{
	// return recvfrom(_networkState->socket, buffer, size, 0, NULL, NULL);
	return recvfrom(socketHandle, buffer, size, 0, NULL, NULL);
}

void UDPConnection::Close()
{
	closesocket(socketHandle);
}

void CleanupNetwork()
{
	WSACleanup();
}
