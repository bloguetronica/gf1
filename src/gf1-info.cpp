/* GF1 Info Command - Version 1.0 for Debian Linux
   Copyright (c) 2022 Samuel Lourenço

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
#include <codecvt>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <locale>
#include <string>
#include "cp2130.h"
#include "error.h"
#include "gf1device.h"

int main(int argc, char **argv)
{
    int err, errlvl = EXIT_SUCCESS;
    GF1Device device;
    if (argc < 2) {  // If the program was called without arguments
        err = device.open();  // Open a device and get the device handle
    } else {  // Serial number was specified as argument
        err = device.open(argv[1]);  // Open the device having the specified serial number, and get the device handle
    }
    if (err == GF1Device::SUCCESS) {  // Device was successfully opened
        int errcnt = 0;
        std::string errstr;
        std::u16string manufacturer = device.getManufacturerDesc(errcnt, errstr);  // Manufacturer descriptor
        std::u16string product = device.getProductDesc(errcnt, errstr);  // Product descriptor
        std::u16string serial = device.getSerialDesc(errcnt, errstr);  // Serial number descriptor
        CP2130::USBConfig config = device.getUSBConfig(errcnt, errstr);  // USB configuration
        if (errcnt > 0) {  // In case of error
            if (device.disconnected()) {  // If the device disconnected
                std::cerr << "Device disconnected.\n";
            } else {
                printErrors(errstr);
            }
            errlvl = EXIT_FAILURE;
        } else {  // Operation successful
            std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
            std::cout << "Manufacturer: " << converter.to_bytes(manufacturer) << std::endl;  // Print manufacturer string
            std::cout << "Product: " << converter.to_bytes(product) << std::endl;  // Print product string
            std::cout << "Serial number: " << converter.to_bytes(serial) << std::endl;  // Print serial number string
            std::cout << "Hardware revision: " << GF1Device::hardwareRevision(config) << " [0x" << std::hex << std::setfill ('0') << std::setw(4) << (config.majrel << 8 | config.minrel) << std::dec << "]" << std::endl;  // Print hardware revision
            std::cout << "Maximum power consumption: " << 2 * config.maxpow << "mA [0x" << std::hex << std::setw(2) << static_cast<int>(config.maxpow) << "]" << std::endl;  // Print maximum power consumption
        }
        device.close();
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
