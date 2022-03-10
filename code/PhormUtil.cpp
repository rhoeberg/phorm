#include <stdarg.h>

char* ReadFile(const char* fileName)
{
    char* result = 0;
    FILE* file = fopen(fileName, "rb");
    if(file) {
        fseek(file, 0, SEEK_END);
        size_t fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        result = (char*)malloc(fileSize + 1);
        fread(result, fileSize, 1, file);
        result[fileSize] = 0;

        fclose(file);
    }
    else {
        printf("cannot find file %s\n", fileName);
    }

    return result;
}

void _DebugLog(char *filename, i32 linenumber, char *format, ...)
{
	printf("%s(%d): warning, ", filename, linenumber);

	va_list args;
	va_start(args, format);
	char buffer[128];
	vsprintf(buffer, format, args);
	vprintf(buffer, {});
	va_end(args);

	printf("\n");

	ConsoleAddMessage(String(buffer), vec3(1.0f, 1.0f, 0.0f));
}

void _ConsoleLog(char *filename, i32 linenumber, char *format, ...)
{
	printf("%s(%d): warning, ", filename, linenumber);

	va_list args;
	va_start(args, format);
	char buffer[128];
	vsprintf(buffer, format, args);
	vprintf(buffer, {});
	va_end(args);

	printf("\n");

	ConsoleAddMessage(String(buffer), vec3(1.0f, 1.0f, 1.0f));
}

void _WarningLog(char *filename, i32 linenumber, char *format, ...)
{
	printf("%s(%d): error, ", filename, linenumber);

	va_list args;
	va_start(args, format);
	char buffer[128];
	vsprintf(buffer, format, args);
	vprintf(buffer, {});
	va_end(args);

	printf("\n");

	ConsoleAddMessage(String(buffer), vec3(1.0f, 1.0f, 0.0f));
}

void _ErrorLog(char *filename, i32 linenumber, char *format, ...)
{
	printf("%s(%d): error, ", filename, linenumber);

	va_list args;
	va_start(args, format);
	char buffer[128];
	vsprintf(buffer, format, args);
	vprintf(buffer, {});
	va_end(args);

	printf("\n");

	ConsoleAddMessage(String(buffer), vec3(1.0f, 0.0f, 0.0f));
}
