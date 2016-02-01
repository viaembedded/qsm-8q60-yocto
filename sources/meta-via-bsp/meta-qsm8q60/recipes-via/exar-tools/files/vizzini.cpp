#include <sys/ioctl.h>
#include "CVLog.h"
#include "vizzini.h"
#include "vzioctl.h"


bool SetHalfDuplex_xr21V141x(int uartfd, int nChannel)
{
	int nArgs[3] = {0};

	nArgs[0] = UART_0_REG_BLOCK + nChannel;
	nArgs[1] = UART_FLOW;

	if(ioctl(uartfd, VZIOC_GET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

//	LOGI("uart flow: %02X", nArgs[2]);

	nArgs[0] = UART_0_REG_BLOCK + nChannel;
	nArgs[1] = UART_FLOW;
	nArgs[2] |= UART_FLOW_HALF_DUPLEX;

	if(ioctl(uartfd, VZIOC_SET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

	nArgs[0] = UART_0_REG_BLOCK + nChannel;
	nArgs[1] = UART_GPIO_MODE;
	nArgs[2] = UART_GPIO_MODE_SEL_XCVR_EN_ACT | UART_GPIO_MODE_XCVR_EN_POL;

	if(ioctl(uartfd, VZIOC_SET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

//	nArgs[0] = UART_0_REG_BLOCK + nChannel;
//	nArgs[1] = UART_FLOW;

//	if(ioctl(uartfd, VZIOC_GET_REG, &nArgs) < 0)
//	{
//		LOGEM("uart ioctl error!!");
//		return false;
//	}

//	LOGI("uart flow: %02X", nArgs[2]);

	return true;
}


bool SetFullDuplex_xr21V141x(int uartfd, int nChannel)
{
	int nArgs[3] = {0};

	nArgs[0] = UART_0_REG_BLOCK + nChannel;
	nArgs[1] = UART_FLOW;

	if(ioctl(uartfd, VZIOC_GET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

//	LOGI("uart flow: %02X", nArgs[2]);

	nArgs[0] = UART_0_REG_BLOCK + nChannel;
	nArgs[1] = UART_FLOW;
	nArgs[2] &= (~UART_FLOW_HALF_DUPLEX);

	if(ioctl(uartfd, VZIOC_SET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

	nArgs[0] = UART_0_REG_BLOCK + nChannel;
	nArgs[1] = UART_GPIO_MODE;

	if(ioctl(uartfd, VZIOC_GET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

//	LOGI("GPIO mode: %02X", nArgs[2]);

	if((UART_GPIO_MODE_SEL_XCVR_EN_ACT | UART_GPIO_MODE_XCVR_EN_POL) == nArgs[2])
	{
		nArgs[0] = UART_0_REG_BLOCK + nChannel;
		nArgs[1] = UART_GPIO_MODE;
		nArgs[2] = 0;

		if(ioctl(uartfd, VZIOC_SET_REG, &nArgs) < 0)
		{
			LOGEM("uart ioctl error!!");
			return false;
		}
	}

//	nArgs[0] = UART_0_REG_BLOCK + nChannel;
//	nArgs[1] = UART_FLOW;

//	if(ioctl(uartfd, VZIOC_GET_REG, &nArgs) < 0)
//	{
//		LOGEM("uart ioctl error!!");
//		return false;
//	}

//	LOGI("uart flow: %02X", nArgs[2]);

	return true;
}
