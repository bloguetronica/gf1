/* Utility functions - Version 1.0.0
   Copyright (c) 2022 Samuel Lourenço

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
#include <cstring>
#include "utils.h"

// Checks if a given C string constitutes a valid integer
bool Utils::isInteger(char *value)
{
    bool retval = true;
    if (value[0] != '+' && value[0] != '-' && (value[0] < '0' || value[0] > '9')) {  // Number is not valid if the first character is not a '+', '-', nor a digit
        retval = false;
    } else {
        size_t len = strlen(value);
        for (size_t i = 1; i < len; ++i) {
            if (value[i] < '0' || value[i] > '9') {  // Number is not valid if subsequent characters are not digits
                retval = false;
                break;
            }
        }
    }
    return retval;
}

// Checks if a given string constitutes a valid integer
bool Utils::isInteger(std::string value)
{
    return isInteger(value.c_str());
}

// Checks if a given C string constitutes a valid number
bool Utils::isNumber(char *value)
{
    bool retval = true;
    if (value[0] != '+' && value[0] != '-' && value[0] != '.' && (value[0] < '0' || value[0] > '9')) {  // Number is not valid if the first character is not a '+', '-', '.', nor a digit
        retval = false;
    } else {
        bool dp_exists = false;
        if (value[0] == '.') {  // If character is a decimal point
            dp_exists = true;
        }
        size_t len = strlen(value);
        for (size_t i = 1; i < len; ++i) {
            if ((value[i] != '.' || dp_exists) && (value[i] < '0' || value[i] > '9')) {  // Number is not valid if subsequent characters are not digits or if there is more than one decimal point
                retval = false;
                break;
            }
            if (value[i] == '.') {  // As before, if character is a decimal point
                dp_exists = true;
            }
        }
    }
    return retval;
}

// Checks if a given string constitutes a valid number
bool Utils::isNumber(std::string value)
{
    return isNumber(value.c_str());
}
