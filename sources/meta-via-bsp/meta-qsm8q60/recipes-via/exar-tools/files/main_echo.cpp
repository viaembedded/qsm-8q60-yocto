#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CVLog.h"
#include "CUEXAR.h"

using namespace std;


int main(int argc, char *argv[])
{
	CUEXAR *pUart = NULL;
	char sRead[BUFFER_SIZE] = {0}, sLog[128] = {0};
	int nBaudRate = DEFAULT_BAUDRATE, nIdx = 0, nReadNum = 0, i = 0;

	if((pUart = new CUEXAR()) == NULL)
	{
		LOGI("new CUEXAR failed!");
		exit(EXIT_FAILURE);
	}

	pUart->Init();
	sprintf(sLog, "UART burn-in echo tool %s --- %s", BUILD_VERSION, __DATE__);
	pUart->PrintLog(sLog);
	printf(SUPPORT_LIST);

	if(argc >= 3 && pUart->Open(argv[1]))
	{
		if(argc >= 4)
			nBaudRate = strtol(argv[3], 0, 10);

		if(pUart->SetConfig(nBaudRate) && pUart->SetMode(argv[2][0]))
		{
			sprintf(sLog, "Open %s and set baud rate to %d with %s mode.", argv[1], nBaudRate, ((MODE_RS232 == argv[2][0]) ? "RS232" : ((MODE_RS422 == argv[2][0]) ? "RS422" : "RS485")));
			pUart->PrintLog(sLog);

			while((nReadNum = pUart->Read(sRead + nIdx, sizeof(sRead) - nIdx)) > 0)
			{
//				printf("Read (%d)\n", nReadNum);
//				for(i = nIdx; i < nIdx + nReadNum; i++)
//					printf(" %02X", (unsigned char)sRead[i]);
//				printf("\n");

				nIdx += nReadNum;

				while(nIdx > 0)
				{
					for(i = 0; i < nIdx; i++)
						if(0xFE == (unsigned char)sRead[0])
							break;

					if(i == nIdx)
					{
						nIdx = 0;
						break;
					}

					if(i > 0 && i < nIdx)
					{
						memcpy(sRead, sRead + i, nIdx - i);
						nIdx -= i;
						continue;
					}

					if(nIdx < 2 || 0xFF != (unsigned char)sRead[nIdx - 1] && nIdx < sizeof(sRead))
						break;

					if(0xFF == (unsigned char)sRead[nIdx - 1])
					{
						LOGI("echo (%d):", nIdx);

						for(i = 0; i < nIdx; i++)
							printf(" %02X", (unsigned char)sRead[i]);
						printf("\n");

						pUart->Write(sRead, nIdx);
						nIdx = 0;
					}
					else if(nIdx >= sizeof(sRead))
						nIdx = 0;
				}
			}
		}
	}

	delete pUart;

	printf(" On the command you need to include the parameters in the following order, they are:\n");
	printf("   1.  The device name				Ex: /dev/ttyUSB0, /dev/ttyXRUSB0, etc.\n");
	printf("   2.  Mode: 0-RS232, 1-RS422, 2-RS485		Ex: 0, 1, 2\n");
	printf("   3.  Baud rate				Ex: %d (default)\n", DEFAULT_BAUDRATE);
	printf(" Example command line:  \033[1m%s /dev/ttyXRUSB0 2 %d\033[0m\n", argv[0], DEFAULT_BAUDRATE);
	printf(" Open /dev/ttyXRUSB0 and set baud rate to %d with RS485 mode to read and echo the data.\n\n", DEFAULT_BAUDRATE, DEFAULT_MAXLENGTH);

	exit(EXIT_FAILURE);

	return 1;
}

