#ifndef _INSTRUCTION_DECODE_H_
#define _INSTRUCTION_DECODE_H_

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void instructionDecode(uint8_t* , uint8_t* , uint8_t* , uint8_t* , uint64_t* , uint64_t* , FILE* );

#endif