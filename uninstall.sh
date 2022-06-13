#!/bin/sh

echo Rolling back configurations...
rm -f /etc/udev/rules.d/70-bgtn-gf1.rules
service udev restart
echo Removing binaries and man pages...
make -C /usr/local/src/gf1 uninstall
echo Removing source code files...
rm -rf /usr/local/src/gf1
echo Done!
