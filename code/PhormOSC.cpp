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

	String result(&message.buffer[0]);
	return result;
}

float ReverseFloat( const float inFloat )
{
   float retVal;
   char *floatToConvert = ( char* ) & inFloat;
   char *returnFloat = ( char* ) & retVal;

   // swap the bytes into a temporary buffer
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];

   return retVal;
}

vec3 OSCTryGetVec3(String address)
{
	vec3 *result = _OSCState->vec3Inputs.Get(address);
	if(result) {
		return *result;
	}

	return vec3(0.0f, 0.0f, 0.0f);
}

void UpdateOSC()
{
	// read last message from UDP
	// if OSC message (string starting with '/' and ending with '\0')
	//    add to hashmap with string/float?

	// if(UDPNewMessage()) {
	// 	NetworkMessage message = GetLastMessage();
	// 	DebugLog("recieved data:");
	// 	// DebugLog("%d, %s", message.length, message.buffer);
	// 	if(message.buffer[0] == '/') {
	// 		// message is OSC
	// 		String address = GetOSCAddress(message);
	// 		DebugLog("address: %s, %d", address.buffer, address.length);

	// 		i32 mod = (address.length + 1) % 4;
	// 		i32 typeOffset = address.length + 1;
	// 		if(mod == 0) {
	// 			// no extra padding needed
	// 		}
	// 		else {
	// 			typeOffset += (4 - mod);
	// 		}

	// 		// i32 padding = (4 - ((address.length + 1) % 4)) + 1;
	// 		String type(&message.buffer[typeOffset]);
	// 		// DebugLog("padding:%d", padding);
	// 		DebugLog("type: %s, %d", type.buffer, type.length);

	// 		i32 dataOffset = typeOffset + type.length + 1;
	// 		mod = (type.length + 1) % 4;
	// 		if(mod != 0) {
	// 			dataOffset += (4 - mod);
	// 		}

	// 		if(!type.Equals(",fff")) {
	// 			WarningLog("OSC: typetag not supported, %s", type.buffer);

	// 		}
	// 		else {
	// 			WarningLog("dataoffset: %d", dataOffset);
	// 			WarningLog("as char: %c", message.buffer[dataOffset]);
	// 			vec3 result = {};
	// 			// f32 *ptr = reinterpret_cast<f32*>(&message.buffer[dataOffset]);
	// 			f32 *ptr = (f32*)(&message.buffer[dataOffset]);

	// 			// f32 *ptr = (f32*)&message.buffer[dataOffset];
	// 			result.x = ReverseFloat(ptr[0]);
	// 			result.y = ReverseFloat(ptr[1]);
	// 			result.z = ReverseFloat(ptr[2]);

	// 			_OSCState->vec3Inputs.Insert(address, result);

	// 			DebugLog("iterating vec3 input hashmap");
	// 			HashIter<String, vec3> iter(&_OSCState->vec3Inputs);
	// 			HashNode<String, vec3> *next = iter.Next();
	// 			while(next) {

	// 				// do stuff
	// 				DebugLog("value: %f %f %f", next->value.x, next->value.y, next->value.z);

	// 				next = iter.Next();
	// 			}
	// 		}

	// 		// String type = 

	// 		// DebugLog("osc address: %s", address.buffer);
	// 	}

	// }
}

void CleanupOSCState()
{
	delete _OSCState;
}
