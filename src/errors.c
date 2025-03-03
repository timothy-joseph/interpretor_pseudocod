#include <errors.h>

#include <stdio.h>
#include <stdarg.h>

void
error(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	free_on_quit();
	exit(-1);
}

