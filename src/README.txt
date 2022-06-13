This directory contains all source code files required for compiling the
commands for GF1 Function Generator. A list of relevant files follows:
– cp2130.cpp;
– cp2130.h;
– error.cpp;
– error.h;
– gf1-amp.cpp;
– gf1-amp50.cpp;
– gf1-clear.cpp;
– gf1device.cpp;
– gf1device.h;
– gf1-freq.cpp;
– gf1-info.cpp;
– gf1-list.cpp;
– gf1-lockotp.cpp;
– gf1-reset.cpp;
– gf1-sine.cpp;
– gf1-start.cpp;
– gf1-stop.cpp;
– gf1-tri.cpp;
– libusb-extra.c;
– libusb-extra.h;
– Makefile;
– man/gf1-amp.1;
– man/gf1-apm50.1;
– man/gf1-clear.1;
– man/gf1-freq.1;
– man/gf1-info.1;
– man/gf1-list.1;
– man/gf1-lockotp.1;
– man/gf1-reset.1;
– man/gf1-sine.1;
– man/gf1-start.1;
– man/gf1-stop.1;
– man/gf1-tri.1;
– utils.cpp;
– utils.h.

In order to compile successfully all commands, you must have the packages
"build-essential" and "libusb-1.0-0-dev" installed. Given that, if you wish to
simply compile, change your working directory to the current one on a terminal
window, and simply invoke "make" or "make all". If you wish to install besides
compiling, run "sudo make install". Alternatively, if you wish to force a
rebuild, you should invoke "make clean all", or "sudo make clean install" if
you prefer to install after rebuilding.

It may be necessary to undo any previous operations. Invoking "make clean"
will delete all object code generated (binaries included) during earlier
compilations. You can also invoke "sudo make uninstall" to unistall the
binaries.

P.S.:
Notice that any make operation containing the targets "install" or "uninstall"
(e.g. "make all install" or "make uninstall") requires root permissions, or in
other words, must be run with sudo.
