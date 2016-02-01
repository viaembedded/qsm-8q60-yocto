FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}_${PV}:"
PRINC := "${@int(PRINC) + 1}"

DEPENDS += " libdca"

