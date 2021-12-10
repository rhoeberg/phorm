#include "string.h"

String::String()
{
	bufferSize = STRING_STARTING_LENGTH;
	buffer = (char*)malloc(bufferSize);
	length = 0;
}

String::String(char *str)
{
	bufferSize = STRING_STARTING_LENGTH;
	length = StrSize(str);
	while(length > bufferSize) {
		bufferSize += STRING_STARTING_LENGTH;
	}
	buffer = (char*)malloc(bufferSize);
	memcpy(buffer, str, length);
}

String::String(String &other)
{
	bufferSize = other.bufferSize;
	length = other.length;
	buffer = (char*)malloc(bufferSize);
	memcpy(buffer, other.buffer, other.length);
}

void String::ResizeBuffer(int min)
{
	if(min <= bufferSize)
		return;

	while(min > bufferSize) {
		bufferSize += STRING_STARTING_LENGTH;
	}
		
	buffer = (char*)realloc(buffer, bufferSize);
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
}

String& String::operator=(char *str) {
	Free();
	*this = String(str);
	return *this;
}

char String::operator[](int index) {
	if(index < 0 || index > length-1) {
		NOT_IMPLEMENTED;
	}

	return buffer[index];
}

void String::Print() {
	printf("%.*s\n", length, buffer);
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

bool String::Equals(String other)
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
	free(buffer);
}

void String::Free() {
	free(buffer);
}

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
