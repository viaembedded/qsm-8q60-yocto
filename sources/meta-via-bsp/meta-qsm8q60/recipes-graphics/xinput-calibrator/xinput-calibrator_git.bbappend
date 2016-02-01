# Mark 'Exec' to disable auto start calibration program

do_install_append() {
	cp ${D}${sysconfdir}/xdg/autostart/xinput_calibrator.desktop ${WORKDIR}/xinput_calibrator.desktop.bak
    sed -e 's,^Exec=,#Exec=,' ${WORKDIR}/xinput_calibrator.desktop.bak > ${D}${sysconfdir}/xdg/autostart/xinput_calibrator.desktop
}
