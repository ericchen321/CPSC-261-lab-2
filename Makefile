all: disassembler

CC=gcc
CLIBS=
CFLAGS=-g -Wall -pedantic -std=c99
LDFLAGS=-g -Wall -pedantic -std=c99

DISASSEMBLEOBJS=disassembler.o instructionDecode.o printRoutines.o

disassembler: $(DISASSEMBLEOBJS)

disassembler.o: disassembler.c y86.h instructionDecode.h printRoutines.h
instructionDecode.o: instructionDecode.c y86.h instructionDecode.h
printRoutines.o: printRoutines.c y86.h printRoutines.h

clean:
	-rm -rf *.o disassembler
