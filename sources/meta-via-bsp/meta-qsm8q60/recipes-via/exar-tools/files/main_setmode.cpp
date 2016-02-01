#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include "CVLog.h"
#include "CUEXAR.h"

using namespace std;


int main(int argc, char *argv[])
{
	CUEXAR *pUart = NULL;

	if((pUart = new CUEXAR()) == NULL)
	{
		LOGI("new CUEXAR failed!");
		exit(EXIT_FAILURE);
	}

	printf("\033[1m UART setting tool for RS232 / RS422 / RS485 %s --- %s\033[0m\n", BUILD_VERSION, __DATE__);
	printf("\033[1m Note:\033[0m Please use this tool after opening UART device and setting the baud rate.\n\n");
	printf(SUPPORT_LIST);

	if(argc >= 3 && pUart->Open(argv[1]) && pUart->SetMode(argv[2][0]))
	{
		printf(" Set %s to %s mode.\n\n", argv[1], ((MODE_RS232 == argv[2][0]) ? "RS232" : ((MODE_RS422 == argv[2][0]) ? "RS422" : "RS485")));

		delete pUart;
		exit(EXIT_SUCCESS);
	}

	delete pUart;

	printf(" On the command you need to include two parameters in the following order, they are:\n");
	printf("   1.  The device name				Ex: /dev/ttyUSB0, /dev/ttyXRUSB0, ......\n");
	printf("   2.  Mode: 0-RS232, 1-RS422, 2-RS485		Ex: 0, 1, 2\n");
	printf(" Example command line:  \033[1m%s /dev/ttyXRUSB0 2\033[0m\n", argv[0]);
	printf(" /dev/ttyXRUSB0 device will be set to RS485 mode.\n\n");
	   
	exit(EXIT_FAILURE);
	
	return 1;
}

