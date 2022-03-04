#pragma once

char* ReadFile(const char* fileName);

void _DebugLog(char *filename, i32 linenumber, char *format, ...);
void _ErrorLog(char *filename, i32 linenumber, char *format, ...);

#define DebugLog(fmt, ...) _DebugLog(##__FILE__, ##__LINE__, fmt, ##__VA_ARGS__)
#define ErrorLog(fmt, ...) _ErrorLog(##__FILE__, ##__LINE__, fmt, ##__VA_ARGS__)
