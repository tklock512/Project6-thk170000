# Filename: Makefile
# Date: 4/28/2020
# Author: Terrence Klock
# Email: thk170000@utdallas.edu
# Version: 1.0
# Copyright 2020, All Rights Reserved
#
# Description: 
#
# Makefile for Program6, complies CDK code
#

#compiler call
CXX = g++
#compiler flags
CPPFLAGS = -Wall -g -I /scratch/perkins/include

#library flags
LFLAGS = -L /scratch/perkins/lib
#linked libraries
LLIBS = -lcdk -lcurses

#executable name
EXEC = executable

#Object files
OBJS = example1.o

all: $(EXEC)

clean: 
	rm -f $(OBJS) $(EXEC) *~ 

$(EXEC): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LFLAGS) $(LLIBS)
