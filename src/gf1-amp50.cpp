/* GF1 Amp50 Command - Version 3.0 for Debian Linux
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
#include <iomanip>
#include <iostream>
#include <string>
#include "error.h"
#include "gf1device.h"
#include "utils.h"

// Global variables
int EXIT_USERERR = 2;  // Exit status value to indicate a command usage error

int main(int argc, char **argv)
{
    int err, errlvl = EXIT_SUCCESS;
    if (argc < 2) {  // If the program was called without arguments
        std::cerr << "Error: Missing argument.\nUsage: gf1-amp50 AMPLITUDE(Vpp)\n";
        errlvl = EXIT_USERERR;
    } else if (!Utils::isNumber(argv[1])) {  // If the argument string doesn't constitute a valid number
        std::cerr << "Error: Argument is not a valid number.\n";
        errlvl = EXIT_USERERR;
    } else {  // Serial number was specified as argument
        float amplitude = 2 * atof(argv[1]);  // Convert the argument string into a floating point number, and multiply it by 2 in order to compensate the effect of the 50Ω load
        if (amplitude < GF1Device::AMPLITUDE_MIN|| amplitude > GF1Device::AMPLITUDE_MAX) {  // If the obtained amplitude value (in Vpp) after conversion is lesser than "AMPLITUDE_MIN" [0] or greater than "AMPLITUDE_MAX" [5]
            std::cerr << "Error: Amplitude should be between 0 and 2.5Vpp.\n";
            errlvl = EXIT_USERERR;
        } else {
            GF1Device device;
            if (argc < 3) {  // If no serial number was specified
                err = device.open();  // Open a device and get the device handle
            } else {  // Serial number was specified as a second (optional) argument
                err = device.open(argv[2]);  // Open the device having the specified serial number, and get the device handle
            }    
            if (err == GF1Device::SUCCESS) {  // Device was successfully opened
                int errcnt = 0;
                std::string errstr;
                device.setupChannel1(errcnt, errstr);  // Setup channel 1
                device.setAmplitude(amplitude, errcnt, errstr);  // Set the amplitude of the generated signal to the given value
                if (errcnt > 0) {  // In case of error
                    if (device.disconnected()) {  // If the device disconnected
                        std::cerr << "Error: Device disconnected.\n";
                    } else {
                        printErrors(errstr);
                    }
                    errlvl = EXIT_FAILURE;
                } else {  // Operation successful
                    float finalAmplitude = GF1Device::expectedAmplitude(amplitude);
                    std::cout << "Amplitude set to " << std::fixed << std::setprecision(2) << finalAmplitude / 2 << "Vpp (" << finalAmplitude << "Vpp unterminated)." << std::endl;  // Print the set amplitude
                }
                device.close();
            } else {  // Failed to open device
                if (err == GF1Device::ERROR_INIT) {  // Failed to initialize libusb
                    std::cerr << "Error: Could not initialize libusb\n";
                } else if (err == GF1Device::ERROR_NOT_FOUND) {  // Failed to find device
                    std::cerr << "Error: Could not find device.\n";
                } else if (err == GF1Device::ERROR_BUSY) {  // Failed to claim interface
                    std::cerr << "Error: Device is currently unavailable.\n";
                }
                errlvl = EXIT_FAILURE;
            }
        }
    }
    return errlvl;
}
