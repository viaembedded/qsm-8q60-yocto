SUMMARY = "Freescale GPU SDK Samples"
DESCRIPTION = "Set of sample applications for Freescale GPU"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://COPYING;md5=cf7b2a944fc10cd1e2471a89ea6b7585"
DEPENDS = "${WL_DEPENDS} ${DFB_DEPENDS} "
DEPENDS_append_mx6q = " virtual/libgles2"
DEPENDS_append_mx6dl = " virtual/libgles2"
DEPENDS_append_mx6sx = " virtual/libgles2"

WL_DEPENDS = "${@base_contains('DISTRO_FEATURES', 'wayland', 'wayland', '', d)}"
DFB_DEPENDS =  "${@base_contains('DISTRO_FEATURES', 'directfb', 'directfb', '', d)}"

inherit fsl-eula-unpack

# For backwards compatibility
RPROVIDES_${PN} = "vivante-gpu-sdk"
RREPLACES_${PN} = "vivante-gpu-sdk"
RCONFLICTS_${PN} = "vivante-gpu-sdk"

SRC_URI = "${FSL_MIRROR}/${PN}-${PV}.bin;fsl-eula=true"

SRC_URI[md5sum] = "c74bba9bfcaf6cb2448c42bb4f597181"
SRC_URI[sha256sum] = "8bee7fedc91e054713742d949d749195b6e0cfa3479c1b0168e588a4104d47a9"

S = "${WORKDIR}/${PN}-${PV}"

BACKEND = "${@base_contains('DISTRO_FEATURES', 'x11', 'X11', \
                    base_contains('DISTRO_FEATURES', 'wayland', 'Wayland', \
                           base_contains('DISTRO_FEATURES', 'directfb', 'DirectFB', 'FB', d), d), d)}"
do_compile () {
     export FSL_GRAPHICS_SDK=${S}
     export FSL_PLATFORM_NAME=Yocto
     export ROOTFS=${STAGING_DIR_HOST}
    ./build.sh -f GNUmakefile_Yocto EGLBackend=${BACKEND}
}

do_install () {
   export FSL_GRAPHICS_SDK=${S}
   export FSL_PLATFORM_NAME=Yocto
   install -d "${D}/opt/${PN}"
  ./build.sh -f  GNUmakefile_Yocto EGLBackend=${BACKEND} install 
   cp -r bin/* "${D}/opt/${PN}"
   rm -rf ${D}/opt/${PN}/GLES2/S05_PrecompiledShader
   rm -rf ${D}/opt/${PN}/GLES3/S05_PrecompiledShader
}

FILES_${PN} += "/opt/${PN}"
FILES_${PN}-dev = "/usr"
FILES_${PN}-dbg = "/opt/${PN}/*/*/.debug"
INSANE_SKIP_${PN} += "rpaths"
