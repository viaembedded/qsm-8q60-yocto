#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "CVLog.h"
#include "CMI2C.h"

#define I2C_DRIVER_BUS		"/sys/bus/i2c/devices/%d-%04x"
#define	I2C_PATH_FORMAT		"/dev/i2c-%d"


/* ============================= CMI2C CLASS ========================== */
CMI2C::CMI2C()
{
}


CMI2C::~CMI2C()
{
}


bool CMI2C::CheckSlave(int nBusNo, int nSlave)
{
	char sBus[32] = {0};

	sprintf(sBus, I2C_DRIVER_BUS, nBusNo, nSlave);

	if(access(sBus, F_OK) == 0)
	{
//		LOGW("i2c-%d slave address 0x%02X is already in use by a driver!!", nBusNo, nSlave);
		return false;
	}

	if(nSlave > 0x7F)
	{
//		LOGW("i2c-%d slave address 0x%02X is out of range!", nBusNo, nSlave);
		return false;
	}

//	if(nSlave < 0x03 || nSlave > 0x77)
//	{
//		LOGW("i2c-%d slave address 0x%02X is out of range (0x03 ~ 0x77)!", nBusNo, nSlave);
//		return false;
//	}

	return true;
}


bool CMI2C::IOCTL(int nBusNo, struct i2c_rdwr_ioctl_data *pmsg_data)
{
	char sPath[20] = {0};
	int i2cfd = -1;

	sprintf(sPath, I2C_PATH_FORMAT, nBusNo);
	
	if((i2cfd = open(sPath, O_RDWR)) < 0)
	{
//		LOGEM("%s: open device %s error!!", __FUNCTION__, sPath);
		return false;
	}
	
	if(ioctl(i2cfd, I2C_RDWR, pmsg_data) < 0)
	{
//		LOGEM("i2c ioctl error!!");
		close(i2cfd);
		return false;
	}
	
	close(i2cfd);

	return true;
}


bool CMI2C::Write(int nBusNo, int nSlave, char *sData, int nDataLen)
{
	struct i2c_rdwr_ioctl_data msg_data = {0};
	struct i2c_msg i2cmsg = {0};

	msg_data.msgs = &i2cmsg;
	msg_data.nmsgs = 1;

	if(!CheckSlave(nBusNo, nSlave))
		return false;

	msg_data.nmsgs = 1;

	i2cmsg.addr = nSlave;
	i2cmsg.flags = 0;
	i2cmsg.len = nDataLen;
	i2cmsg.buf = (__u8*)sData;

	return IOCTL(nBusNo, &msg_data);
}


bool CMI2C::Write(int nBusNo, int nSlave, char cOffset, char cData)
{
	struct i2c_rdwr_ioctl_data msg_data = {0};
	struct i2c_msg i2cmsg = {0};
	char sBuf[2] = { cOffset, cData };

	msg_data.msgs = &i2cmsg;
	msg_data.nmsgs = 1;

	if(!CheckSlave(nBusNo, nSlave))
		return false;

	msg_data.nmsgs = 1;

	i2cmsg.addr = nSlave;
	i2cmsg.flags = 0;
	i2cmsg.len = 2;
	i2cmsg.buf = (__u8*)sBuf;

	return IOCTL(nBusNo, &msg_data);
}


bool CMI2C::Read(int nBusNo, int nSlave, char *sOffset, int nOffsetLen, char *sBuf, int nBufLen)
{
	struct i2c_rdwr_ioctl_data msg_data = {0};
	struct i2c_msg i2cmsg[2] = {0};

	msg_data.msgs = i2cmsg;

	if(!CheckSlave(nBusNo, nSlave))
		return false;

	if(0 == nOffsetLen)
	{
		msg_data.nmsgs = 1;

		i2cmsg[0].addr = nSlave;
		i2cmsg[0].flags = I2C_M_RD;
		i2cmsg[0].len = nBufLen;
		i2cmsg[0].buf = (__u8*)sBuf;
	}
	else
	{
		msg_data.nmsgs = 2;

		i2cmsg[0].addr = nSlave;
		i2cmsg[0].flags = 0;
		i2cmsg[0].len = nOffsetLen;
		i2cmsg[0].buf = (__u8*)sOffset;

		i2cmsg[1].addr = nSlave;
		i2cmsg[1].flags = I2C_M_RD;
		i2cmsg[1].len = nBufLen;
		i2cmsg[1].buf = (__u8*)sBuf;
	}

	return IOCTL(nBusNo, &msg_data);
}


bool CMI2C::Read(int nBusNo, int nSlave, char cOffset, char *sData)
{
	struct i2c_rdwr_ioctl_data msg_data = {0};
	struct i2c_msg i2cmsg[2] = {0};

	msg_data.msgs = i2cmsg;

	if(!CheckSlave(nBusNo, nSlave))
		return false;

	msg_data.nmsgs = 2;

	i2cmsg[0].addr = nSlave;
	i2cmsg[0].flags = 0;
	i2cmsg[0].len = 1;
	i2cmsg[0].buf = (__u8*)&cOffset;

	i2cmsg[1].addr = nSlave;
	i2cmsg[1].flags = I2C_M_RD;
	i2cmsg[1].len = 1;
	i2cmsg[1].buf = (__u8*)sData;

	return IOCTL(nBusNo, &msg_data);
}


bool CMI2C::Read(int nBusNo, int nSlave, char cOffset, short *sData)
{
	struct i2c_rdwr_ioctl_data msg_data = {0};
	struct i2c_msg i2cmsg[2] = {0};

	msg_data.msgs = i2cmsg;

	if(!CheckSlave(nBusNo, nSlave))
		return false;

	msg_data.nmsgs = 2;

	i2cmsg[0].addr = nSlave;
	i2cmsg[0].flags = 0;
	i2cmsg[0].len = 1;
	i2cmsg[0].buf = (__u8*)&cOffset;

	i2cmsg[1].addr = nSlave;
	i2cmsg[1].flags = I2C_M_RD;
	i2cmsg[1].len = 2;
	i2cmsg[1].buf = (__u8*)sData;

	return IOCTL(nBusNo, &msg_data);
}
