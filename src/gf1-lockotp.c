/* GF1 LockOTP Command - Version 2.0 for Debian Linux
   Copyright (c) 2017-2018 Samuel Lourenço

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
#include "gf1-core.h"
#include "libusb-extra.h"

// Defines
#define EXIT_USERERR 2  // Exit status value to indicate a command usage error

int main(int argc, char **argv)
{
    err_level = EXIT_SUCCESS;  // Note that this variable is declared externally!
    if (argc < 2)  // If the program was called without arguments
    {
        fprintf(stderr, "Error: Missing argument.\nUsage: gf1-lockotp SERIALNUMBER\n");
        err_level = EXIT_USERERR;
    }
    else
    {
        libusb_context *context;
        if (libusb_init(&context) != 0)  // Initialize libusb. In case of failure
        {
            fprintf(stderr, "Error: Could not initialize libusb.\n");
            err_level = EXIT_FAILURE;
        }
        else  // If libusb is initialized
        {
            libusb_device_handle *devhandle = libusb_open_device_with_vid_pid_serial(context, 0x10C4, 0x8A7D, (unsigned char *)argv[1]);  // Open the device having the specified serial number, and get the device handle
            if (devhandle == NULL)  // If the previous operation fails to get a device handle
            {
                fprintf(stderr, "Error: Could not find device.\n");
                err_level = EXIT_FAILURE;
            }
            else  // If the device is successfully opened and a handle obtained
            {
                bool kernel_attached = false;
                if (libusb_kernel_driver_active(devhandle, 0) != 0)  // If a kernel driver is active on the interface
                {
                    libusb_detach_kernel_driver(devhandle, 0);  // Detach the kernel driver from the interface
                    kernel_attached = true;  // Flag that the kernel driver was attached
                }
                if (libusb_claim_interface(devhandle, 0) != 0)  // Claim the interface. In case of failure
                {
                    fprintf(stderr, "Error: Device is currently unavailable.\n");
                    err_level = EXIT_FAILURE;
                }
                else  // If the interface is successfully claimed
                {
                    if (is_otp_locked(devhandle) && err_level == 0)  // Check if the OTP ROM is locked (err_level can change to 1 as a consequence of that verification, hence the need for "&& err_level == 0" in order to avoid misleading messages)
                        printf("Device OTP ROM is already locked.\n");
                    else if (err_level == 0)  // If all goes well
                    {
                        printf("Device OTP ROM contains unlocked fields, which can be overwritten.\n");
                        printf("Do you wish to permanently lock all fields? [y/N] ");
                        char cin = getc(stdin);  // Get character entered by user
                        if (cin == 'Y' || cin == 'y')  // If user entered "Y" or "y"
                        {
                            lock_otp(devhandle);  // Lock the OTP ROM
                            reset(devhandle);  // Reset the device
                            if (err_level == 0)  // If all goes well
                                printf("Device OTP ROM is now locked.\n");  // Notice that no verification is done after reset, since the device has to be allowed to re-enumerate before getting the updated register values
                        }   
                        else  // If user entered any other character
                            printf("Lock operation canceled.\n");
                    }
                    libusb_release_interface(devhandle, 0);  // Release the interface
                }
                if (kernel_attached)  // If a kernel driver was attached to the interface before
                    libusb_attach_kernel_driver(devhandle, 0);  // Reattach the kernel driver
                libusb_close(devhandle);  // Close the device
            }
            libusb_exit(context);  // Deinitialize libusb
        }
    }
    return err_level;
}
