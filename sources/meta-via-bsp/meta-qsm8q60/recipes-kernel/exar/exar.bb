SUMMARY = "Exar USB Serial Driver"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile"
SRC_URI += "file://xr_usb_serial_common.h"
SRC_URI += "file://xr_usb_serial_ioctl.h"
SRC_URI += "file://xr_usb_serial_hal.c"
SRC_URI += "file://xr_usb_serial_common.c"

S = "${WORKDIR}"
