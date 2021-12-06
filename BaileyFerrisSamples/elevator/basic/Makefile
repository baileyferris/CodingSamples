########################################
#
#  IUPUI CSCI 503 -- Operating Systems
#
########################################

## Compiler, tools and options
CC      = gcc
LINK    = gcc
OPT     = -g

CCFLAGS = $(OPT) -Wall -Wextra -Wvla -Wmissing-prototypes -pthread
LDFLAGS = $(OPT) -pthread

## Libraries
LIBS = 
INC  = -I.

## FILES
OBJECTS = basic.o
TARGET  = basic


## Implicit rules
.SUFFIXES: .c
.c.o:
	$(CC) -c $(CCFLAGS) $(INC) $<

## Build rules
all: $(TARGET) 

$(TARGET): $(OBJECTS)
	$(LINK) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET) 
	rm -f *~ core

# sync:
# 	rsync -azP ./ bkferris@tesla.cs.iupui.edu:/home/bkferris/lab2/