#include "log.h"

#include <stdio.h>
#include <stdarg.h> 
#include <stdio.h> 

#if WIN
#include <Windows.h>
#endif


void log_info(const char* str, ...)
{
	char msg[5000];
	va_list ap;
	va_start(ap, str);
	vsnprintf(msg, sizeof(char) * 5000, str, ap);
	va_end(ap);

	printf("%s\n", msg);
}


void log_error(const char* str, ...)
{
	char msg[5000];
	va_list ap;
	va_start(ap, str);
	vsnprintf(msg, sizeof(char) * 5000, str, ap);
	va_end(ap);

#if !DEBUG && WIN
	MessageBox(NULL, str, msg, MB_OKCANCEL);
#else
	printf("ERROR: %s\n", msg);
#endif
}