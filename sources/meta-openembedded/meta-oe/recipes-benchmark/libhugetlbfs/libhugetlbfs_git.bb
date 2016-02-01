SUMMARY = "A library which provides easy access to huge pages of memory"
LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://LGPL-2.1;md5=2d5025d4aa3495befef8f17206a5b0a1"

DEPENDS = "sysfsutils perl"
RDEPENDS_${PN} += "python python-io python-lang python-subprocess python-resource"

SRCREV = "ea3f6b273f535aab38cefae30030774457bbbfe6"
SRC_URI = "git://git.code.sf.net/p/libhugetlbfs/code \
    file://skip-checking-LIB32-and-LIB64-if-they-point-to-the-s.patch \
    file://libhugetlbfs-avoid-search-host-library-path-for-cros.patch \
    file://tests-Makefile-install-static-4G-edge-testcases.patch \
    file://0001-run_test.py-not-use-hard-coded-path-.-obj-hugeadm.patch \
    file://0001-aarch64-fix-cross-compilation.patch \
    file://0001-aarch64-fix-page-size-not-properly-computed.patch \
    file://0001-replace-lib-lib64-hardcoded-values-by-LIBDIR32-LIBDI.patch \
"

S = "${WORKDIR}/git"

COMPATIBLE_HOST = "(x86_64|powerpc|powerpc64|aarch64).*-linux*"

LIBARGS = "LIB32=${baselib} LIB64=${baselib}"
EXTRA_OEMAKE = "'ARCH=${TARGET_ARCH}' 'OPT=${CFLAGS}' 'CC=${CC}' ${LIBARGS} V=1"
CFLAGS += "-fexpensive-optimizations -frename-registers -fomit-frame-pointer -g0"

TARGET_CC_ARCH += "${LDFLAGS}"

#The CUSTOM_LDSCRIPTS doesn't work with the gold linker
do_configure() {
    if [ "${@base_contains('DISTRO_FEATURES', 'ld-is-gold', 'ld-is-gold', '', d)}" = "ld-is-gold" ] ; then
      sed -i 's/CUSTOM_LDSCRIPTS = yes/CUSTOM_LDSCRIPTS = no/'  Makefile
    fi

    # fixup perl module directory hardcoded to perl5
    sed -i 's/perl5/perl/g'  Makefile
}

do_install() {
        oe_runmake PREFIX=${prefix} DESTDIR=${D}  \
          INST_TESTSDIR32=${libdir}/libhugetlbfs/tests \
          INST_TESTSDIR64=${libdir}/libhugetlbfs/tests \
          install-tests
}

PARALLEL_MAKE_pn-${PN} = ""

PACKAGES =+ "${PN}-perl ${PN}-tests "
FILES_${PN}-dbg += "${libdir}/libhugetlbfs/tests/obj32/.debug ${libdir}/libhugetlbfs/tests/obj64/.debug"
FILES_${PN}-perl = "${libdir}/perl"
FILES_${PN}-tests += "${libdir}/libhugetlbfs/tests"

INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
