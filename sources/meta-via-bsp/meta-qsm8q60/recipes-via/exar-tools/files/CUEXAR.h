#ifndef __CUEXAR_H__
#define __CUEXAR_H__

#include "CMUART.h"
#include "CF75113.h"


/* ============================= CUEXAR CLASS ========================== */
class CUEXAR : public CMUART
{
public:

	CUEXAR();
	~CUEXAR();

	void	Init();

	void	PrintLog(char *sStr, bool bRed = false);

	void	PrintLogTime(char *sStr, bool bRed = false);
	
	bool	Open(char *sDevName);
	
	bool	SetConfig(int nBaudRate, int nDataBits=8, int nStopBits=1, int nParity=0, int nFlowCtrl=0);

	bool	SetConfig(UART_CONFIG stConfig);

	bool	SetTimeout(int nTimeout = 200);
	
	bool	SetMode(char cMode);

	bool	SetRS232();
	
	bool	SetRS422();
	
	bool	SetRS485();
	
protected:

private:

	bool	GetIDfromSysClass(char *sDevName);

	bool	GetIDfromUSBSerial(char *sDevName);
	
	void	GetChannel(int nDevNum, char *sName);
	
	void	SetHalfDuplex();
	
	void	SetFullDuplex();
	
	CF75113	m_cMode;
	
	char	m_sLog[32];
	
	int		m_nChannel;
	
	int		m_nPID;
	
	int		m_nVID;
	
	bool	m_bHalfDuplex;

};

#endif	// __CUEXAR_H__
