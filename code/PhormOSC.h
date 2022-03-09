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

 */

#pragma once

