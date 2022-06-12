# Makefile for all GF1 commands, Debian Linux (GCC)
# Copyright (c) 2017-2022 Samuel Lourenço

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


# Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com


CC = gcc
CFLAGS = -O2 -std=gnu99 -Wall -pedantic
CXX = g++
CXXFLAGS = -O2 -std=c++11 -Wall -pedantic
LDFLAGS = -s
LDLIBS = -lusb-1.0
OBJECTS = cp2130.o error.o gf1device.o libusb-extra.o utils.o
TARGETS = gf1-info gf1-list gf1-reset gf1-start gf1-stop

.PHONY: all clean install uninstall

all: $(TARGETS)

$(TARGETS): % : %.o $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	$(RM) *.o $(TARGETS)

install:
	mv -f $(TARGETS) /usr/local/bin/.

uninstall:
	cd /usr/local/bin && $(RM) $(TARGETS)
