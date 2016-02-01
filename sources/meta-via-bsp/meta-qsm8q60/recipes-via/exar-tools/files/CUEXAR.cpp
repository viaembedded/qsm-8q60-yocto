#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "CVLog.h"
#include "CUEXAR.h"

#define UART_USBSERIALPROC	"/proc/tty/driver/usbserial"
#define UART_USBSERIAL		"ttyUSB"
#define UART_USBXR			"ttyXRUSB"
#define UART_USBUEVENT		"/sys/class/tty/%s/device/uevent"
#define UART_USBPORTNUMBER	"/sys/class/tty/%s/device/bInterfaceNumber"

bool SetHalfDuplex_xr21B1411(int uartfd, int nChannel);
bool SetFullDuplex_xr21B1411(int uartfd, int nChannel);
bool SetHalfDuplex_xr21V141x(int uartfd, int nChannel);
bool SetFullDuplex_xr21V141x(int uartfd, int nChannel);


/* ============================= CUEXAR CLASS ========================== */
CUEXAR::CUEXAR()
: m_nChannel(-1)
, m_nVID(0)
, m_nPID(0)
, m_bHalfDuplex(false)
{
	memset(m_sLog, 0, sizeof(m_sLog));
}


CUEXAR::~CUEXAR()
{
}


void CUEXAR::Init()
{
	time_t timep = time(NULL);
	struct tm *data = localtime(&timep);

	sprintf(m_sLog, "%d-%02d-%02d_%02d:%02d:%02d.txt", 1900 + data->tm_year, data->tm_mon + 1, data->tm_mday, data->tm_hour, data->tm_min, data->tm_sec);
}


void CUEXAR::PrintLog(char *sStr, bool bRed)
{
	char sCommand[512] = {0};

	printf("\033[1m %s\033[0m\n", sStr);

	sprintf(sCommand, "echo ' %s' >> %s", sStr, m_sLog);
	system(sCommand);
}


void CUEXAR::PrintLogTime(char *sStr, bool bRed)
{
	char sData[256] = {0}, sCommand[512] = {0};
	time_t timep = time(NULL);
	struct tm *data = localtime(&timep);

	if(bRed)
		printf("%d-%02d-%02d_%02d:%02d:%02d\033[1;31m %s\033[0m\n", 1900 + data->tm_year, data->tm_mon + 1, data->tm_mday, data->tm_hour, data->tm_min, data->tm_sec, sStr);
	else
		printf("%d-%02d-%02d_%02d:%02d:%02d %s\n", 1900 + data->tm_year, data->tm_mon + 1, data->tm_mday, data->tm_hour, data->tm_min, data->tm_sec, sStr);

	sprintf(sData, "%d-%02d-%02d_%02d:%02d:%02d %s", 1900 + data->tm_year, data->tm_mon + 1, data->tm_mday, data->tm_hour, data->tm_min, data->tm_sec, sStr);
	sprintf(sCommand, "echo '%s' >> %s", sData, m_sLog);
	system(sCommand);
}


bool CUEXAR::Open(char *sDevName)
{
	if(CMUART::Open(sDevName))
	{
		if(GetIDfromUSBSerial(sDevName) || GetIDfromSysClass(sDevName))
			SetFullDuplex();
			
		return true;
	}
	
	return false;
}


bool CUEXAR::SetConfig(int nBaudRate, int nDataBits, int nStopBits, int nParity, int nFlowCtrl)
{
	if(CMUART::SetConfig(nBaudRate, nDataBits, nStopBits, nParity, nFlowCtrl))
	{
		if(m_bHalfDuplex)
			SetHalfDuplex();
			
		return true;
	}
	
	return false;
}


bool CUEXAR::SetConfig(UART_CONFIG stConfig)
{
	return SetConfig(stConfig.nBaudRate, stConfig.byDataBits, stConfig.byStopBits, stConfig.byParity, stConfig.byFlowCtrl);
}


bool CUEXAR::SetTimeout(int nTimeout)
{
	if(CMUART::SetTimeout(nTimeout))
	{
		if(m_bHalfDuplex)
			SetHalfDuplex();
			
		return true;
	}
	
	return false;
}


bool CUEXAR::GetIDfromSysClass(char *sDevName)
{
	char sBuf[256] = {0}, *sIdx = NULL, sPath[128] = {0};
	FILE *fp = NULL;

	if(!(sIdx = strstr(sDevName, UART_USBXR)))
		return false;

	sprintf(sPath, UART_USBUEVENT, sIdx);

	if((fp = fopen(sPath, "r")) == NULL)
	{
		LOGEM("%s: create file %s failed!!", __FUNCTION__, sPath);
		return false;
	}

	while(fgets(sBuf, sizeof(sBuf) - 1, fp))
	{
		if(sscanf(sBuf, "PRODUCT=%x/%x/", &m_nVID, &m_nPID) == 2)
		{
			fclose(fp);

			sprintf(sPath, UART_USBPORTNUMBER, sIdx);

			if((fp = fopen(sPath, "r")) == NULL)
			{
				LOGEM("%s: create file %s failed!!", __FUNCTION__, sPath);
				return false;
			}

			if(fgets(sBuf, sizeof(sBuf) - 1, fp))
				sscanf(sBuf, "%x", &m_nChannel);

			if(EXAR_PRODUCT_VIZZINI1 == m_nPID)
				m_nChannel = -1;
			else
				m_nChannel /= 2;

			fclose(fp);
			return true;
		}
	}

	fclose(fp);

	return false;
}


bool CUEXAR::GetIDfromUSBSerial(char *sDevName)
{
	char sBuf[256] = {0}, *sIdx = NULL, sPath[128] = {0};
	int nIdx = 0, nNum = 0;
	FILE *fp = NULL;

	if(!(sIdx = strstr(sDevName, UART_USBSERIAL)))
		return false;

	nIdx = strtol(sIdx + strlen(UART_USBSERIAL), 0, 10);

	if((fp = fopen(UART_USBSERIALPROC, "r")) == NULL)
	{
		LOGEM("%s: create file %s failed!!", __FUNCTION__, UART_USBSERIALPROC);
		return false;
	}

	while(fgets(sBuf, sizeof(sBuf) - 1, fp))
	{
		if(sscanf(sBuf, "%d: %*[^\"]\"%*[^\"]\" vendor:%x product:%x %*[^ ] %*[^ ] path:%s", &nNum, &m_nVID, &m_nPID, sPath) == 4)
		{
			if(nIdx == nNum)
			{
				fclose(fp);
				GetChannel(nIdx, sPath);
				return true;
			}
		}
	}

	fclose(fp);

	return false;
}


void CUEXAR::GetChannel(int nDevNum, char *sName)
{
	char sBuf[256] = {0}, sPath[128] = {0};
	int nNum = 0, nVendor = 0, nProduct = 0;;
	FILE *fp = NULL;
	
	m_nChannel = 0;

	if((fp = fopen(UART_USBSERIALPROC, "r")) == NULL)
	{
		LOGEM("%s: create file %s failed!!", __FUNCTION__, UART_USBSERIALPROC);
		return;
	}

	while(fgets(sBuf, sizeof(sBuf) - 1, fp))
	{
		if(sscanf(sBuf, "%d: %*[^\"]\"%*[^\"]\" vendor:%x product:%x %*[^ ] %*[^ ] path:%s", &nNum, &nVendor, &nProduct, sPath) == 4)
		{
			if(nDevNum == nNum)
				break;
			else if(nVendor == m_nVID && nProduct == m_nPID && !strcmp(sPath, sName))
				m_nChannel++;
		}
	}

	fclose(fp);
}


void CUEXAR::SetHalfDuplex()
{
	if(EXAR_VENDOR == m_nVID)
	{
		switch(m_nPID)
		{
			case EXAR_PRODUCT_VIZZINI1:
				SetHalfDuplex_xr21B1411(m_uartfd, m_nChannel);
				m_bHalfDuplex = true;
				break;
			case EXAR_PRODUCT_VIZZINI2:
			case EXAR_PRODUCT_VIZZINI3:
				SetHalfDuplex_xr21V141x(m_uartfd, m_nChannel);
				m_bHalfDuplex = true;
				break;
		}
	}
}


void CUEXAR::SetFullDuplex()
{
	m_bHalfDuplex = false;
	
	if(EXAR_VENDOR == m_nVID)
	{
		switch(m_nPID)
		{
			case EXAR_PRODUCT_VIZZINI1:
				SetFullDuplex_xr21B1411(m_uartfd, m_nChannel);
				break;
			case EXAR_PRODUCT_VIZZINI2:
			case EXAR_PRODUCT_VIZZINI3:
				SetFullDuplex_xr21V141x(m_uartfd, m_nChannel);
				break;
		}
	}
}


bool CUEXAR::SetRS232()
{
	SetFullDuplex();
	return m_cMode.SetRS232(m_nChannel);
}


bool CUEXAR::SetRS422()
{
	SetFullDuplex();
	return m_cMode.SetRS422(m_nChannel);
}


bool CUEXAR::SetRS485()
{
	SetHalfDuplex();
	return m_cMode.SetRS485(m_nChannel);
}


bool CUEXAR::SetMode(char cMode)
{
	switch(cMode)
	{
		case MODE_RS232:
			SetRS232();
			break;
		case MODE_RS422:
			SetRS422();
			break;
		case MODE_RS485:
			SetRS485();
			break;
		default:
			return false;
	}
	
	return true;
}
