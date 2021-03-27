#!/bin/sh

echo Obtaining required packages...
apt-get -qq update
apt-get -qq install build-essential
apt-get -qq install libusb-1.0-0-dev
echo Copying source code files...
mkdir -p /usr/local/src/gf1
cp -f src/gf1-amp.c /usr/local/src/gf1/.
cp -f src/gf1-clear.c /usr/local/src/gf1/.
cp -f src/gf1-freq.c /usr/local/src/gf1/.
cp -f src/gf1-lockotp.c /usr/local/src/gf1/.
cp -f src/gf1-reset.c /usr/local/src/gf1/.
cp -f src/gf1-sine.c /usr/local/src/gf1/.
cp -f src/gf1-start.c /usr/local/src/gf1/.
cp -f src/gf1-stop.c /usr/local/src/gf1/.
cp -f src/gf1-tri.c /usr/local/src/gf1/.
cp -f src/Makefile /usr/local/src/gf1/.
cp -f src/README.txt /usr/local/src/gf1/.
echo Building and installing binaries...
make -C /usr/local/src/gf1 all install clean
echo Installing man pages...
mkdir -p /usr/local/share/man/man1
cp -f man/gf1-amp.1.gz /usr/local/share/man/man1/.
cp -f man/gf1-clear.1.gz /usr/local/share/man/man1/.
cp -f man/gf1-freq.1.gz /usr/local/share/man/man1/.
cp -f man/gf1-lockotp.1.gz /usr/local/share/man/man1/.
cp -f man/gf1-reset.1.gz /usr/local/share/man/man1/.
cp -f man/gf1-sine.1.gz /usr/local/share/man/man1/.
cp -f man/gf1-start.1.gz /usr/local/share/man/man1/.
cp -f man/gf1-stop.1.gz /usr/local/share/man/man1/.
cp -f man/gf1-tri.1.gz /usr/local/share/man/man1/.
echo Applying configurations...
cat > /etc/udev/rules.d/70-bgtn-gf1.rules << EOF
SUBSYSTEM=="usb", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8a7d", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8a7d", MODE="0666"
EOF
service udev restart
echo Done!
