# Copyright (C) 2014 O.S. Systems Software LTDA.
# Copyright (C) 2014 Freescale Semiconductor

SUMMARY = "Produces a Manufacturing Tool compatible Linux Kernel"
DESCRIPTION = "Linux Kernel provided and supported by Freescale that produces a \
Manufacturing Tool compatible Linux Kernel to be used in updater environment"

require recipes-kernel/linux/linux-imx.inc
require recipes-kernel/linux/linux-dtb.inc
require recipes-kernel/linux/linux-mfgtool.inc

DEPENDS += "lzop-native bc-native"

SRCBRANCH = "imx_3.10.53_1.1.0_ga"
LOCALVERSION = "-1.1.0_ga"
SRCREV = "496fbe0b831b77d0ea0a2c7bbd1d58820b3a01f7"
KERNEL_SRC ?= "git://git.freescale.com/imx/linux-2.6-imx.git;protocol=git" 
SRC_URI = "${KERNEL_SRC};branch=${SRCBRANCH}"

DEFAULT_PREFERENCE = "1"

do_configure_prepend() {
    cp ${S}/arch/arm/configs/imx_v7_mfg_defconfig ${S}/.config
    cp ${S}/arch/arm/configs/imx_v7_mfg_defconfig ${S}/../defconfig
}

# copy zImage to deploy directory
# update uImage with defconfig ane git info in name
# this is since build script can build multiple ways
# and will overwrite previous builds

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install  arch/arm/boot/zImage ${DEPLOY_DIR_IMAGE}/zImage_mfgtool
}
