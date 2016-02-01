#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "CVLog.h"
#include "CMUART.h"


const unsigned int g_uart_baudrate_table[][2] = {
	B0, 0, B50, 50, B75, 75, B110, 110, B134, 134, B150, 150, B200, 200, B300, 300, B600, 600,
	B1200, 1200, B1800, 1800, B2400, 2400, B4800, 4800, B9600, 9600, B19200, 19200, B38400, 38400, B57600, 57600,
	B115200, 115200, B230400, 230400, B460800, 460800, B500000, 500000, B576000, 576000, B921600, 921600, B1000000, 1000000,
	B1152000, 1152000, B1500000, 1500000, B2000000, 2000000, B2500000, 2500000, B3000000, 3000000, B3500000, 3500000,
	B4000000, 4000000 };

const int g_uart_baudrate_amount = sizeof(g_uart_baudrate_table) / (sizeof(int) * 2);


/* ============================= CMUART CLASS ========================== */
CMUART::CMUART()
: m_uartfd(-1)
{
}


CMUART::~CMUART()
{
	Close();
}


void CMUART::Close()
{
	if(-1 != m_uartfd)
	{
		tcflush(m_uartfd, TCIOFLUSH);
		close(m_uartfd);
		m_uartfd = -1;
	}
}


bool CMUART::Open(char *sDevName)
{
	if(!sDevName)
		return false;

	Close();

	if((m_uartfd = open(sDevName, O_RDWR | O_NOCTTY)) > 0)
	{
		tcflush(m_uartfd, TCIOFLUSH);
		return true;
	}

	LOGEM("open device %s error!!", sDevName);
	return false;
}


DWORD CMUART::GetBaudRateCode(int nBaudRate)
{
	for(int i = 0; i < g_uart_baudrate_amount; i++)
		if(g_uart_baudrate_table[i][1] == nBaudRate)
			return g_uart_baudrate_table[i][0];

	return -1;
}


int CMUART::GetBaudRate(DWORD dwCode)
{
	for(int i = 0; i < g_uart_baudrate_amount; i++)
		if(g_uart_baudrate_table[i][0] == dwCode)
			return g_uart_baudrate_table[i][1];

	return -1;
}


bool CMUART::SetConfig(int nBaudRate, int nDataBits, int nStopBits, int nParity, int nFlowCtrl)
{
	if(-1 == m_uartfd)
		return false;

	DWORD dwCode = GetBaudRateCode(nBaudRate);
	struct termios newtio = {0};

	if(dwCode == -1)
	{
		LOGE("uart baud rate %d is not supported!!", nBaudRate);
		return false;
	}

	if(nDataBits < 7 || nDataBits > 8 || nStopBits < 1 || nStopBits > 2 || nParity < 0 || nParity > 2 || nFlowCtrl < 0 || nFlowCtrl > 1)
	{
		LOGE("uart setconfig error (%d %d %d %d)!!", nDataBits, nStopBits, nParity, nFlowCtrl);
		return false;
	}

	newtio.c_cflag = CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;

	// data bits. 7: 7-bit databits; 8: 8-data bits
	if(nDataBits == UART_DATABITS7)
		newtio.c_cflag |= CS7;
	else if(nDataBits == UART_DATABITS8)
		newtio.c_cflag |= CS8;

	// stop bits. 1: 1-stop bits; 2: 2-stop bits
	if(nStopBits == UART_STOPBITS2)
		newtio.c_cflag |= CSTOPB;

	// parity. 0: none; 1: odd; 2: even
	if(nParity == UART_PARITY_ODD)
	{
		newtio.c_cflag |= (PARENB | PARODD);
		newtio.c_iflag |= INPCK;
	}
	else if(nParity == UART_PARITY_EVEN)
	{
		newtio.c_cflag |= PARENB;
		newtio.c_iflag |= INPCK;
	}

	// flow control. 0: none; 1: cts/rts
	if(nFlowCtrl == UART_FLOWCTL_CTSRTS)
		newtio.c_cflag |= CRTSCTS;

	// for read block
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;

	// baud rate
	cfsetispeed(&newtio, dwCode);
	cfsetospeed(&newtio, dwCode);

	if(tcsetattr(m_uartfd, TCSANOW, &newtio) == 0)
	{
		tcflush(m_uartfd, TCIOFLUSH);
		return true;
	}

	LOGEM("uart tcsetattr error!!");
	return false;
}


bool CMUART::SetConfig(UART_CONFIG stConfig)
{
	return SetConfig(stConfig.nBaudRate, stConfig.byDataBits, stConfig.byStopBits, stConfig.byParity, stConfig.byFlowCtrl);
}


bool CMUART::GetConfig(UART_CONFIG &stConfig)
{
	if(-1 == m_uartfd)
		return false;

	struct termios oldtio = {0};

	if(tcgetattr(m_uartfd, &oldtio) == -1)
	{
		LOGEM("uart tcgetattr error!!");
		return false;
	}

	// data bits. 7: 7-bit databits; 8: 8-data bits
	if((oldtio.c_cflag & CS8) == CS8)
		stConfig.byDataBits = UART_DATABITS8;
	else if(oldtio.c_cflag & CS7)
		stConfig.byDataBits = UART_DATABITS7;

	// stop bits. 1: 1-stop bits; 2: 2-stop bits
	stConfig.byStopBits = ((oldtio.c_cflag & CSTOPB) ? UART_STOPBITS2 : UART_STOPBITS1);

	// parity. 0: none; 1: odd; 2: even
	if((oldtio.c_cflag & (PARENB | PARODD)) == (PARENB | PARODD))
		stConfig.byParity = UART_PARITY_ODD;
	else if(oldtio.c_cflag & PARENB)
		stConfig.byParity = UART_PARITY_EVEN;
	else
		stConfig.byParity = UART_PARITY_NONE;

	// flow control. 0: none; 1: cts/rts
	stConfig.byFlowCtrl = ((oldtio.c_cflag & CRTSCTS) ? UART_FLOWCTL_CTSRTS : UART_FLOWCTL_NONE);

	// baud rate
	if((stConfig.nBaudRate = GetBaudRate(cfgetispeed(&oldtio))) == -1)
		return false;

	return true;
}


bool CMUART::SetTimeout(int nTimeout)
{
	if(-1 == m_uartfd)
		return false;

	struct termios oldtio = {0};

	if(tcgetattr(m_uartfd, &oldtio) == -1)
	{
		LOGEM("uart tcgetattr error!!");
		return false;
	}

	// for read block
	oldtio.c_cc[VMIN] = 0;
	oldtio.c_cc[VTIME] = nTimeout;

	if(tcsetattr(m_uartfd, TCSANOW, &oldtio) == 0)
		return true;

	LOGEM("uart tcsetattr error!!");
	return false;
}


int CMUART::Write(char *sData, int nDataLen)
{
	int nWriteLen = -1;

	if(-1 != m_uartfd && (nWriteLen = write(m_uartfd, sData, nDataLen)) == -1)
	{
		LOGEM("uart write data error!!");
		tcflush(m_uartfd, TCOFLUSH);
	}

	return nWriteLen;
}


int CMUART::Read(char *sBuf, int nBufLen)
{
	int nReadNum = -1;

	if(-1 != m_uartfd && (nReadNum = read(m_uartfd, sBuf, nBufLen)) == -1)
		LOGEM("uart read data error!!");

	return nReadNum;
}
