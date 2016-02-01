#ifndef __COMMON_H__
#define __COMMON_H__

#define	LOG_TAG	"EXAR-UART"			// Android Log Tag
#define	BUILD_VERSION				"v0.0.3"

#define	SUPPORT_LIST				" <only for VAB-600-RS485, VAB-600-E (RS485), QSM-8Q60-DB2 (RS232/RS422/RS485).>\n\n"

#define	MODE_RS232					'0'
#define	MODE_RS422					'1'
#define	MODE_RS485					'2'

#define	BUFFER_SIZE					1024
#define	DEFAULT_BAUDRATE			115200
#define DEFAULT_MAXLENGTH			20

#define EXAR_VENDOR					0x04E2
#define EXAR_PRODUCT_VIZZINI1		0x1411
#define EXAR_PRODUCT_VIZZINI2		0x1412
#define EXAR_PRODUCT_VIZZINI3		0x1414

typedef unsigned char BYTE;
typedef long DWORD;

#endif	// __COMMON_H__

