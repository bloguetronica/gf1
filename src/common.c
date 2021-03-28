/* Common functions - Version 1.2
   Copyright (c) 2018-2019 Samuel Lourenço

   This library is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
   License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library.  If not, see <https://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


// Includes
#include <stddef.h>
#include "common.h"

bool isnumber(char *value)  // Checks if a given string constitutes a valid number
{
    bool retval = true;
    if (value[0] != '+' && value[0] != '-' && value[0] != '.' && (value[0] < '0' || value[0] > '9'))  // Number is not valid if the first character is not a '+', '-', '.', nor a digit
        retval = false;
    else
    {
        bool dp_exists = false;
        if (value[0] == '.')  // If character is a decimal point
            dp_exists = true;
        for (size_t i = 1; value[i] != '\0'; ++i)
        {
            if ((value[i] != '.' || dp_exists) && (value[i] < '0' || value[i] > '9'))  // Number is not valid if subsequent characters are not digits or if there is more than one decimal point
            {
                retval = false;
                break;
            }
            if (value[i] == '.')  // As before, if character is a decimal point
                dp_exists = true;
        }
    }
    return retval;
}
