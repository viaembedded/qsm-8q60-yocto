#ifndef __CMUART_H__
#define __CMUART_H__

#include "common.h"

#define	UART_DATABITS7			7
#define	UART_DATABITS8			8

#define	UART_STOPBITS1			1
#define	UART_STOPBITS2			2

#define	UART_PARITY_NONE		0
#define	UART_PARITY_ODD			1
#define	UART_PARITY_EVEN		2

#define	UART_FLOWCTL_NONE		0
#define	UART_FLOWCTL_CTSRTS		1

/* ============================= UART CONFIGURATION ========================== */
typedef struct tagUARTConfig	{
	int		nBaudRate;		// baud rate
	BYTE	byDataBits;		// data bits. 7: 7-bit data bits; 8: 8-bit data bits
	BYTE	byStopBits;		// stop bits. 1: 1-stop bits; 2: 2-stop bits
	BYTE	byParity;		// parity. 0: none; 1: odd; 2: even
	BYTE	byFlowCtrl;		// flow control. 0: none; 1: cts/rts
} UART_CONFIG, *PUART_CONFIG;


/* ============================= CMUART CLASS ========================== */
class CMUART
{
public:

	CMUART();
	~CMUART();

	bool	Open(char *sDevName);

	void	Close();

	bool	SetConfig(int nBaudRate, int nDataBits=8, int nStopBits=1, int nParity=0, int nFlowCtrl=0);

	bool	SetConfig(UART_CONFIG stConfig);

	bool	SetTimeout(int nTimeout = 200);

	bool	GetConfig(UART_CONFIG &stConfig);

	int		Write(char *sData, int nDataLen);

	int		Read(char *sBuf, int nBufLen);

	int		GetFD() { return m_uartfd; };

protected:

	int		m_uartfd;

private:

	DWORD	GetBaudRateCode(int nBaudRate);

	int		GetBaudRate(DWORD dwCode);

};

#endif	// __CMUART_H__
