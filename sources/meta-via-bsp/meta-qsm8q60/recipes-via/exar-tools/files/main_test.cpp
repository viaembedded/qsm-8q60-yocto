#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "CVLog.h"
#include "CUEXAR.h"

using namespace std;


int main(int argc, char *argv[])
{
	CUEXAR *pUart = NULL;
	char sRead[BUFFER_SIZE] = {0}, sWrite[BUFFER_SIZE] = {0}, sLog[128] = {0};
	int nBaudRate = DEFAULT_BAUDRATE, i = 0, nIdx = 0, nLen = 0, nReadNum = 0, nTimes = 0, nCount = 0, nSucCount = 0, nErrorCount = 0, nMaxLen = DEFAULT_MAXLENGTH;

	if((pUart = new CUEXAR()) == NULL)
	{
		LOGI("new CUEXAR failed!");
		exit(EXIT_FAILURE);
	}

	pUart->Init();
	sprintf(sLog, "UART burn-in test tool %s --- %s", BUILD_VERSION, __DATE__);
	pUart->PrintLog(sLog);
	printf(SUPPORT_LIST);

	if(argc >= 3 && pUart->Open(argv[1]))
	{
		if(argc >= 4)
			nBaudRate = strtol(argv[3], 0, 10);

		if(argc > 4)
		{
			nMaxLen = strtol(argv[4], 0, 10);
			
			if (nMaxLen > sizeof(sWrite))
				nMaxLen = sizeof(sWrite);
			else if(nMaxLen < 2)
				nMaxLen = 2;
		}

		if(pUart->SetConfig(nBaudRate) && pUart->SetMode(argv[2][0]))
		{
			pUart->SetTimeout();

			sprintf(sLog, "Open %s and set baud rate to %d with %s mode, the maximum data length is %d", argv[1], nBaudRate, ((MODE_RS232 == argv[2][0]) ? "RS232" : ((MODE_RS422 == argv[2][0]) ? "RS422" : "RS485")), nMaxLen);
			pUart->PrintLog(sLog);

			srand(time(NULL));

			while(true)
			{
				nLen = (rand() % (nMaxLen - 1)) + 2;

				for(i = 1; i < (nLen - 1); i++)
					sWrite[i] = rand() % 0xFE;

				sWrite[0] = 0xFE;
				sWrite[nLen - 1] = 0xFF;

				pUart->Write(sWrite, nLen);

//				printf("Write (%d)\n", nLen);
//				for(int j = 0; j < nLen; j++)
//					printf(" %02X", (unsigned char)sWrite[j]);
//				printf("\n");

				nIdx = 0;

				while(nIdx < nLen && (nReadNum = pUart->Read(sRead + nIdx, sizeof(sRead) - nIdx)) > 0)
					nIdx += nReadNum;

				for(i = 0; i < nLen; i++)
					if(sWrite[i] != sRead[i])
						break;

//				printf("Read (%d)\n", nLen);
//				for(int j = 0; j < nLen; j++)
//					printf(" %02X", (unsigned char)sRead[j]);
//				printf("\n");

				nTimes++;
				nCount += nLen;

				if(i != nLen)
				{
					nErrorCount++;
					sprintf(sLog, "write & read compare failed!! Times: %d Success: %d Failed: %d (%d %d %d)", nTimes, nSucCount, nErrorCount, nCount, nLen, nIdx);
					pUart->PrintLogTime(sLog, true);
				}
				else
				{
					nSucCount++;
					sprintf(sLog, "write & read compare successfully!! Times: %d Success: %d Failed: %d (%d %d)", nTimes, nSucCount, nErrorCount, nCount, nLen);
					pUart->PrintLogTime(sLog);
				}
			}
		}
	}

	delete pUart;

	printf(" On the command you need to include the parameters in the following order, they are:\n");
	printf("   1.  The device name				Ex: /dev/ttyUSB0, /dev/ttyXRUSB0, etc.\n");
	printf("   2.  Mode: 0-RS232, 1-RS422, 2-RS485		Ex: 0, 1, 2\n");
	printf("   3.  Baud rate				Ex: %d (default)\n", DEFAULT_BAUDRATE);
	printf("   4.  Maximum data length (2 ~ %d)		Ex: %d (default)\n", sizeof(sWrite), DEFAULT_MAXLENGTH);
	printf(" Example command line:  \033[1m%s /dev/ttyXRUSB0 2 %d %d\033[0m\n", argv[0], DEFAULT_BAUDRATE, DEFAULT_MAXLENGTH);
	printf(" Open /dev/ttyXRUSB0 and set baud rate to %d with RS485 mode to write/read and compare the data.\n\n", DEFAULT_BAUDRATE, DEFAULT_MAXLENGTH);
	   
	exit(EXIT_FAILURE);
	
	return 1;
}
