# Copyright (C) 2015 VIA Technologies,Inc.

SUMMARY = "gst-fsl-plugins for QSM8Q60"
DESCRIPTION = "The gstreamer hardware accelerated plugins for QSM8Q60"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI += "file://0001-EminZhang-redrawing-colorkey-when-exit-open-file-men.patch \
            file://0002-redrawing-colorkey-when-drag-open-file-menu.patch \
            file://improve-redraw-colorkey-when-drag-videoplayer-open-f.patch"

COMPATIBLE_MACHINE = "(mx6)"
