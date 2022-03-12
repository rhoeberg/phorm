#pragma once

#define STRING_STARTING_LENGTH 128

struct String
{
	char *buffer;
	int length;
	size_t bufferSize;
	bool initialized;

	String();
	String(const char *str);
	String(const String &str);
	void ReCalc();
	void FindBufferSize(int min);
	void ResizeBuffer(int min);

	// TODO (rhoe) make concat immutable?
	void Concat(const char *str);
	void Concat(String& other);
	void Concat(int i);

	String& operator=(const char *str);
	String& operator=(const String other);
	// String& operator=(String other);
	char operator[](int index);
	void Print();
	bool operator==(String &other);
	bool operator!=(String &other);
	bool Equals(const char *str);
	bool Equals(String &other);
	~String();
	void Free();
};

size_t StrSize(const char *str);
