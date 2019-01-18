#ifndef _Y86_H_
#define _Y86_H_

// ifun definitions
#define NO_CON 0x0
#define LE 0x1
#define L 0x2
#define E 0x3
#define NE 0x4
#define GE 0x5
#define G 0x6
#define ADD_FLAG 0x0
#define SUB_FLAG 0x1
#define AND_FLAG 0x2
#define XOR_FLAG 0x3
#define MUL_FLAG 0x4
#define DIV_FLAG 0x5
#define MOD_FLAG 0x6

// op code definitions
#define CMOV_BASE 0x20
#define JMPXX_BASE 0x70
#define OP_BASE 0x60

#define HALT 0x00
#define NOP 0x10
#define RRMOVQ 0x20
#define IRMOVQ 0x30
#define RMMOVQ 0x40
#define MRMOVQ 0x50
#define CALL 0x80
#define RET 0x90
#define PUSHQ 0xA0
#define POPQ 0xB0
#define CMOVLE (CMOV_BASE + LE)
#define CMOVL (CMOV_BASE + L)
#define CMOVE (CMOV_BASE + E)
#define CMOVNE (CMOV_BASE + NE)
#define CMOVGE (CMOV_BASE + GE)
#define CMOVG (CMOV_BASE + G)
#define JMP (JMPXX_BASE + NO_CON)
#define JLE (JMPXX_BASE + LE)
#define JL (JMPXX_BASE + L)
#define JE (JMPXX_BASE + E)
#define JNE (JMPXX_BASE + NE)
#define JGE (JMPXX_BASE + GE)
#define JG (JMPXX_BASE + G)
#define ADDQ (OP_BASE + ADD_FLAG)
#define SUBQ (OP_BASE + SUB_FLAG)
#define ANDQ (OP_BASE + AND_FLAG)
#define XORQ (OP_BASE + XOR_FLAG)
#define MULQ (OP_BASE + MUL_FLAG)
#define DIVQ (OP_BASE + DIV_FLAG)
#define MODQ (OP_BASE + MOD_FLAG)


#endif