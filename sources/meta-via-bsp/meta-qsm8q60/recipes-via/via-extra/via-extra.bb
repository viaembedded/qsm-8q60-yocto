DESCRIPTION = "Extra scripts for via-image-gui image"
LICENSE = "CLOSED"

SRC_URI = "file://cpu_temperature \
		file://gpio_test.sh \
		file://Module-ZU200 \
		file://Chat-Module-ZU200-connect \
		file://Chat-Module-ZU200-disconnect \
		"

do_install () {
	install -d ${D}/usr/local/bin
	install -m 777 ${WORKDIR}/cpu_temperature ${D}/usr/local/bin
	install -m 777 ${WORKDIR}/gpio_test.sh ${D}/usr/local/bin
    install -d ${D}/etc/ppp/peers
    install -m 750 ${WORKDIR}/Module-ZU200 ${D}/etc/ppp/peers/
    install -m 750 ${WORKDIR}/Chat-Module-ZU200-connect ${D}/etc/ppp/peers/
    install -m 750 ${WORKDIR}/Chat-Module-ZU200-disconnect ${D}/etc/ppp/peers/
}

FILES_${PN} += "/usr/local/bin"
