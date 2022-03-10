/*
  OSC message consists of:
  - address string
  - type tag string
  - arguments
  
  each string ends with a NULL and are then padded with extra NULLs until the next byte


  OSC message example:

  address: "/the/path\0\0\0"
  type: ",fff\0\0\0\0"
  data: three 32 bit float values

  ---
  For now we keep the implementation simple
  we support the basic phorm data types:
  - float
  - vec3
  - int

  which means we only accept the type tags: 
  - r
  - rrr
  - i
 */

#pragma once

struct OSCState
{
	PMap<String, vec3> vec3Inputs;
	/* PMap<String, vec3> vec3Inputs; */
};

void InitializeOSC();
String GetOSCAddress(NetworkMessage message);
void UpdateOSC();

global OSCState *_OSCState;
