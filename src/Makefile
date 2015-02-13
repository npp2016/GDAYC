##############################################################################
HOME     = /Users/mdekauwe
#CFLAGS   = -g -Wall -Wformat
CFLAGS   = -O3 -Wall
ARCH     =  x86_64
INCLS    = -I./include -I$(HOME)/include/
LIBS     = -lm 
CC       =  gcc 
PROGRAM  =  gday

SOURCES  =  \
$(PROGRAM).c read_met_file.c memory.c ini_file_parser.c

OBJECTS = $(SOURCES:.c=.o)
RM       =  rm -f
##############################################################################

# top level create the program...
all: 		$(PROGRAM)

# Compile the src file...
$(OBJECTS):	$(SOURCES)
		$(CC) ${INCLS} $(CFLAGS) -c $(SOURCES)

# Linking the program...
$(PROGRAM):	$(OBJECTS)
		$(CC) $(OBJECTS) $(LIBS) ${INCLS} $(CFLAGS) -o $(PROGRAM) 

clean:
		$(RM) $(OBJECTS) $(PROGRAM)

install:
		cp $(PROGRAM) $(HOME)/bin/$(ARCH)/.	
		$(RM) $(OBJECTS)
##############################################################################
