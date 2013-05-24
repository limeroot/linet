# 	networkd Makefile
#
# 	Created on 11/05/2013 17:37:12
#
# 	This file is part of the networkd command for GNU/Linux LimeRoot
#
# 	Copyright (c) LimeRoot, http://www.limeroot.org, devel@limeroot.org
#
# 	Author: Daniel R. Torres Ortiz, daniel@limeroot.org
#
# 	This program is free software: you can redistribute it and/or modify
# 	it under the terms of the GNU General Public License as published by
# 	the Free Software Foundation, either version 3 of the License, or
# 	(at your option) any later version.
#
# 	This program is distributed in the hope that it will be useful,
# 	but WITHOUT ANY WARRANTY; without even the implied warranty of
# 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# 	GNU General Public License for more details.
#
# 	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

OBJS = main.o nicmon.o system.o database.o sqlite3.o wan.o wan_pppoe.o interface.o
EXTOBJS = /usr/local/boost32/lib/libboost_filesystem.a /usr/local/boost32/lib/libboost_system.a
CC = g++
DEBUG = -ggdb
CFLAGS = -Wall -c $(DEBUG) -std=c++0x -m32 
LFLAGS = -Wall $(DEBUG) -std=c++0x -m32 
LIBFLAGS = -ldl -lpthread

#-lboost_system -lboost_filesystem -L/usr/local/boost32/lib

linet : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $(EXTOBJS) -o linet $(LIBFLAGS)

main.o : main.cpp nicmon/nicmon.h constants.h wan/wan.h wan/wan_pppoe.h
	$(CC) $(CFLAGS) main.cpp
	
nicmon.o : nicmon/nicmon.h nicmon/nicmon.cpp constants.h system/system.h
	$(CC) $(CFLAGS) nicmon/nicmon.cpp

system.o : system/system.h system/system.h
	$(CC) $(CFLAGS) system/system.cpp
    
database.o : database/database.h database/database.h
	$(CC) $(CFLAGS) database/database.cpp

sqlite3.o : database/sqlite3.h database/sqlite3.c
	gcc -Wall -c -m32 database/sqlite3.c

wan.o : wan/wan.h wan/wan.h interface/interface.h database/database.h
	$(CC) $(CFLAGS) wan/wan.cpp

wan_pppoe.o : wan/wan_pppoe.h wan/wan_pppoe.cpp wan/wan.h interface/interface.h database/database.h
	$(CC) $(CFLAGS) wan/wan_pppoe.cpp

interface.o : interface/interface.h interface/interface.cpp
	$(CC) $(CFLAGS) interface/interface.cpp

clean :
	rm -rf *.o linet
    
    
    