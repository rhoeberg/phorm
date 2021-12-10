#pragma once

#define STRING_STARTING_LENGTH 128

struct String
{
	char *buffer;
	int length;
	size_t bufferSize;

	String();
	String(char *str);
	String(String &str);
	void ResizeBuffer(int min);
	void Concat(char *str);
	String& operator=(char *str);
	char operator[](int index);
	void Print();
	bool Equals(char *str);
	bool Equals(String other);
	~String();
	void Free();
};

size_t StrSize(char *str);
