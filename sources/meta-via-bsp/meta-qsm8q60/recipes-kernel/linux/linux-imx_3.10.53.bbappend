# Copyright (C) 2014, 2015 VIA Technologies, Inc.

SUMMARY = "Linux Kernel provided by VIA Technologies, Inc."
DESCRIPTION = "Linux Kernel provided by VIA Technologies, Inc."

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI += "file://defconfig \
			file://imx6qdl-qsm8q60.dtsi \
			file://imx6dl-qsm8q60.dts \
			file://imx6q-qsm8q60.dts \
			file://qsm8q60-linux-imx.patch \
			"

DTS_DIR = "${S}/arch/arm/boot/dts/"
CONF_DIR = "${S}/arch/arm/configs"

do_configure_prepend() {
    mv ${CONF_DIR}/imx_v7_defconfig ${CONF_DIR}/imx_v7_defconfig.tmp
    cp ${WORKDIR}/defconfig ${CONF_DIR}/imx_v7_defconfig
    cp ${WORKDIR}/imx6qdl-qsm8q60.dtsi ${DTS_DIR}
    cp ${WORKDIR}/imx6dl-qsm8q60.dts ${DTS_DIR}
    cp ${WORKDIR}/imx6q-qsm8q60.dts ${DTS_DIR}
}

do_configure_append() {
    mv ${CONF_DIR}/imx_v7_defconfig.tmp ${CONF_DIR}/imx_v7_defconfig
}

COMPATIBLE_MACHINE = "(mx6)"
