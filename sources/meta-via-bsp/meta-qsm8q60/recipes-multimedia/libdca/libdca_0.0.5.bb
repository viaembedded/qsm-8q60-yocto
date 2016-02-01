DESCRIPTION = "Decoding library for DTS Coherent Acoustics streams."
HOMEPAGE = "http://www.videolan.org/developers/libdca.html"
LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://COPYING;md5=751419260aa954499f7abaabaa882bbe"

PV = "0.0.5+git${SRCPV}"
SRCREV = "e113628da67d7ccb11d06704778eed193a0b0a1c"
SRC_URI = "git://anonscm.debian.org/collab-maint/libdca.git"
S = "${WORKDIR}/git"

inherit autotools pkgconfig

