#!/bin/sh

# assign variable from command line.

#var=$(read /sys/bus/usb/devices/1-1.1/idProduct)

var=$(cat /sys/bus/usb/devices/1-1.1/idProduct)
echo "var = $var"
