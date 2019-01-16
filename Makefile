all: disassembler

CC=gcc
CLIBS=
CFLAGS=-g -Wall -pedantic -std=c99
LDFLAGS=-g -Wall -pedantic -std=c99

DISASSEMBLEOBJS=disassembler.o printRoutines.o

disassembler: $(DISASSEMBLEOBJS)

disassembler.o: disassembler.c y86.h printRoutines.h
printRoutines.o: printRoutines.c y86.h printRoutines.h

clean:
	-rm -rf *.o disassembler
