/* This file contains the prototypes and constants needed to use the
   routines defined in printRoutines.c
*/

#ifndef _PRINTROUTINES_H_
#define _PRINTROUTINES_H_

#include <stdio.h>

int printPosition(FILE *, unsigned long);
int printInstruction(FILE *);
void printLine(FILE* , uint8_t*, uint8_t*, uint8_t*, uint64_t*, uint64_t*, uint64_t*, _Bool*);

#endif /* PRINTROUTINES */
