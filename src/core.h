#ifndef CORE_H
#define CORE_H
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#define MEM_SIZE 2000000

// Opcodes
#define OPCODE_LOAD   0x3
#define OPCODE_STORE  0x23
#define OPCODE_BRANCH 0x63
#define OPCODE_JALR   0x67
#define OPCODE_JAL    0x6f
#define OPCODE_OP_IMM 0x13
#define OPCODE_OP     0x33
#define OPCODE_SYSTEM 0x73
#define OPCODE_AUIPC  0x17
#define OPCODE_LUI    0x37

// Masks
#define GET_RD(x)  (x >> 7) & 0x1f
#define GET_RS1(x) (x >> 15) & 0x1f
#define GET_RS2(x) (x >> 20) & 0x1f
#define GET_OPCODE(x) (x & 0x7F)
#define GET_FUNCT3(x) (x >> 12) & 0x7
#define GET_FUNCT7(x) (x >> 25)

// Sign extend
#define SEXT8(x)  ((((int32_t) x) << 24) >> 24)
#define SEXT12(x) ((((int32_t) x) << 20) >> 20)
#define SEXT16(x) ((((int32_t) x) << 16) >> 16)
#define SEXT20(x) ((((int32_t) x) << 12) >> 12)

// Register names
#define SP 2



typedef struct {
  uint32_t PC;
  uint32_t reg[32];
  uint8_t mem[MEM_SIZE];
  int is_running;
  uint32_t last_address; //The last usefull address.
} machine_t;


// I know, what a waste of bits.
typedef struct {
    uint32_t opcode;
    uint32_t funct3;
    uint32_t funct7;
    uint32_t imm;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t rd;
} instruction_t;


uint32_t fetch(machine_t*);
uint32_t get_word(machine_t*, uint32_t);
instruction_t decode(machine_t*, uint32_t);
void execute(machine_t*, instruction_t*);

void cycle(machine_t*);

#endif
