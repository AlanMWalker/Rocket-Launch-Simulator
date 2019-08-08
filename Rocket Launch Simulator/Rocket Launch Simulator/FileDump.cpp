#include <stdarg.h>
#include "FileDump.h"	

#define DUMP_FILE_NAME "Rock_Sim_Run.txt"

static FILE* pDumpFile = NULL;

bool setup_dump_file()
{
	const errno_t openResult = fopen_s(&pDumpFile, DUMP_FILE_NAME, "w");
	if (openResult != 0)
	{
		printf_s("Failed to open dump file\n");
		return false;
	}
	return true;
}

void write_to_dump_file(const char * format, ...)
{
	va_list argList;
	va_start(argList, format);
	vfprintf_s(pDumpFile, format, argList);
	va_end(argList);
}

void close_dump_file()
{
	fclose(pDumpFile);
}


