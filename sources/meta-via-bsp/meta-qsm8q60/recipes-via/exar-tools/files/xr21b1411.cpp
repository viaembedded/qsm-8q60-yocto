#include <sys/ioctl.h>
#include "CVLog.h"
#include "xr21b1411.h"
#include "xr21b1411ioctl.h"


bool SetHalfDuplex_xr21B1411(int uartfd, int nChannel)
{
	int nArgs[3] = { nChannel, 0, 0 };
	int nReg = ((-1 == nChannel) ? 1 : 0);
	int nVal = ((-1 == nChannel) ? 2 : 1);

//	nArgs[nReg] = XR21B1411_REG_UART + UART_ENABLE;
//	nArgs[nVal] = 0;

//	if(ioctl(m_uartfd, XRUSBIOC_SET_REG, &nArgs) < 0)
//		LOGEM("uart ioctl error!!");

	nArgs[nReg] = XR21B1411_REG_UART + UART_FLOW;
	nArgs[nVal] = 0;

	if(ioctl(uartfd, XRUSBIOC_GET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

//	LOGI("uart flow: %02X", nArgs[nVal]);

	nArgs[nReg] = XR21B1411_REG_UART + UART_FLOW;
	nArgs[nVal] |= UART_FLOW_HALF_DUPLEX;

	if(ioctl(uartfd, XRUSBIOC_SET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

	nArgs[nReg] = XR21B1411_REG_UART + UART_GPIO_MODE;
	nArgs[nVal] = UART_GPIO_MODE_SEL_XCVR_EN_ACT | UART_GPIO_MODE_XCVR_EN_POL;

	if(ioctl(uartfd, XRUSBIOC_SET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

//	nArgs[nReg] = XR21B1411_REG_UART + UART_ENABLE;
//	nArgs[nVal] = UART_ENABLE_TX | UART_ENABLE_RX;;

//	if(ioctl(m_uartfd, XRUSBIOC_SET_REG, &nArgs) < 0)
//		LOGEM("uart ioctl error!!");

//	nArgs[nReg] = XR21B1411_REG_UART + UART_FLOW;
//	nArgs[nVal] = 0;

//	if(ioctl(uartfd, XRUSBIOC_GET_REG, &nArgs) < 0)
//		LOGEM("uart ioctl error!!");

//	LOGI("uart flow: %02X", nArgs[nVal]);

	return true;
}


bool SetFullDuplex_xr21B1411(int uartfd, int nChannel)
{
	int nArgs[3] = { nChannel, 0, 0 };
	int nReg = ((-1 == nChannel) ? 1 : 0);
	int nVal = ((-1 == nChannel) ? 2 : 1);

//	nArgs[nReg] = XR21B1411_REG_UART + UART_ENABLE;
//	nArgs[nVal] = 0;

//	if(ioctl(m_uartfd, XRUSBIOC_SET_REG, &nArgs) < 0)
//		LOGEM("uart ioctl error!!");

	nArgs[nReg] = XR21B1411_REG_UART + UART_FLOW;

	if(ioctl(uartfd, XRUSBIOC_GET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

//	LOGI("uart flow: %02X", nArgs[nVal]);

	nArgs[nReg] = XR21B1411_REG_UART + UART_FLOW;
	nArgs[nVal] &= (~UART_FLOW_HALF_DUPLEX);

	if(ioctl(uartfd, XRUSBIOC_SET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

	nArgs[nReg] = XR21B1411_REG_UART + UART_GPIO_MODE;

	if(ioctl(uartfd, XRUSBIOC_GET_REG, &nArgs) < 0)
	{
		LOGEM("uart ioctl error!!");
		return false;
	}

//	LOGI("GPIO mode: %02X", nArgs[nVal]);

	if((UART_GPIO_MODE_SEL_XCVR_EN_ACT | UART_GPIO_MODE_XCVR_EN_POL) == nArgs[nVal])
	{
		nArgs[nReg] = XR21B1411_REG_UART + UART_GPIO_MODE;
		nArgs[nVal] = 0;

		if(ioctl(uartfd, XRUSBIOC_SET_REG, &nArgs) < 0)
		{
			LOGEM("uart ioctl error!!");
			return false;
		}
	}

//	nArgs[nReg] = XR21B1411_REG_UART + UART_ENABLE;
//	nArgs[nVal] = UART_ENABLE_TX | UART_ENABLE_RX;;

//	if(ioctl(m_uartfd, XRUSBIOC_SET_REG, &nArgs) < 0)
//		LOGEM("uart ioctl error!!");

//	nArgs[nReg] = XR21B1411_REG_UART + UART_FLOW;

//	if(ioctl(uartfd, XRUSBIOC_GET_REG, &nArgs) < 0)
//		LOGEM("uart ioctl error!!");

//	LOGI("uart flow: %02X", nArgs[nVal]);

	return true;
}
