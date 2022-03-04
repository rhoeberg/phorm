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

	// cannot use normal printf when using varargs, have to use vprintf
	//                   (for sprintf we use vsprintf)
	vprintf(format, args);
	va_end(args);

	printf("\n");
}

void _ErrorLog(char *filename, i32 linenumber, char *format, ...)
{
	printf("%s(%d): error, ", filename, linenumber);

	va_list args;
	va_start(args, format);

	vprintf(format, args);
	va_end(args);

	printf("\n");
}
