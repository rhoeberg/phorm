#pragma once


char* readFile(const char* fileName);
/* void DebugLog(char *format, ...); */
/* void ErrorLog(char *format, ...); */
void _DebugLog(char *format, char *filename, i32 linenumber, ...);
void _ErrorLog(char *format, char *filename, i32 linenumber, ...);

#define DebugLog(fmt, ...) _DebugLog(fmt, ##__FILE__, ##__LINE__, ##__VA_ARGS__)
#define ErrorLog(fmt, ...) _ErrorLog(fmt, ##__FILE__, ##__LINE__, ##__VA_ARGS__)
