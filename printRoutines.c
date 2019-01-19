#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "y86.h"
#include "printRoutines.h"

/*
  You probably want to create a number of printing routines in this
  file.  Put the prototypes in printRoutines.h.
*/

int printPosition(FILE *out, unsigned long location) {

  return fprintf(out, ".pos 0x%lx\n", location);
}

/* This is a function to demonstrate how to do print formatting. It
 * takes the file descriptor the I/O is to be done to. You are not
 * required to use the same type of printf formatting, but you must
 * produce the same resulting format. You are also not required to
 * keep this function in your final result, and may replace it with
 * other equivalent functions.
 *
 * The arguments for the format string in the example printing are
 * strictly for illustrating the spacing. You are free to construct
 * the output however you want.
 */
int printInstruction(FILE *out) {

  int res = 0;
  
  char * r1 = "%rax";
  char * r2 = "%rdx";
  char * inst1 = "rrmovq";
  char * inst2 = "jne";
  char * inst3 = "irmovq";
  char * inst4 = "mrmovq";
  unsigned long destAddr = 8193;
  
  res += fprintf(out, "    %-8s%s, %s          # %-22s\n", 
		 inst1, r1, r2, "2002");

  res += fprintf(out, "    %-8s0x%lx              # %-22s\n", 
		 inst2, destAddr, "740120000000000000");

  res += fprintf(out, "    %-8s$0x%lx, %s         # %-22s\n", 
		 inst3, 16L, r2, "30F21000000000000000");

  res += fprintf(out, "    %-8s0x%lx(%s), %s # %-22s\n", 
		 inst4, 65536L, r2, r1, "50020000010000000000"); 
  
  res += fprintf(out, "    %-8s%s, %s          # %-22s\n", 
		 inst1, r2, r1, "2020");
  
  res += fprintf(out, "    %-8s0x%lx  # %-22s\n", 
		 ".quad", 0xFFFFFFFFFFFFFFFFL, "FFFFFFFFFFFFFFFF");

  return res;
}  

// requires: no input pointers should be NULL;
//           value of pc's reference is offset of given instruction/word
// effects: prints instruction/word to given file
void printLine(FILE* outFile, uint8_t* opCode, uint8_t* regA, uint8_t* regB, uint64_t* C, uint64_t* pc, _Bool* halt_flag){  
  if(*opCode == HALT){
    if (!(*halt_flag)){
      fprintf(outFile, "halt instr\n");
      *halt_flag = 1;
    }
  }
  else {
    if(*halt_flag){
      printPosition(outFile, (unsigned long)*pc);
      *halt_flag = 0;
    }
    
    if(*opCode == NOP){
      fprintf(outFile, "nop\n");
    }

    else if(*opCode == RRMOVQ){
      fprintf(outFile, "rrmovq\n");
    }

    else if(*opCode == IRMOVQ){
      fprintf(outFile, "irmovq\n");
    }

    else if(*opCode == RMMOVQ){
      fprintf(outFile, "rmmovq\n");
    }

    else if(*opCode == MRMOVQ){
      fprintf(outFile, "mrmovq\n");
    }

    else if(*opCode == CALL){
      fprintf(outFile, "call\n");
    }

    else if(*opCode == RET){
      fprintf(outFile, "ret\n");
    }

    else if(*opCode == PUSHQ){
      fprintf(outFile, "pushq\n");
    }

    else if(*opCode == POPQ){
      fprintf(outFile, "popq\n");
    }

    else if ((*opCode >> 4) == 0x02 && (*opCode & 0x0F) != 7){
      fprintf(outFile, "some kind of comov\n");
    }

    else if((*opCode >> 4) == 0x07 && (*opCode & 0x0F) != 7){
      fprintf(outFile, "some kind of jump\n");
    }

    else if ((*opCode >> 4) == 0x06 && (*opCode & 0x0F) != 7){
      fprintf(outFile, "some kind of arith op\n");
    }

    else if(*opCode == QUAD_WORD){
      fprintf(outFile, "a quad word: %lx\n", *C);
    }

    else if(*opCode == BYTE_WORD){
      fprintf(outFile, "a byte word\n");
    }

    else{
      fprintf(outFile, "\n");
    }
  }
}
  
