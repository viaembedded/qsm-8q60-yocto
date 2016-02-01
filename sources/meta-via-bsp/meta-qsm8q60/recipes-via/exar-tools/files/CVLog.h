#ifndef __CVLOG_H__
#define __CVLOG_H__

#include <stdarg.h>
#include "common.h"

#define LOGV(...)		LOG(0,	 LOG_TAG, __VA_ARGS__)
#define LOGD(...)		LOG(0,	 LOG_TAG, __VA_ARGS__)
#define LOGI(...)		LOG(0,	 LOG_TAG, __VA_ARGS__)
#define LOGW(...)		LOG(0,	 LOG_TAG, __VA_ARGS__)
#define LOGE(...)		LOG(0,	 LOG_TAG, __VA_ARGS__)

#define LOGEM(...)		LOGM(0,	 LOG_TAG, __VA_ARGS__)		// Log with errno string
#define LOGEN(no, ...)	LOGN(0,	 LOG_TAG, no, __VA_ARGS__)	// Log with errno string


// for Debug message display
#ifdef	VDEBUG

#define VLOGD(...)		LOG(0,	LOG_TAG, __VA_ARGS__)		// Debug Log
#define VLOGDM(...)		LOGM(0,	LOG_TAG, __VA_ARGS__)		// Debug Log with errno string
#define VLOGDN(no, ...)	LOGN(0,	LOG_TAG, no, __VA_ARGS__)	// Debug Log with errno string

#else

#define VLOGD(...)
#define VLOGDM(...)
#define VLOGDN(no, ...)

#endif


void LOG(int nType, const char *sTag, const char *sFormat, ...);
void LOGM(int nType, const char *sTag, const char *sFormat, ...);
void LOGN(int nType, const char *sTag, int nErrNo, const char *sFormat, ...);


/* ============================= CVLog CLASS ========================== */
class CVLog
{
public:

	CVLog();
	~CVLog();

	void	Log(int nType, const char *sTag, const char *sFormat, va_list args);

	void	ErrorMessage(int nType, const char *sTag);

	void	ErrorMessage(int nType, const char *sTag, int nErrNo);

protected:

private:

};

#endif	// __CVLOG_H__
