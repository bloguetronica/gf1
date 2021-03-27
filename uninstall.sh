#!/bin/sh

echo Rolling back configurations...
rm -f /etc/udev/rules.d/70-bgtn-gf1.rules
service udev restart
echo Removing man pages...
rm -f /usr/local/share/man/man1/gf1-amp.1.gz
rm -f /usr/local/share/man/man1/gf1-amp50.1.gz
rm -f /usr/local/share/man/man1/gf1-clear.1.gz
rm -f /usr/local/share/man/man1/gf1-freq.1.gz
rm -f /usr/local/share/man/man1/gf1-lockotp.1.gz
rm -f /usr/local/share/man/man1/gf1-reset.1.gz
rm -f /usr/local/share/man/man1/gf1-sine.1.gz
rm -f /usr/local/share/man/man1/gf1-start.1.gz
rm -f /usr/local/share/man/man1/gf1-stop.1.gz
rm -f /usr/local/share/man/man1/gf1-tri.1.gz
rmdir --ignore-fail-on-non-empty /usr/local/share/man/man1
echo Removing binaries...
make -C /usr/local/src/gf1 uninstall
echo Removing source code files...
rm -rf /usr/local/src/gf1
echo Done!
