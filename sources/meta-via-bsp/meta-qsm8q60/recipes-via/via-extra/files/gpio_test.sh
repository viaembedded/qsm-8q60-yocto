#!/bin/bash

#######################################
# Test script for QSM-8Q60-DB2 GPIOs
#
# 2015/09/02
#######################################




declare sBoard=QSM-8Q60-DB2		# Test script for special board
declare sVersion=v0.0.1				# Script Released Version
declare sDate=2015.09.02			# Script Released Date
declare -i nReturn=0					# function return value




echo -e "\n\033[1m GPIO test script on $sBoard board $sVersion --- $sDate\033[0m\n"




# check i2c-tools exist or not
function detect_i2ctools()
{
	(find /usr/sbin/i2cget >> /dev/null) || return $?
	(find /usr/sbin/i2cset >> /dev/null) || return $?
	return 0
}
detect_i2ctools || nReturn=$?
[ $nReturn -ne 0 ] && echo -e " Please make sure i2c-tools had beed included when building the image.\n" && exit $nReturn




echo -e "\033[1m================ Start Fintek F75113 (x32) GPIOs Function Test ================\033[0m\n"




# Enable GPIO function
function gpio_mode()
{
	i2cset -y 0 0x37 0x02 0x0 || return $?		# GPIO0X mode control Register 1
	i2cset -y 0 0x37 0x03 0x0 || return $?		# GPIO0X mode control Register 2
	i2cset -y 0 0x37 0x04 0x0 || return $?		# GPIO1X mode control Register 1
	i2cset -y 0 0x37 0x05 0x0 || return $?		# GPIO1X mode control Register 2
	i2cset -y 0 0x37 0x06 0x0 || return $?		# GPIO2X mode control Register
}
gpio_mode || nReturn=$?
[ $nReturn -ne 0 ] && echo -e " Please check your permission!\n" && exit $nReturn




function gpio0123_high()
{
	echo -e "\n Set GPOX0 ~ GPOX3 to high, GPIX4 ~ GPIX7 are input\n"
	
	i2cset -y 0 0x37 0x10 0x0F || return $?		# GPIO0X Input Output control Register
	i2cset -y 0 0x37 0x20 0x0F || return $?		# GPIO1X Input Output control Register
	i2cset -y 0 0x37 0x30 0x0F || return $?		# GPIO2X Input Output control Register
	i2cset -y 0 0x37 0x40 0x0F || return $?		# GPIO3X Input Output control Register
	
	i2cset -y 0 0x37 0x11 0x0F || return $?		# GPIO0X output data Register
	i2cset -y 0 0x37 0x21 0x0F || return $?		# GPIO1X output data Register
	i2cset -y 0 0x37 0x31 0x0F || return $?		# GPIO2X output data Register
	i2cset -y 0 0x37 0x41 0x0F || return $?		# GPIO3X output data Register

	[ $(i2cget -y 0 0x37 0x12) == 0xff ] || return $?			# GPIO0X status Register
	[ $(i2cget -y 0 0x37 0x22) == 0xff ] || return $?			# GPIO1X status Register
	[ $(i2cget -y 0 0x37 0x32) == 0xff ] || return $?			# GPIO2X status Register
	[ $(i2cget -y 0 0x37 0x42) == 0xff ] || return $?			# GPIO3X status Register
}




function gpio0123_low()
{
	echo -e "\n Set GPOX0 ~ GPOX3 to low, GPIX4 ~ GPIX7 are input\n"

	i2cset -y 0 0x37 0x10 0x0F || return $?		# GPIO0X Input Output control Register
	i2cset -y 0 0x37 0x20 0x0F || return $?		# GPIO1X Input Output control Register
	i2cset -y 0 0x37 0x30 0x0F || return $?		# GPIO2X Input Output control Register
	i2cset -y 0 0x37 0x40 0x0F || return $?		# GPIO3X Input Output control Register
	
	i2cset -y 0 0x37 0x11 0x0 || return $?		# GPIO0X output data Register
	i2cset -y 0 0x37 0x21 0x0 || return $?		# GPIO1X output data Register
	i2cset -y 0 0x37 0x31 0x0 || return $?		# GPIO2X output data Register
	i2cset -y 0 0x37 0x41 0x0 || return $?		# GPIO3X output data Register

	[ $(i2cget -y 0 0x37 0x12) == 0x00 ] || return $?			# GPIO0X status Register
	[ $(i2cget -y 0 0x37 0x22) == 0x00 ] || return $?			# GPIO1X status Register
	[ $(i2cget -y 0 0x37 0x32) == 0x00 ] || return $?			# GPIO2X status Register
	[ $(i2cget -y 0 0x37 0x42) == 0x00 ] || return $?			# GPIO3X status Register
}




function gpio4567_high()
{
	echo -e "\n Set GPOX4 ~ GPOX7 to high, GPIX0 ~ GPIX3 are input\n"
	
	i2cset -y 0 0x37 0x10 0xF0 || return $?		# GPIO0X Input Output control Register
	i2cset -y 0 0x37 0x20 0xF0 || return $?		# GPIO1X Input Output control Register
	i2cset -y 0 0x37 0x30 0xF0 || return $?		# GPIO2X Input Output control Register
	i2cset -y 0 0x37 0x40 0xF0 || return $?		# GPIO3X Input Output control Register
	
	i2cset -y 0 0x37 0x11 0xF0 || return $?		# GPIO0X output data Register
	i2cset -y 0 0x37 0x21 0xF0 || return $?		# GPIO1X output data Register
	i2cset -y 0 0x37 0x31 0xF0 || return $?		# GPIO2X output data Register
	i2cset -y 0 0x37 0x41 0xF0 || return $?		# GPIO3X output data Register

	[ $(i2cget -y 0 0x37 0x12) == 0xff ] || return $?			# GPIO0X status Register
	[ $(i2cget -y 0 0x37 0x22) == 0xff ] || return $?			# GPIO1X status Register
	[ $(i2cget -y 0 0x37 0x32) == 0xff ] || return $?			# GPIO2X status Register
	[ $(i2cget -y 0 0x37 0x42) == 0xff ] || return $?			# GPIO3X status Register
}




function gpio4567_low()
{
	echo -e "\n Set GPOX4 ~ GPOX7 to low, GPIX0 ~ GPIX3 are input\n"
	
	i2cset -y 0 0x37 0x10 0xF0 || return $?		# GPIO0X Input Output control Register
	i2cset -y 0 0x37 0x20 0xF0 || return $?		# GPIO1X Input Output control Register
	i2cset -y 0 0x37 0x30 0xF0 || return $?		# GPIO2X Input Output control Register
	i2cset -y 0 0x37 0x40 0xF0 || return $?		# GPIO3X Input Output control Register
	
	i2cset -y 0 0x37 0x11 0x0 || return $?		# GPIO0X output data Register
	i2cset -y 0 0x37 0x21 0x0 || return $?		# GPIO1X output data Register
	i2cset -y 0 0x37 0x31 0x0 || return $?		# GPIO2X output data Register
	i2cset -y 0 0x37 0x41 0x0 || return $?		# GPIO3X output data Register

	[ $(i2cget -y 0 0x37 0x12) == 0x00 ] || return $?			# GPIO0X status Register
	[ $(i2cget -y 0 0x37 0x22) == 0x00 ] || return $?			# GPIO1X status Register
	[ $(i2cget -y 0 0x37 0x32) == 0x00 ] || return $?			# GPIO2X status Register
	[ $(i2cget -y 0 0x37 0x42) == 0x00 ] || return $?			# GPIO3X status Register
}




# $1: GPIO status
function gpio_jumper()
{
	if [ $(($1 & 0x1)) == $(($1 >> 4 & 0x1)) ]; then
		echo -en " \e[1;33m|\e[0m "
	else
		echo -en " \e[1;31mx\e[0m "
	fi
	
	if [ $(($1 >> 1 & 0x1)) == $(($1 >> 5 & 0x1)) ]; then
		echo -en " \e[1;33m|\e[0m "
	else
		echo -en " \e[1;31mx\e[0m "
	fi

	if [ $(($1 >> 2 & 0x1)) == $(($1 >> 6 & 0x1)) ]; then
		echo -en " \e[1;33m|\e[0m "
	else
		echo -en " \e[1;31mx\e[0m "
	fi

	if [ $(($1 >> 3 & 0x1)) == $(($1 >> 7 & 0x1)) ]; then
		echo -en " \e[1;33m|\e[0m   "
	else
		echo -en " \e[1;31mx\e[0m   "
	fi
}




function gpio_pins()
{
	echo -e "\t04 05 06 07 X 14 15 16 17 X 24 25 26 27 X 34 35 36 37 X"
	echo -en "\t"
	
	if [ $1 ]; then
		gpio_jumper $1
		gpio_jumper $1
		gpio_jumper $1
		gpio_jumper $1
	else
		gpio_jumper $(i2cget -y 0 0x37 0x12)
		gpio_jumper $(i2cget -y 0 0x37 0x22)
		gpio_jumper $(i2cget -y 0 0x37 0x32)
		gpio_jumper $(i2cget -y 0 0x37 0x42)
	fi
	
	echo -e "\e[1;33m<--- Jumper\e[0m"
	echo -e "\t00 01 02 03 X 10 11 12 13 X 20 21 22 23 X 30 31 32 33 X"
}




echo -e "\e[1m Please short GPIOs as the following...\e[0m\n"
gpio_pins 0
echo -ne "\n\e[1m Press \e[0m\e[1;33mENTER\e[0m\e[1m when ready...\e[0m"
read
	
# GPO5 -> GPI6; GPO7 -> GPI8 => output high
gpio0123_high || nReturn=$?
gpio_pins

# GPO5 -> GPI6; GPO7 -> GPI8 => output low
gpio0123_low || nReturn=$?
gpio_pins

# GPO6 -> GPI5; GPO8 -> GPI7 => output high
gpio4567_high || nReturn=$?
gpio_pins

# GPO6 -> GPI5; GPO8 -> GPI7 => output low
gpio4567_low || nReturn=$?
gpio_pins




#[ $nReturn -ne 0 ] && echo " => error code: $nReturn"
[ $nReturn == 0 ] && echo -e "\n\033[1;34m GPIOs function test successfully!\033[0m\n"
[ $nReturn -ne 0 ] && echo -e "\n\033[1;31m GPIOs function test failed!\033[0m\n" && echo -e " Plese check if GPIO pins are shorted on $sBoard board.\n"


exit $nReturn



