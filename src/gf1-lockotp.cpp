/* GF1 LockOTP Command - Version 3.0 for Debian Linux
   Copyright (c) 2017-2022 Samuel Lourenço

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <https://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


// Includes
#include <cstdlib>
#include <iostream>
#include <string>
#include "cp2130.h"
#include "error.h"
#include "gf1device.h"

// Definitions
static const int EXIT_USERERR = 2;  // Exit status value to indicate a command usage error

int main(int argc, char **argv)
{
    int errlvl = EXIT_SUCCESS;
    if (argc < 2) {  // If the program was called without arguments
        std::cerr << "Error: Missing argument.\nUsage: gf1-lockotp SERIALNUMBER\n";
        errlvl = EXIT_USERERR;
    } else {
        CP2130 cp2130;
        int err = cp2130.open(GF1Device::VID, GF1Device::PID, argv[1]);
        if (err == GF1Device::SUCCESS) {  // Open the device having the specified serial number, and get the device handle. If successful
            int errcnt = 0;
            std::string errstr;
            if (cp2130.isOTPLocked(errcnt, errstr) && errcnt == 0) {  // Check if the OTP ROM is locked (errcnt can increment as a consequence of that verification, hence the need for "&& errcnt == 0" in order to avoid misleading messages)
                std::cout << "Device OTP ROM is already locked." << std::endl;
            } else if (errcnt == 0) {  // If OTP is not locked
                std::cout << "Device OTP ROM contains unlocked fields, which can be overwritten." << std::endl;
                std::cout << "Do you wish to permanently lock all fields? [y/N] ";
                char cin;
                std::cin.get(cin);  // Get character entered by user
                if (cin == 'Y' || cin == 'y') {  // If user entered "Y" or "y"
                    cp2130.lockOTP(errcnt, errstr);  // Lock the OTP ROM
                    cp2130.reset(errcnt, errstr);  // Reset the device
                    if (errcnt > 0) {  // In case of error
                        if (cp2130.disconnected()) {  // If the device disconnected
                            std::cerr << "Device disconnected.\n";
                        } else {
                            printErrors(errstr);
                        }
                        errlvl = EXIT_FAILURE;
                    } else {  // Operation successful
                        std::cout << "Device OTP ROM is now locked." << std::endl;  // Notice that no verification is done after reset, since the device has to be allowed to re-enumerate before getting the updated register values
                    }
                } else {  // If user entered any other character
                    std::cout << "Lock operation canceled." << std::endl;
                }
            } else {
                if (cp2130.disconnected()) {  // If the device disconnected
                    std::cerr << "Device disconnected.\n";
                } else {
                    printErrors(errstr);
                }
                errlvl = EXIT_FAILURE;
            }
            cp2130.close();
        } else {  // Failed to open device
            if (err == GF1Device::ERROR_INIT) {  // Failed to initialize libusb
                std::cerr << "Could not initialize libusb\n";
            } else if (err == GF1Device::ERROR_NOT_FOUND) {  // Failed to find device
                std::cerr << "Could not find device.\n";
            } else if (err == GF1Device::ERROR_BUSY) {  // Failed to claim interface
                std::cerr << "Device is currently unavailable.\n";
            }
            errlvl = EXIT_FAILURE;
        }
        return errlvl;
    }
}
