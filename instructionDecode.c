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

// effects: extract regA and regB from given currByte
void fill_ra_rb(uint8_t* currByte, uint8_t* regA, uint8_t* regB){
  *regA = *currByte >> 4;
  *regB = *currByte & 0x0F;
}

// effects: based on the given currByte, write opCode, regA, regB, value C to given pointers;
//          increment file pointer's cursor;
//          increment *pc to the next instruction
void instructionDecode(uint8_t* currByte, uint8_t* opCode, uint8_t* regA, uint8_t* regB, uint64_t* C, uint64_t* pc, FILE* inputFile){
  uint8_t arr[7];
  size_t successfull_read_num;
  int i;
  
  if (*currByte == HALT){
    // FIXME: also consider quad word = 0 cases, and a bunch of other cases
    *opCode = *currByte;
    *pc+=1;
  }
    
  else if (*currByte == NOP){
    *opCode = *currByte;
    *pc+=1;
  }
    
  else if (*currByte == RRMOVQ){
    *opCode = *currByte;
    fread(currByte,1,1,inputFile);
    fill_ra_rb(currByte, regA, regB);
    *pc+=2;
  }
    
  else if (*currByte == IRMOVQ){
    *opCode = *currByte;
    fread(currByte,1,1,inputFile);
    *regB = *currByte & 0x0F;
    fread(C,8,1,inputFile);
    *pc+=10;
  }
    
  else if (*currByte == RMMOVQ){
    *opCode = *currByte;
    fread(currByte,1,1,inputFile);
    fill_ra_rb(currByte, regA, regB);
    fread(C,8,1,inputFile);
    *pc+=10;
  }
    
  else if (*currByte == MRMOVQ){
    *opCode = *currByte;
    fread(currByte,1,1,inputFile);
    fill_ra_rb(currByte, regA, regB);
    fread(C,8,1,inputFile);
    *pc+=10;
  }
    
  else if (*currByte == CALL){
    *opCode = *currByte;
    fread(C,8,1,inputFile);
    *pc+=9;
  }

  else if (*currByte == RET){
    *opCode = *currByte;
    *pc+=1;
  }

  else if (*currByte == PUSHQ){
    *opCode = *currByte;
    fread(currByte,1,1,inputFile);
    *regA = *currByte >> 4;
    *pc+=2;
  }
      
  else if (*currByte == POPQ){
    *opCode = *currByte;
    fread(currByte,1,1,inputFile);
    *regA = *currByte >> 4;
    *pc+=2;
  }

  else if ((*currByte >> 4) == 0x02 && (*currByte & 0x0F) != 7){
    *opCode = *currByte;
    fread(currByte,1,1,inputFile);
    fill_ra_rb(currByte, regA, regB);
    *pc+=2;
  }

  else if ((*currByte >> 4) == 0x07 && (*currByte & 0x0F) != 7){
    *opCode = *currByte;
    fread(C,8,1,inputFile);
    *pc+=9;
  }
    
  else if ((*currByte >> 4) == 0x06 && (*currByte & 0x0F) != 7){
    *opCode = *currByte;
    fread(currByte,1,1,inputFile);
    fill_ra_rb(currByte, regA, regB);
    *pc+=2;
  }

  else {
    if (*pc % 8 == 0){
      successfull_read_num = fread(arr,1,7,inputFile);
      if(successfull_read_num == 7){ // Quad word read
        *opCode = QUAD_WORD;
        *C = 0;
        *C = (u_int64_t)*currByte;
        for(i=0; i<7; i++){
          *C = *C | ((u_int64_t)arr[i] << (8*(i+1)));
        }
        *pc+=8;
      }
      else {  // Byte word read
        fseek(inputFile, -successfull_read_num, SEEK_CUR);
        *opCode = BYTE_WORD;
        *C = *currByte;
        *pc+=1;
      }
    }
    else {  // Byte word read
      *opCode = BYTE_WORD;
      *C = (u_int64_t)currByte;
      *pc+=1;
    }
  }
}