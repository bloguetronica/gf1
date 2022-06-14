This package contains all the necessary files and scripts to install the
commands for GF1 Function Generator. The scripts included here are compatible
with most 32-bit and 64-bit Debian based operating systems (e.g. Linux Mint,
Ubuntu and variants, etc). Prior to installation, you must certify that your
system is Debian based (or at least uses apt-get) and that you have an active
Internet connection.

A list of the included scripts follows:
– install.sh;
– test.sh;
– uninstall.sh.

In order to compile and install the commands for the first time, all you have
to do is run "install.sh" by invoking "sudo ./install.sh" on a terminal
window, after changing your working directory to the current one. This script
will first obtain and install the required "build-essential" and
"libusb-1.0-0-dev" packages (if they are not installed yet). Then it will
compile the binaries and move them to "/usr/local/bin/". Note that the build
process will also compress the respective man pages and copy them to
"/usr/local/share/man/man1/". Finally, it will create the necessary rules for
accessing the device.

The package also includes a test script named "test.sh". This will set up GF1
so that it generates a 1KHz triangle wave signal with its amplitude set to
4Vpp. If the installation was successful, this script should run without
errors of any kind. Note that "test.sh" doesn't require root permissions.

Lastly, to uninstall, you should run "uninstall.sh" by invoking "sudo
./uninstall.sh", again on a terminal window after making sure that your
working directory is this one. This will delete all the previously installed
binaries, the related man pages and source code, and the rules created during
the installation. However it won't remove the "build-essential" and
"libusb-1.0-0-dev" packages, since they could be already installed prior to
the first installation.
