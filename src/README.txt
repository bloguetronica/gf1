This directory contains all source code files required for compiling the
commands for GF1 Function Generator. A list of relevant files follows:
– gf1-amp.c;
– gf1-clear.c;
– gf1-freq.c;
– gf1-lockotp.c;
– gf1-reset.c;
– gf1-sine.c;
– gf1-start.c;
– gf1-stop.c;
– gf1-tri.c;
– Makefile.

In order to compile successfully all commands, you must have the packages
"build-essential" and "libusb-1.0-0-dev" installed. Given that, if you wish to
simply compile, change your working directory to the current one on a terminal
window, and simply invoke "make" or "make all". If you wish to install besides
compiling, run "sudo make all install". Alternatively, if you wish to force a
rebuild, you should invoke "make clean all", or "sudo make clean all install"
if you prefer to install after rebuilding.

It may be necessary to undo any previous operations. Invoking "make clean"
will delete all object code generated (binaries included) during earlier
compilations. You can also invoke "sudo make uninstall" to unistall the
binaries.

P.S.:
Notice that any make operation containing the targets "install" or "uninstall"
(e.g. "make all install" or "make uninstall") requires root permissions, or in
other words, must be run with sudo.
