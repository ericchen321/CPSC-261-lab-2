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
#include "printRoutines.h"

#define ERROR_RETURN -1
#define SUCCESS 0

int main(int argc, char **argv) {
  
  FILE *machineCode, *outputFile;
  uint8_t currByte;
  uint64_t pc = 0;

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
  // printPosition(outputFile, 0x1234);
  // printInstruction(outputFile);

  // Your code starts here.

  // Searches for first byte from start of file, print .pos if not at 0x00
  // FIXME: consider empty machinecode
  fread(&currByte,1,1,machineCode);
  while(currByte == 0x00){
    fread(&currByte,1,1,machineCode);
    pc+=1;
  }
  if (pc != 0){
    printPosition(outputFile, (unsigned long)pc);
  }

  do{

  }
  while(fread(&currByte,1,1,machineCode));
  
  fclose(machineCode);
  fclose(outputFile);
  return SUCCESS;
}
