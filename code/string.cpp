#include "string.h"

// TODO (rhoe) unsafe if using non terminated string
size_t StrSize(char *str)
{
	char *current = str;

	int size = 0;
	while(current[0] != '\0') {
		size++;
		current++;
	}

	return size;
}

// used if string buffer is modified outside of string to
// re calculate length.
// Modifying buffer directly is ofcourse very unsafe and should be
// done with precautions. But this type has the safeguards OFF.
// Therefore if you need to modify buffer directly its very important
// that you make sure to null terminate the string

void String::ReCalc()
{
	length = StrSize(buffer);
}

void String::FindBufferSize(int min)
{
	while(min+1 > bufferSize) {
		bufferSize += STRING_STARTING_LENGTH;
	}
}

void String::ResizeBuffer(int min)
{
	if(min <= bufferSize)
		return;

	FindBufferSize(min);
		
	buffer = (char*)realloc(buffer, bufferSize);
}

String::String()
{
	bufferSize = STRING_STARTING_LENGTH;
	buffer = (char*)calloc(1, bufferSize);
	length = 0;
	initialized = true;
}

String::String(char *str)
{
	bufferSize = STRING_STARTING_LENGTH;
	length = StrSize(str);
	FindBufferSize(length);
	buffer = (char*)calloc(1, bufferSize);
	buffer[length] = '\0';
	memcpy(buffer, str, length);
	initialized = true;
}

String::String(String &other)
{
	bufferSize = other.bufferSize;
	length = other.length;
	buffer = (char*)calloc(1, bufferSize);
	memcpy(buffer, other.buffer, other.length);
	buffer[length] = '\0';
	initialized = true;
}

void String::Concat(char *str)
{
	size_t strSize = StrSize(str);
	if((length + strSize) > bufferSize) {
		ResizeBuffer(strSize + length);
	}

	char *strPos = buffer + length;
	memcpy(strPos, str, strSize);
	length += strSize;
	buffer[length] = '\0';
}

void String::Concat(String& other)
{
	if((length + other.length) > bufferSize) {
		ResizeBuffer(length + other.length);
	}

	char *strPos = buffer + length;
	memcpy(strPos, other.buffer, other.length);
	length += other.length;
	buffer[length] = '\0';
}

String& String::operator=(char *str) {
	Free();
	*this = String(str);
	return *this;
}

String& String::operator=(const String &other) {
	Free();

	bufferSize = other.bufferSize;
	length = other.length;
	buffer = (char*)calloc(1, bufferSize);
	memcpy(buffer, other.buffer, other.length);
	buffer[length] = '\0';

	return *this;
}

char String::operator[](int index) {
	if(index < 0 || index > length-1) {
		NOT_IMPLEMENTED;
	}

	return buffer[index];
}

void String::Print() {
	printf("%s\n", buffer);
}

bool String::Equals(char *str)
{
	if(StrSize(str) != length)
		return false;

	for(int i = 0; i < length; i++) {
		if(str[i] != buffer[i])
			return false;
	}

	return true;
}

bool String::Equals(String &other)
{
	if(other.length != length)
		return false;

	for(int i = 0; i < length; i++) {
		if(other[i] != buffer[i])
			return false;
	}

	return true;
}

String::~String() {
	Free();
}

void String::Free() {
	if(initialized)
		free(buffer);
}

void Test_String()
{
	String s1 = "bla bla testing";
	String s2 = "rasmus";
	String s3 = "rasmus";
	String s4 = "concatme";
	s4.Concat(s2);

	printf("=== TESTING STRING TYPE\n");
	printf("s1: %s\n", s1.buffer);
	printf("s1 null terminator: %d\n", s1.buffer[s1.length]);
	printf("s4: %s\n", s4.buffer);

	ASSERT(s1.length == 15);
	ASSERT(s1.length != 0);
	ASSERT(s1.length != 14);
	ASSERT(s1.length != 16);
	ASSERT(s1.length != 1000);
	ASSERT(s2.Equals(s3));
	ASSERT(s2.Equals("rasmus"));
	ASSERT(!s2.Equals("peter"));
	ASSERT(s1.buffer[s1.length] == 0);
	ASSERT(s1.buffer[s1.length] == '\0');
	ASSERT(s4.length == 14);
	ASSERT(s4.buffer[s4.length] == 0);
	
	printf("STRING TEST: success\n");
}
