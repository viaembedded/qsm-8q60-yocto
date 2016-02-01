#ifndef __CF75113_H__
#define __CF75113_H__

#include "CMI2C.h"


/* ============================= CF75113 CLASS ========================== */
class CF75113 : public CMI2C
{
public:

	CF75113();
	~CF75113();
	
	bool	SetRS232(int nChannel);
	
	bool	SetRS422(int nChannel);
	
	bool	SetRS485(int nChannel);

protected:

private:

	bool	CheckChipID();
	
	void	SetValue(char cOffset, char cMask, char cValue);

};

#endif	// __CF75113_H__
