# Copyright (C) 2012 O.S. Systems Software LTDA.

DESCRIPTION = "Extra files for via-image-gui"
LICENSE = "LGPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=fff32fc3e1a5880f3070be98a4e417a5"

SRC_URI = "file://rc.local.etc \
           file://rc.local.init \
           file://LICENSE \
           "

S = "${WORKDIR}"

inherit update-rc.d

INITSCRIPT_NAME = "rc.local"
INITSCRIPT_PARAMS = "start 99 2 3 4 5 ."

PACKAGE_EXCLUDE = "fsl-rc-local"

do_install () {
    install -d ${D}/${sysconfdir}/init.d
    install -m 755 ${S}/rc.local.etc ${D}/${sysconfdir}/rc.local
    install -m 755 ${S}/rc.local.init ${D}/${sysconfdir}/init.d/rc.local
}
