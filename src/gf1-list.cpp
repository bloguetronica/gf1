/* GF1 List Command - Version 2.0 for Debian Linux
   Copyright (c) 2018-2022 Samuel Lourenço

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
#include <list>
#include <string>
#include "error.h"
#include "gf1device.h"

int main()
{
    int errcnt = 0, errlvl = EXIT_SUCCESS;
    std::string errstr;
    std::list<std::string> deviceList = GF1Device::listDevices(errcnt, errstr);  // Get a device list
    if (errcnt > 0) {  // In case of error
        printErrors(errstr);
        errlvl = EXIT_FAILURE;
    } else if (deviceList.empty()) {  // If list is empty
        std::cout << "No devices found." << std::endl;
    } else {
        size_t counter = 0;  // Device counter
        for (std::string device : deviceList) {  // Traverse device list
            ++counter;
            std::cout << counter << "\t" << device;  // Print device order and serial numbers
            if (counter == 1) {  // The first device on the list is always the preferred one
                std::cout << " (default)";
            }
            std::cout << std::endl;
        }
    }
    return errlvl;
}
