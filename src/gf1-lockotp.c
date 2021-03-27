/* GF1 LockOTP Command - Version 1.0 for Debian Linux
   Copyright (c) 2017 Samuel Lourenço

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


// Includes
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

// Defines
#define TR_TIMEOUT 500  // Transfer timeout in milliseconds

// Function prototypes
bool is_otp_locked(libusb_device_handle *);
void lock_otp(libusb_device_handle *);
void reset(libusb_device_handle *);

// Global variables
int err_level = EXIT_SUCCESS;  // This variable is manipulated by other functions besides main()!

int main(void)
{
    libusb_context *context;
    libusb_device_handle *devhandle;
    bool kernel_attached = false;
    char cin;
    if (libusb_init(&context) != 0)  // Initialize libusb. In case of failure
    {
        fprintf(stderr, "Error: Could not initialize libusb.\n");
        err_level = EXIT_FAILURE;
    }
    else  // If libusb is initialized
    {
        devhandle = libusb_open_device_with_vid_pid (context, 0x10C4, 0x8A7D);  // Open a device and get the device handle
        if (devhandle == NULL)  // If the previous operation fails to get a device handle
        {
            fprintf(stderr, "Error: Could not find device.\n");
            err_level = EXIT_FAILURE;
        }
        else  // If the device is successfully opened and a handle obtained
        {
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
                    cin = getc(stdin);  // Get character entered by user
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
    printf("(Exit status %d)\n", err_level);
    return err_level;
}

bool is_otp_locked(libusb_device_handle *devhandle)  // Checks if the OTP ROM of the CP2130 is locked
{
    unsigned char control_buf_in[2];
    if (libusb_control_transfer(devhandle, 0xC0, 0x6E, 0x0000, 0x0000, control_buf_in, sizeof(control_buf_in), TR_TIMEOUT) != sizeof(control_buf_in))
    {
        fprintf(stderr, "Error: Failed control transfer (0xC0, 0x6E).\n");
        err_level = EXIT_FAILURE;
    }
    return (control_buf_in[0] == 0x00 && control_buf_in[1] == 0x00);  // Returns one if both lock bytes are set to zero, that is, the OPT ROM is locked
}

void lock_otp(libusb_device_handle *devhandle)  // Locks the OTP ROM on the CP2130
{
    unsigned char control_buf_out[2] = {
        0x00, 0x00  // Values to be written into the lock bytes, so that both are set to zero
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x6F, 0xA5F1, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x6F).\n");
        err_level = EXIT_FAILURE;
    }
}

void reset(libusb_device_handle *devhandle)  // Issues a reset to the CP2130, which in effect resets the entire device
{
    if (libusb_control_transfer(devhandle, 0x40, 0x10, 0x0000, 0x0000, NULL, 0, TR_TIMEOUT) != 0)
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x10).\n");
        err_level = EXIT_FAILURE;
    }
}
