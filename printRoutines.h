/* This file contains the prototypes and constants needed to use the
   routines defined in printRoutines.c
*/

#ifndef _PRINTROUTINES_H_
#define _PRINTROUTINES_H_

#include <stdio.h>

int printPosition(FILE *, unsigned long);
int printInstruction(FILE *);
char* reg_num_to_str(uint8_t*);
char* cond_opCode_to_str(uint8_t*);
void printLineHex(FILE*, uint8_t*, uint8_t*, uint8_t*, uint64_t*, uint64_t*, _Bool*);
void printLine(FILE* , uint8_t*, uint8_t*, uint8_t*, uint64_t*, uint64_t*, uint64_t*, _Bool*);

#endif /* PRINTROUTINES */
