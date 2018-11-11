#ifndef CORE_H
#define CORE_H
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#define MEM_SIZE 1000000

typedef struct {
  uint32_t PC;
  uint32_t reg[32];
  uint8_t mem[MEM_SIZE];
  int is_running;
  uint32_t last_address; //The last usefull address.
} machine_t;

typedef enum {R,I, S, SB, U, UJ} instruction_format;

// I know, what a waste of bits.
typedef struct {
    uint32_t opcode;
    uint32_t funct3;
    uint32_t funct7;
    uint32_t imm;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t rd;
    instruction_format format;
} instruction_t;

uint32_t fetch(machine_t*);
instruction_t decode(machine_t*, uint32_t);
void execute(machine_t*, instruction_t*);
void memory_access(machine_t*);
void write_back(machine_t*);

void cycle(machine_t*);

uint32_t get_word(machine_t*, uint32_t);

uint32_t sign_ext11(uint32_t number);
uint32_t sign_ext20(uint32_t number);

#endif
