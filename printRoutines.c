#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
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

/* effects: returns the string representing register of given number
 *          if 0x0 <= *reg <= 0xF. Otherwise return "NO WAY!"
 */
char* reg_num_to_str(uint8_t* reg){
  switch (*reg){
    case 0x0:
      return "%rax";

    case 0x1:
      return "%rcx";

    case 0x2:
      return "%rdx";

    case 0x3:
      return "%rbx";

    case 0x4:
      return "%rsp";

    case 0x5:
      return "%rbp";

    case 0x6:
      return "%rsi";
    
    case 0x7:
      return "%rdi";

    case 0x8:
      return "%r8";

    case 0x9:
      return "%r9";

    case 0xA:
      return "%r10";

    case 0xB:
      return "%r11";
    
    case 0xC:
      return "%r12";

    case 0xD:
      return "%r13";

    case 0xE:
      return "%r14";

    default: 
      return "NO WAY!";
  }
}

/* requires: given opCode is the valid opCode of one of cmovXX,
 *           OPq, or jXX operations
 * effects: returns the string representing the operation with
 *          given opCode to out_str
 */
char* cond_opCode_to_str(uint8_t* opCode){
  uint8_t cond;

  if ((*opCode >> 4) == 0x02){
    cond = *opCode & 0x0F;

    switch(cond){
      case NO_CON:
        return "cmov";

      case LE:
        return "cmovle";
      
      case L:
        return "cmovl";

      case E:
        return "cmove";

      case NE:
        return "cmovne";
      
      case GE:
        return "cmovge";

      case G:
        return "cmovg";

      default:
        return "messed up";
    }
  }

  else if((*opCode >> 4) == 0x07){
    cond = *opCode & 0x0F;

    switch(cond){
      case NO_CON:
        return "jmp";

      case LE:
        return "jle";
      
      case L:
        return "jl";

      case E:
        return "je";

      case NE:
        return "jne";
      
      case GE:
        return "jge";

      case G:
        return "jg";

      default:
        return "messed up";
    }
  }

  else if((*opCode >> 4) == 0x06){
    switch(*opCode){
      case ADDQ:
        return "addq";
      
      case SUBQ:
        return "subq";
      
      case ANDQ:
        return "addq";

      case XORQ:
        return "xorq";
      
      case MULQ:
        return "mulq";

      case DIVQ:
        return "divq";

      case MODQ:
        return "modq";
      
      default:
        return "messed up";
    }
  }

  else{
    return "messed up";
  }
}

/* requires: no input pointers should be NULL;
 *           value of pc's reference is offset of given instruction/word
 * effects: prints instruction/word to given file
 */
void printLine(FILE* outFile, uint8_t* opCode, uint8_t* regA, uint8_t* regB, uint64_t* C, uint64_t* pc, uint64_t* zero_count, _Bool* halt_flag){
  if(*opCode == HALT){
    if (!(*halt_flag)){
      fprintf(outFile, "    halt\n");
      *halt_flag = 1;
    }
    else{
      *zero_count += 1;
    }
  }
  else {
    if(*halt_flag){
      fprintf(outFile, "\n");
      printPosition(outFile, (unsigned long)*pc);
      *halt_flag = 0;
      *zero_count = 0;
    }
    
    if(*opCode == NOP){
      fprintf(outFile, "    nop\n");
    }

    else if(*opCode == RRMOVQ){
      fprintf(outFile, "    %-8s%s, %s\n", "rrmovq", reg_num_to_str(regA), reg_num_to_str(regB));
    }

    else if(*opCode == IRMOVQ){
      fprintf(outFile, "    %-8s$0x%lx, %s\n", "irmovq", (unsigned long)(*C), reg_num_to_str(regB));
    }

    else if(*opCode == RMMOVQ){
      fprintf(outFile, "    %-8s%s, 0x%lx(%s)\n", "rmmovq", reg_num_to_str(regA), (unsigned long)(*C), reg_num_to_str(regB));
    }

    else if(*opCode == MRMOVQ){
      fprintf(outFile, "    %-8s$0x%lx(%s), %s\n", "mrmovq", (unsigned long)(*C), reg_num_to_str(regB), reg_num_to_str(regA));
    }

    else if(*opCode == CALL){
      fprintf(outFile, "    %-8s$0x%lx\n", "call", (unsigned long)(*C));
    }

    else if(*opCode == RET){
      fprintf(outFile, "    ret\n");
    }

    else if(*opCode == PUSHQ){
      fprintf(outFile, "    %-8s%s\n", "pushq", reg_num_to_str(regA));
    }

    else if(*opCode == POPQ){
      fprintf(outFile, "    %-8s%s\n", "popq", reg_num_to_str(regA));
    }

    else if ((*opCode >> 4) == 0x02 && (*opCode & 0x0F) != 7){ // cmov
      fprintf(outFile, "    %-8s%s, %s\n", cond_opCode_to_str(opCode), reg_num_to_str(regA), reg_num_to_str(regB));
    }

    else if((*opCode >> 4) == 0x07 && (*opCode & 0x0F) != 7){ // jumps
      fprintf(outFile, "    %-8s0x%lx\n", cond_opCode_to_str(opCode), (unsigned long)(*C));
    }

    else if ((*opCode >> 4) == 0x06 && (*opCode & 0x0F) != 7){ // arith ops
      fprintf(outFile, "    %-8s%s, %s\n", cond_opCode_to_str(opCode), reg_num_to_str(regA), reg_num_to_str(regB));
    }

    else if(*opCode == QUAD_WORD){
      fprintf(outFile, "    .quad 0x%lx\n", (unsigned long)(*C));
    }

    else if(*opCode == BYTE_WORD){
      fprintf(outFile, "    .byte 0x%lx\n", (unsigned long)(*C));
    }

    else{
      fprintf(outFile, "THIS SHOULD NEVER APPEAR!!!\n");
    }
  }
}
  
