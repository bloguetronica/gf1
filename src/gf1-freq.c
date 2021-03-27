/* GF1 Freq Command - Version 2.0 for Debian Linux
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
#include <unistd.h>
#include <libusb-1.0/libusb.h>
#include "common.h"
#include "gf1-core.h"
#include "libusb-extra.h"

// Defines
#define EXIT_USERERR 2  // Exit status value to indicate a command usage error

int main(int argc, char **argv)
{
    err_level = EXIT_SUCCESS;  // Note that this variable is declared externally!
    if (argc < 2)  // If the program was called without arguments
    {
        fprintf(stderr, "Error: Missing argument.\nUsage: gf1-freq FREQUENCY(KHz)\n");
        err_level = EXIT_USERERR;
    }
    else if (!isnumber(argv[1]))  // If the argument string doesn't constitute a valid number
    {
        fprintf(stderr, "Error: Argument is not a valid number.\n");
        err_level = EXIT_USERERR;
    }
    else
    {
        float frequency = atof(argv[1]);  // Convert the argument string into a floating point number
        if (frequency < 0 || frequency > 25000)  // If the obtained frequency value (in KHz) after conversion is lesser than 0 or greater than 25000
        {
            fprintf(stderr, "Error: Frequency should be between 0 and 25000KHz.\n");
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
                libusb_device_handle *devhandle;
                if (argc == 2)  // If no serial number was specified
                    devhandle = libusb_open_device_with_vid_pid(context, 0x10C4, 0x8A7D);  // Open a device and get the device handle
                else  // Serial number was specified as a second (optional) argument
                    devhandle = libusb_open_device_with_vid_pid_serial(context, 0x10C4, 0x8A7D, (unsigned char *)argv[2]);  // Open the device having the specified serial number, and get the device handle
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
                        libusb_detach_kernel_driver(devhandle, 0);  // Detach the kernel driver
                        kernel_attached = true;  // Flag that the kernel driver was attached
                    }
                    if (libusb_claim_interface(devhandle, 0) != 0)  // Claim the interface. In case of failure
                    {
                        fprintf(stderr, "Error: Device is currently unavailable.\n");
                        err_level = EXIT_FAILURE;
                    }
                    else  // If the interface is successfully claimed
                    {
                        unsigned int freq_code = (unsigned int)(frequency * 8388608 / 25000 + 0.5);
                        configure_spi_mode(devhandle, 0, CPOL1, CPHA0);  // Clock polarity regarding channel 0 is active low (CPOL = 1) and data is valid on each falling edge (CPHA = 0)
                        disable_spi_delays(devhandle, 0);  // Disable all SPI delays for channel 0
                        set_gpio2(devhandle, false);  // Make sure that both GPIO.2
                        set_gpio3(devhandle, false);  // and GPIO.3 are set to to a logical low first
                        set_gpio3(devhandle, true);  // Then set GPIO.3 to a logical high
                        set_gpio3(devhandle, false);  // and again to a logical low (this toggle is not really necessary, unless the frequency increments are set to be externally triggered via GPIO.2/CTRL)
                        select_cs(devhandle, 0);  // Enable the chip select corresponding to channel 0, and disable any others
                        set_frequency(devhandle, freq_code);  // Set the frequency to the intended value (by sending a sequence of bytes to the AD5932 waveform generator on channel 0)
                        usleep(100);  // Wait 100us, in order to prevent possible errors while disabling the chip select (workaround)
                        disable_cs(devhandle, 0);  // Disable the previously enabled chip select
                        set_gpio2(devhandle, true);  // Set GPIO.2 to a logical high
                        set_gpio2(devhandle, false);  // and then to a logical low
                        if (err_level == 0)  // If all goes well
                            printf("Frequency set to %.3fKHz.\n", freq_code * 25000.0 / 8388608);
                        libusb_release_interface(devhandle, 0);  // Release the interface
                    }
                    if (kernel_attached)  // If a kernel driver was attached to the interface before
                        libusb_attach_kernel_driver(devhandle, 0);  // Reattach the kernel driver
                    libusb_close(devhandle);  // Close the device
                }
                libusb_exit(context);  // Deinitialize libusb
            }
        }
    }
    return err_level;
}
