#ifndef __CMI2C_H__
#define __CMI2C_H__


/* ============================= CMI2C CLASS ========================== */
class CMI2C
{
public:

	CMI2C();
	~CMI2C();
	
	bool	Write(int nBusNo, int nSlave, char *sData, int nDataLen);
	
	bool	Write(int nBusNo, int nSlave, char cOffset, char cData);

	bool	Read(int nBusNo, int nSlave, char *sOffset, int nOffsetLen, char *sBuf, int nBufLen);
	
	bool	Read(int nBusNo, int nSlave, char cOffset, char *sData);
	
	bool	Read(int nBusNo, int nSlave, char cOffset, short *sData);

protected:

private:

	bool	CheckSlave(int nBusNo, int nSlave);
	
	bool	IOCTL(int nBusNo, struct i2c_rdwr_ioctl_data *pmsg_data);

};

#endif	// __CMI2C_H__
