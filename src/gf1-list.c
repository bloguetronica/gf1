/* GF1 List Command - Version 1.0 for Debian Linux
   Copyright (c) 2018 Samuel Lourenço

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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

int main(void)
{
    libusb_context *context;
    int err_level = EXIT_SUCCESS;
    if (libusb_init(&context) != 0)  // Initialize libusb. In case of failure
    {
        fprintf(stderr, "Error: Could not initialize libusb.\n");
        err_level = EXIT_FAILURE;
    }
    else  // If libusb is initialized
    {
        libusb_device **devs;
        ssize_t devlist = libusb_get_device_list(context, &devs);  // Get a device list
        if (devlist < 0)  // If the previous operation fails to get a device list
        {
            fprintf(stderr, "Error: Failed to retrieve a list of devices.\n");
            err_level = EXIT_FAILURE;
        }
        else
        {
            int counter = 0;
            for (ssize_t i = 0; i < devlist; i++)  // Run through all listed devices
            {
                struct libusb_device_descriptor desc;
                if (libusb_get_device_descriptor(devs[i], &desc) == 0 && desc.idVendor == 0x10C4 && desc.idProduct == 0x8A7D)  // If the device descriptor is retrieved, and both VID and PID correspond to the GF1 Function Generator
                {
                    libusb_device_handle *devhandle;
                    counter++;  // Increment the counter, since a suitable device was found
                    printf("%d\t", counter);  // Print the item number
                    if (libusb_open(devs[i], &devhandle) == 0)  // Open the listed device. If successfull
                    {
                        unsigned char str_desc[256];
                        libusb_get_string_descriptor_ascii(devhandle, desc.iSerialNumber, str_desc, sizeof(str_desc));  // Get the serial number string in ASCII format
                        printf("%s", str_desc);  // Print the serial number string
                        libusb_close(devhandle);  // Close the device
                    }
                    else  // If not, the device is simply listed as unidentified (no error reported)
                        printf("Unidentified");
                    if (counter == 1)  // If the device is the first to be listed, then it must be the one that accepts commands by default (i.e., without having a serial number specified)
                        printf(" (default)");
                    printf("\n");  // End of line for the list item
                }
            }
            if (counter == 0)
                printf("No devices found.\n");
            libusb_free_device_list(devs, 1);  // Free device list
        }
        libusb_exit(context);  // Deinitialize libusb
    }
    return err_level;
}
