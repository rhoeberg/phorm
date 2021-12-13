#pragma once

#define STRING_STARTING_LENGTH 128

struct String
{
	char *buffer;
	int length;
	size_t bufferSize;
	bool initialized;

	String();
	String(char *str);
	String(String &str);
	void FindBufferSize(int min);
	void ResizeBuffer(int min);
	void Concat(char *str);
	void Concat(String& other);
	String& operator=(char *str);
	String& operator=(const String &other);
	char operator[](int index);
	void Print();
	bool Equals(char *str);
	bool Equals(String &other);
	~String();
	void Free();
};

size_t StrSize(char *str);
