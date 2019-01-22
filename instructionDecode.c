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

/* effects: writes opCode based on currByte given. 
 *          If currByte is the opCode of a valid instruction
 *          and all following bytes are valid, then opCode = 
 *          currByte; otherwise opCode would be written as
 *          either QUAD_WORD for quad-word or BYTE_WORD for 
 *          byte-word.
 *          The function preserves currByte, pc and inputFile's 
 *          cursor offset.
 */
void instructionIdentify(uint8_t* currByte, uint8_t* opCode, uint64_t* pc, FILE* inputFile){
  uint8_t rA, rB;
  uint8_t nextByte;
  uint8_t byte_array[9];
  size_t successfull_read_num;

  if (*currByte == HALT || *currByte == NOP || *currByte == RET){
    *opCode = *currByte;
  }

  else if (((*currByte >> 4) == 0x02 || (*currByte >> 4) == 0x06) &&
           ((*currByte & 0x0F) >= 0 && (*currByte & 0x0F)<=6)){
    if(fread(&nextByte,1,1,inputFile)){
      fseek(inputFile, -1, SEEK_CUR);
      fill_ra_rb(&nextByte, &rA, &rB);
      if (rA >= 0 && rA <= 0x0E && rB >= 0 && rB <= 0x0E){  // valid register numbers
        *opCode = *currByte;
      }
      else{ // either quad or byte word
        identify_quad_or_byte_word(currByte, opCode, pc, inputFile);
      }
    }
    else{ // end of file reached and we have a byte word
      *opCode = BYTE_WORD;
    }
  }

  else if (*currByte == IRMOVQ){
    successfull_read_num = fread(byte_array,1,9,inputFile);
    fseek(inputFile, -successfull_read_num, SEEK_CUR);
    if (successfull_read_num == 9){ // can read in 9 more bytes
      fill_ra_rb(&byte_array[0], &rA, &rB);
      if (rA == 0x0F && rB >= 0 && rB <= 0x0E){  // valid rB and F is in rA
        *opCode = *currByte;
      }
      else{ // either quad or byte word
        identify_quad_or_byte_word(currByte, opCode, pc, inputFile);
      }
    }
    else{
      identify_quad_or_byte_word(currByte, opCode, pc, inputFile);
    }
  }

  else if (*currByte == RMMOVQ || *currByte == MRMOVQ){
    successfull_read_num = fread(byte_array,1,9,inputFile);
    fseek(inputFile, -successfull_read_num, SEEK_CUR);
    if (successfull_read_num == 9){ // can read in 9 more bytes
      fill_ra_rb(&byte_array[0], &rA, &rB);
      if (rA >= 0 && rA <= 0x0E && rB >= 0 && rB <= 0x0E){  // valid register numbers
        *opCode = *currByte;
      }
      else{ // either quad or byte word
        identify_quad_or_byte_word(currByte, opCode, pc, inputFile);
      }
    }
    else{
      identify_quad_or_byte_word(currByte, opCode, pc, inputFile);
    }
  }

  else if ((*currByte >> 4) == 0x07){
    successfull_read_num = fread(byte_array,1,8,inputFile);
    fseek(inputFile, -successfull_read_num, SEEK_CUR);
    if(successfull_read_num == 8 && (*currByte & 0x0F) >= 0 && (*currByte & 0x0F) <= 6){ // valid condition code
      *opCode = *currByte;
    }
    else{ // either quad or byte word
      identify_quad_or_byte_word(currByte, opCode, pc, inputFile);
    }
  }

  else if (*currByte == CALL){
    successfull_read_num = fread(byte_array,1,8,inputFile);
    fseek(inputFile, -successfull_read_num, SEEK_CUR);
    if (successfull_read_num == 8){
      *opCode = *currByte;
    }
    else{
      identify_quad_or_byte_word(currByte, opCode, pc, inputFile);
    }
  }

  else if (*currByte == PUSHQ || *currByte == POPQ){
    if(fread(&nextByte,1,1,inputFile)){
      fseek(inputFile, -1, SEEK_CUR);
      if( (nextByte >> 4) >= 0 && (nextByte >> 4) <= 0x0E){ // valid register numbers
        *opCode = *currByte;
      }
      else{
        identify_quad_or_byte_word(currByte, opCode, pc, inputFile);
      }
    }
    else{ // end of file reached and we have a byte word
      *opCode = BYTE_WORD;
    }
  }

  else{
    identify_quad_or_byte_word(currByte, opCode, pc, inputFile);
  }
}

/* effects: identify currByte as the LSB of a quad-word or as
 *          a byte word. 
 *          If is the LSB of a quad-word then writes opCode as
 *          QUAD_WORD, otherwise writes as BYTE_WORD.
 *          The function preserves currByte, pc, and inputFile's
 *          cursor offset.
 */
void identify_quad_or_byte_word(uint8_t* currByte, uint8_t* opCode, uint64_t* pc, FILE* inputFile){
  size_t successfull_read_num;
  uint8_t arr[7];

  if (*pc % 8 == 0){
      successfull_read_num = fread(arr,1,7,inputFile);
      fseek(inputFile, -successfull_read_num, SEEK_CUR);
      if(successfull_read_num == 7){ // Quad word read
        *opCode = QUAD_WORD;
      }
      else {  // Byte word read
        *opCode = BYTE_WORD;
      }
  }
  
  else {  // Byte word read
    *opCode = BYTE_WORD;
  }
}

/* effects: extract rA and rB from given currByte, and sets regA
 *          and regB.
 *          This function preserves currByte.
 */
void fill_ra_rb(uint8_t* currByte, uint8_t* regA, uint8_t* regB){
  *regA = *currByte >> 4;
  *regB = *currByte & 0x0F;
}

/* effects: based on the given opCode, regA, regB, value C to given pointers;
 *          increment file pointer's cursor;
 *          increment pc to the next instruction;
 *          preserves opCode.
 *          
 */
void instructionDecode(uint8_t* currByte, uint8_t* opCode, uint8_t* regA, uint8_t* regB, uint64_t* C, uint64_t* pc, FILE* inputFile){
  if (*opCode == HALT){
    *pc+=1;
  }
    
  else if (*opCode == NOP){
    *pc+=1;
  }
    
  else if (*opCode == RRMOVQ){
    fread(currByte,1,1,inputFile);
    fill_ra_rb(currByte, regA, regB);
    *pc+=2;
  }
    
  else if (*opCode == IRMOVQ){
    fread(currByte,1,1,inputFile);
    *regB = *currByte & 0x0F;
    fread(C,8,1,inputFile);
    *pc+=10;
  }
    
  else if (*opCode == RMMOVQ){
    fread(currByte,1,1,inputFile);
    fill_ra_rb(currByte, regA, regB);
    fread(C,8,1,inputFile);
    *pc+=10;
  }
    
  else if (*opCode == MRMOVQ){
    fread(currByte,1,1,inputFile);
    fill_ra_rb(currByte, regA, regB);
    fread(C,8,1,inputFile);
    *pc+=10;
  }
    
  else if (*opCode == CALL){
    fread(C,8,1,inputFile);
    *pc+=9;
  }

  else if (*opCode == RET){
    *pc+=1;
  }

  else if (*opCode == PUSHQ){
    fread(currByte,1,1,inputFile);
    *regA = *currByte >> 4;
    *pc+=2;
  }
      
  else if (*opCode == POPQ){
    fread(currByte,1,1,inputFile);
    *regA = *currByte >> 4;
    *pc+=2;
  }

  else if ((*opCode >> 4) == 0x02 && (*opCode & 0x0F) != 7){
    fread(currByte,1,1,inputFile);
    fill_ra_rb(currByte, regA, regB);
    *pc+=2;
  }

  else if ((*opCode >> 4) == 0x07 && (*opCode & 0x0F) != 7){
    fread(C,8,1,inputFile);
    *pc+=9;
  }
    
  else if ((*opCode >> 4) == 0x06 && (*opCode & 0x0F) != 7){
    fread(currByte,1,1,inputFile);
    fill_ra_rb(currByte, regA, regB);
    *pc+=2;
  }

  else if (*opCode == QUAD_WORD){
    fread(C,7,1,inputFile);
    *C = *C << 8;
    *C = *C | *currByte;
    *pc+=8;
  }

  else if (*opCode == BYTE_WORD){
    *C = *currByte;
    *pc+=1;
  }

  else {
    // this should never happen
    printf("ERROR!!! opCode: %x\n", *opCode);
  }
}