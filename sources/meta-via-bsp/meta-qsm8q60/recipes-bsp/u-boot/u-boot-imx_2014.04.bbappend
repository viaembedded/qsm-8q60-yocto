# Copyright (C) 2014, 2015 VIA Technologies, Inc.

SUMMARY = "Linux Kernel provided by VIA Technologies, Inc."
DESCRIPTION = "Linux Kernel provided by VIA Technologies, Inc."

FILESEXTRAPATHS := "${THISDIR}/${PN}:"

SRC_URI += "file://qsm8q60-uboot-imx.patch"

COMPATIBLE_MACHINE = "(mx6)"
