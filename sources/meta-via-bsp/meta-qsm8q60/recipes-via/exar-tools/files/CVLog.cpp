#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "CVLog.h"


CVLog	g_Log;


void LOG(int nType, const char *sTag, const char *sFormat, ...)
{
	va_list args;

	va_start(args, sFormat);
	g_Log.Log(nType, sTag, sFormat, args);
	va_end(args);
}


void LOGM(int nType, const char *sTag, const char *sFormat, ...)
{
	if(sFormat && strlen(sFormat))
	{
		va_list args;

		va_start(args, sFormat);
		g_Log.Log(nType, sTag, sFormat, args);
		va_end(args);
	}

	g_Log.ErrorMessage(nType, sTag);
}


void LOGN(int nType, const char *sTag, int nErrNo, const char *sFormat, ...)
{
	if(sFormat && strlen(sFormat))
	{
		va_list args;

		va_start(args, sFormat);
		g_Log.Log(nType, sTag, sFormat, args);
		va_end(args);
	}

	g_Log.ErrorMessage(nType, sTag, nErrNo);
}


/* ============================= CVLog CLASS ========================== */
CVLog::CVLog()
{
}


CVLog::~CVLog()
{
}


void CVLog::Log(int nType, const char *sTag, const char *sFormat, va_list args)
{
	vprintf(sFormat, args);
	printf("\n");
}


void CVLog::ErrorMessage(int nType, const char *sTag)
{
	printf("errno : %d, %s\n", errno, strerror(errno));
}


void CVLog::ErrorMessage(int nType, const char *sTag, int nErrNo)
{
	printf("errno : %d, %s\n", nErrNo, strerror(nErrNo));
}
