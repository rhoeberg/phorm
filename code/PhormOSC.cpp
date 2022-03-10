void InitializeOSC()
{
	_OSCState = new OSCState();
}

String GetOSCAddress(NetworkMessage message)
{
	/* i32 addressEnd = 1; */
	/* for(i32 i = 1; i < message.length; i++) { */
	/* 	if(message.buffer[i] == '\0') { */
	/* 		addressEnd = i; */
	/* 		break; */
	/* 	} */
	/* } */

	String result(&message.buffer[1]);
	return result;
}

void UpdateOSC()
{
	// read last message from UDP
	// if OSC message (string starting with '/' and ending with '\0')
	//    add to hashmap with string/float?

	if(UDPNewMessage()) {
		NetworkMessage message = GetLastMessage();
		if(message.buffer[0] == '/') {
			// message is OSC
			String address = GetOSCAddress(message);

			// DebugLog("osc address: %s", address.buffer);
		}
	}
}

void CleanupOSCState()
{
	delete _OSCState;
}
