.PHONY: all, clean

# Disable implicit rules
.SUFFIXES:

CC=gcc
CFLAGS=-Wall -g
VPATH=src/

# Note: -lnsl does not seem to work on Mac OS but will
# probably be necessary on Solaris for linking network-related functions
#LIBS += -lsocket -lnsl -lrt
LIBS+=-lpthread

INCLUDE = readcmd.h csapp.h traiter_cmd.h listeInt.h listeJobs.h variablesExternes.h handlersSignaux.h fonctions_utiles.h
OBJS = readcmd.o csapp.o traiter_cmd.o listeInt.o listeJobs.o handlersSignaux.o fonctions_utiles.o
INCLDIR = -I.

all: clean shell loop loop_100

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) $(INCLDIR) -c -o $@ $<

%: %.o $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

clean:
	rm -f shell loop loop_100 *.o
