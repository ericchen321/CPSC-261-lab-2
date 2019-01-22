#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "y86.h"
#include "instructionDecode.h"
#include "printRoutines.h"

#define ERROR_RETURN -1
#define SUCCESS 0

int main(int argc, char **argv) {
  
  FILE *machineCode, *outputFile;
  uint8_t currByte;
  uint8_t opCode;
  uint8_t regA, regB;
  uint64_t C;
  uint64_t pc = 0;
  uint64_t pc_not_updated;  // pass un-updated offset to print
  _Bool halt_flag = 0;      // set high after a HALT instruction is printed; set low when the flag is high
                            // AND a non-HALT instruction encountered
  uint64_t zero_count = 0;  // this counts the number of zero-bytes starting from the 1st halt instruction,
                            // so after the 1st halt ins this becomes 1, and add by 1 everytime a new HALT is
                            // read and halt_flag is high. Set to 0 after a non-halt instruction is encountered 
                            // again. Meant to be read after entire machine code is read. 
                            // If >= 1 then print the last zero-byte's position and .byte 0x0.

  // Verify that the command line has an appropriate number
  // of arguments

  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s InputFilename [OutputFilename]\n", argv[0]);
    return ERROR_RETURN;
  }

  // First argument is the file to read, attempt to open it 
  // for reading and verify that the open did occur.
  machineCode = fopen(argv[1], "rb");

  if (machineCode == NULL) {
    fprintf(stderr, "Failed to open %s: %s\n", argv[1], strerror(errno));
    return ERROR_RETURN;
  }

  // Second argument is the file to write, attempt to open it for
  // writing and verify that the open did occur. Use standard output
  // if not provided.
  outputFile = argc <= 2 ? stdout : fopen(argv[2], "w");
  
  if (outputFile == NULL) {
    fprintf(stderr, "Failed to open %s: %s\n", argv[2], strerror(errno));
    fclose(machineCode);
    return ERROR_RETURN;
  }

  fprintf(stderr, "Opened %s\n", argv[1]);
  fprintf(stderr, "Saving output to %s\n", argc <= 2 ? "standard output" : argv[2]);

  // Comment or delete the following lines and this comment before
  // handing in your final version.
  //printPosition(outputFile, 0x1234);
  //printInstruction(outputFile);

  // Your code starts here.

  // assuming file contains at least one byte
  // Searches for first non-zero byte from start of file,
  // quit if end of file encountered or got a non-zero byte
  while(fread(&currByte,1,1,machineCode) && currByte == 0x00){
    pc+=1;
  }

  if (pc == 1 && currByte == 0x00){ // file contains single zero byte 
    fprintf(outputFile, "    .byte 0x0\n");
    return SUCCESS;
  }
  else if (pc >= 2 && currByte == 0x00){ // file contains only and at least 2 bytes with 0
    printPosition(outputFile, (unsigned long)(pc-1));
    fprintf(outputFile, "    .byte 0x0\n");
    return SUCCESS;
  }
  else if (currByte != 0x00 && pc != 0){ // first non-zero byte not the first byte
    printPosition(outputFile, (unsigned long)pc);
  }
  else if (currByte !=0x00 && pc == 0){ // first byte is non-zero
    // does not print pos
  }
  
  do{
    pc_not_updated = pc; 
    instructionIdentify(&currByte, &opCode, &pc, machineCode);
    instructionDecode(&currByte, &opCode, &regA, &regB, &C, &pc, machineCode);
    printLine(outputFile, &opCode, &regA, &regB, &C, &pc_not_updated, &zero_count, &halt_flag);
  }
  while(fread(&currByte,1,1,machineCode));

  if(zero_count >= 1){
    printPosition(outputFile, (unsigned long)pc_not_updated);
    fprintf(outputFile, "    .byte 0x0\n");
  }
    
  fclose(machineCode);
  fclose(outputFile);
  return SUCCESS;
}
