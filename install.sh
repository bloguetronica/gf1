#!/bin/sh

echo Obtaining required packages...
apt-get -qq update
apt-get -qq install build-essential
apt-get -qq install libusb-1.0-0-dev
echo Copying source code files...
mkdir -p /usr/local/src/gf1/man
cp -f src/cp2130.cpp /usr/local/src/gf1/.
cp -f src/cp2130.h /usr/local/src/gf1/.
cp -f src/error.cpp /usr/local/src/gf1/.
cp -f src/error.h /usr/local/src/gf1/.
cp -f src/gf1-amp.cpp /usr/local/src/gf1/.
cp -f src/gf1-amp50.cpp /usr/local/src/gf1/.
cp -f src/gf1-clear.cpp /usr/local/src/gf1/.
cp -f src/gf1device.cpp /usr/local/src/gf1/.
cp -f src/gf1device.h /usr/local/src/gf1/.
cp -f src/gf1-freq.cpp /usr/local/src/gf1/.
cp -f src/gf1-info.cpp /usr/local/src/gf1/.
cp -f src/gf1-list.cpp /usr/local/src/gf1/.
cp -f src/gf1-lockotp.cpp /usr/local/src/gf1/.
cp -f src/gf1-reset.cpp /usr/local/src/gf1/.
cp -f src/gf1-sine.cpp /usr/local/src/gf1/.
cp -f src/gf1-start.cpp /usr/local/src/gf1/.
cp -f src/gf1-stop.cpp /usr/local/src/gf1/.
cp -f src/gf1-tri.cpp /usr/local/src/gf1/.
cp -f src/GPL.txt /usr/local/src/gf1/.
cp -f src/LGPL.txt /usr/local/src/gf1/.
cp -f src/libusb-extra.c /usr/local/src/gf1/.
cp -f src/libusb-extra.h /usr/local/src/gf1/.
cp -f src/Makefile /usr/local/src/gf1/.
cp -f src/man/gf1-amp.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-amp50.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-clear.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-freq.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-info.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-list.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-lockotp.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-reset.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-sine.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-start.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-stop.1 /usr/local/src/gf1/man/.
cp -f src/man/gf1-tri.1 /usr/local/src/gf1/man/.
cp -f src/README.txt /usr/local/src/gf1/.
cp -f src/utils.cpp /usr/local/src/gf1/.
cp -f src/utils.h /usr/local/src/gf1/.
echo Building and installing binaries and man pages...
make -C /usr/local/src/gf1 install clean
echo Applying configurations...
cat > /etc/udev/rules.d/70-bgtn-gf1.rules << EOF
SUBSYSTEM=="usb", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8a7d", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8a7d", MODE="0666"
EOF
service udev restart
echo Done!
