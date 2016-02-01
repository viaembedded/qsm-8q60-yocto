DESCRIPTION = "Exar USB Serial Test Tools"
LICENSE = "CLOSED"

SRC_URI = "file://CVLog.cpp"
SRC_URI += "file://xr21b1411.cpp"
SRC_URI += "file://vizzini.cpp"
SRC_URI += "file://CMUART.cpp"
SRC_URI += "file://CMI2C.cpp"
SRC_URI += "file://CF75113.cpp"
SRC_URI += "file://CUEXAR.cpp"
SRC_URI += "file://main_test.cpp"
SRC_URI += "file://main_echo.cpp"
SRC_URI += "file://main_setmode.cpp"
SRC_URI += "file://CF75113.h"
SRC_URI += "file://CMI2C.h"
SRC_URI += "file://CMUART.h"
SRC_URI += "file://common.h"
SRC_URI += "file://CUEXAR.h"
SRC_URI += "file://CVLog.h"
SRC_URI += "file://vizzini.h"
SRC_URI += "file://vzioctl.h"
SRC_URI += "file://xr21b1411.h"
SRC_URI += "file://xr21b1411ioctl.h"

S = "${WORKDIR}"

do_compile() {
	${CXX} CVLog.cpp xr21b1411.cpp vizzini.cpp CMUART.cpp CMI2C.cpp CF75113.cpp CUEXAR.cpp main_test.cpp -o exar_test
	${CXX} CVLog.cpp xr21b1411.cpp vizzini.cpp CMUART.cpp CMI2C.cpp CF75113.cpp CUEXAR.cpp main_echo.cpp -o exar_echo
	${CXX} CVLog.cpp xr21b1411.cpp vizzini.cpp CMUART.cpp CMI2C.cpp CF75113.cpp CUEXAR.cpp main_setmode.cpp -o exar_setmode
}

do_install () {
	install -d ${D}/usr/local/bin
	install -m 755 exar_echo ${D}/usr/local/bin
	install -m 755 exar_setmode ${D}/usr/local/bin
	install -m 755 exar_test ${D}/usr/local/bin
}

FILES_${PN} += "/usr/local/bin"
FILES_${PN}-dbg += "/usr/local/bin/.debug"
