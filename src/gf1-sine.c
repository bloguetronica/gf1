/* GF1 Sine Command - Version 1.0 for Debian Linux
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
void configure_spi_mode(libusb_device_handle *, unsigned char, bool, bool);
void disable_cs(libusb_device_handle *, unsigned char);
void disable_spi_delays(libusb_device_handle *, unsigned char);
void select_cs(libusb_device_handle *, unsigned char);
void set_gpio2(libusb_device_handle *, bool);
void set_gpio3(libusb_device_handle *, bool);
void set_sine_wave(libusb_device_handle *);

// Global variables
int err_level = EXIT_SUCCESS;  // This variable is manipulated by other functions besides main()!

int main(void)
{
    libusb_context *context;
    libusb_device_handle *devhandle;
    bool kernel_attached = false;
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
                set_gpio2(devhandle, false);  // First, ensure that both GPIO.2
                set_gpio3(devhandle, false);  // and GPIO.3 are set to a logical low
                configure_spi_mode(devhandle, 0, true, false);  // Clock polarity regarding channel 0 is active low (CPOL = 1) and data is valid on each falling edge (CPHA = 0)
                disable_spi_delays(devhandle, 0);  // Disable all SPI delays for channel 0
                select_cs(devhandle, 0);  // Enable the chip select corresponding to channel 0, and disable any others
                set_sine_wave(devhandle);  // Set the waveform to sinusoidal (by sending a specific sequence of bytes to the AD5932 waveform generator on channel 0)
                disable_cs(devhandle, 0);  // Disable the previously enabled chip select
                set_gpio2(devhandle, true);  // Set GPIO.2 to a logical high
                set_gpio2(devhandle, false);  // and then to a logical low
                if (err_level == 0)  // If all goes well
                    printf("Waveform set to sinusoidal.\n");
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

void configure_spi_mode(libusb_device_handle *devhandle, unsigned char channel, bool cpol, bool cpha)  // Configures the given SPI channel in respect to its clock polarity and phase
{
    unsigned char control_buf_out[2] = {
        channel,                          // Selected channel
        0x20 * cpha + 0x10 * cpol + 0x08  // Control word (specified polarity and phase, push-pull mode, 12MHz)
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x31, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x31).\n");
        err_level = EXIT_FAILURE;
    }
}

void disable_cs(libusb_device_handle *devhandle, unsigned char channel)  // Disables the chip select corresponding to the target channel
{
    unsigned char control_buf_out[2] = {
        channel,  // Selected channel
        0x00      // Corresponding chip select disabled
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x25, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x25).\n");
        err_level = EXIT_FAILURE;
    }
}

void disable_spi_delays(libusb_device_handle *devhandle, unsigned char channel)  // Disables all SPI delays for a given channel
{
    unsigned char control_buf_out[8] = {
        channel,     // Selected channel
        0x00,        // All SPI delays disabled, no CS toggle
        0x00, 0x00,  // Inter-byte,
        0x00, 0x00,  // post-assert and
        0x00, 0x00   // pre-deassert delays all set to 0us
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x33, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x33).\n");
        err_level = EXIT_FAILURE;
    }
}

void select_cs(libusb_device_handle *devhandle, unsigned char channel)  // Enables the chip select of the target channel, disabling any others
{
    unsigned char control_buf_out[2] = {
        channel,  // Selected channel
        0x02      // Only the corresponding chip select is enabled, all the others are disabled
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x25, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x25).\n");
        err_level = EXIT_FAILURE;
    }
}

void set_gpio2(libusb_device_handle *devhandle, bool value)  // Sets the GPIO.2 pin on the CP2130 to a given value
{
    unsigned char control_buf_out[4] = {
        0x00, 0xFF * value,  // Set the value of GPIO.2 to the intended value
        0x00, 0x20           // Set the mask so that only GPIO.2 is changed
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x21, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x21).\n");
        err_level = EXIT_FAILURE;
    }
}

void set_gpio3(libusb_device_handle *devhandle, bool value)  // Sets the GPIO.3 pin on the CP2130 to a given value
{
    unsigned char control_buf_out[4] = {
        0x00, 0xFF * value,  // Set the value of GPIO.3 to the intended value
        0x00, 0x40           // Set the mask so that only GPIO.3 is changed
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x21, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x21).\n");
        err_level = EXIT_FAILURE;
    }
}

void set_sine_wave(libusb_device_handle *devhandle)  // Configures the control register on the AD5932 waveform generator to generate a sine wave (channel 0 must be enabled)
{
    unsigned char write_command_buf[10] = {
        0x00, 0x00,              // Reserved
        0x01,                    // Write command
        0x00,                    // Reserved
        0x02, 0x00, 0x00, 0x00,  // Two bytes to write
        0x0F, 0xD3               // Sinusoidal waveform, automatic increments, MSBOUT pin enabled, SYNCOUT pin disabled, B24 = 1, SYNCSEL = 0
    };
    int bytes_written;
    if (libusb_bulk_transfer(devhandle, 0x01, write_command_buf, sizeof(write_command_buf), &bytes_written, TR_TIMEOUT) != 0)
    {
        fprintf(stderr, "Error: Failed bulk OUT transfer to endpoint address 0x01.\n");
        err_level = EXIT_FAILURE;
    }
}
