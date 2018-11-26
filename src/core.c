#include "frontend.h"
#include "core.h"

void initialize(machine_t* machine) {
    machine->reg[SP] = MEM_SIZE-1;
}

void cycle(machine_t* machine) {
  uint32_t raw = fetch(machine);
  instruction_t inst = decode(machine, raw);
  execute(machine, &inst);
  machine->reg[0] = 0;
  memory_access(machine);
  write_back(machine);
  if(machine->PC > machine->last_address
          && machine->last_address > 0) {
    machine->is_running = 0;
  }
}

uint32_t get_word(machine_t* machine, uint32_t addr) {
    return machine->mem[addr] |
        machine->mem[addr+1] << 8 |
        machine->mem[addr+2] << 16 |
        machine->mem[addr+3] << 24;
}

uint32_t fetch(machine_t* machine) {
    return get_word(machine, machine->PC);
}

instruction_t decode(machine_t* machine, uint32_t raw_inst) {

    instruction_t inst;
    inst.opcode = GET_OPCODE(raw_inst);
    inst.rd = GET_RD(raw_inst);
    inst.funct3 = GET_FUNCT3(raw_inst);
    inst.funct7 = GET_FUNCT7(raw_inst);
    inst.rs1 = GET_RS1(raw_inst);
    inst.rs2 = GET_RS2(raw_inst);

    if(inst.opcode == OPCODE_LUI || inst.opcode == OPCODE_AUIPC) {
        // auipc or lui (U)
        //inst.imm = SEXT20(raw_inst >> 12) << 12;
        inst.imm = raw_inst & (~0xFFF);

    } else if(inst.opcode == OPCODE_LOAD ||
            inst.opcode == OPCODE_OP_IMM ||
            inst.opcode == OPCODE_SYSTEM ||
            inst.opcode == OPCODE_JALR) {
        inst.imm = SEXT12(raw_inst >> 20);
    } else if(inst.opcode == OPCODE_STORE) {
        uint32_t t_imm = (raw_inst & (0xFE << 24))  >> 20 |
                         (raw_inst & 0xF80) >> 7;

        inst.imm = SEXT12(t_imm);
    } else if(inst.opcode == OPCODE_JAL) {
        uint32_t t_imm = (raw_inst & (0x1 << 31))  >> 11 |
                         (raw_inst & (0x3ff << 21)) >> 20 |
                         (raw_inst & (0x1 << 20))>> 9 |
                         (raw_inst & 0xFF000);

        inst.imm = SEXT20(t_imm >> 1) << 1;
    } else if(inst.opcode == OPCODE_BRANCH) {
        uint32_t t_imm = (raw_inst & (0x1 << 31))  >> 19 |
                         (raw_inst & (0x3f << 25)) >> 20 |
                         (raw_inst & (0x1 << 7))   <<  4 |
                         (raw_inst & (0xf << 8))   >>  7;

        inst.imm = SEXT12(t_imm >> 1) << 1;
    } else if(inst.opcode == OPCODE_OP) {
        // Nothing to do?
    }

    return inst;
}

void execute(machine_t* machine, instruction_t* inst) {
    if(inst->opcode == OPCODE_OP) {
        if(inst->funct3 == 0 && inst->funct7 == 0) {
            //add
            machine->reg[inst->rd] =
                machine->reg[inst->rs1] + machine->reg[inst->rs2];
        } else if(inst->funct3 == 0 && inst->funct7 == 0x20) {
            //sub
            machine->reg[inst->rd] =
                machine->reg[inst->rs1] - machine->reg[inst->rs2];
        } else if(inst->funct3 == 0x1) {
            //sll
            machine->reg[inst->rd] =
                machine->reg[inst->rs1] << machine->reg[inst->rs2];
        } else if(inst->funct3 == 0x2) {
            //slt
            int32_t rs1 = machine->reg[inst->rs1];
            int32_t rs2 = machine->reg[inst->rs2];
            machine->reg[inst->rd] = rs1 < rs2 ? 1 : 0;
        } else if(inst->funct3 == 0x3) {
            //sltu
            uint32_t rs1 = machine->reg[inst->rs1];
            uint32_t rs2 = machine->reg[inst->rs2];
            machine->reg[inst->rd] = rs1 < rs2 ? 1 : 0;
        } else if(inst->funct3 == 0x4) {
            //xor
            machine->reg[inst->rd] =
                machine->reg[inst->rs1] ^ machine->reg[inst->rs2];
        } else if(inst->funct3 == 0x5 && inst->funct7 == 0x0) {
            //srl
            machine->reg[inst->rd] =
                machine->reg[inst->rs1] >> machine->reg[inst->rs2];
        } else if(inst->funct3 == 0x5 && inst->funct7 == 0x20) {
            //sra
            machine->reg[inst->rd] =
                ((int32_t) machine->reg[inst->rs1])
                >> machine->reg[inst->rs2];
        } else if(inst->funct3 == 0x6) {
            // or
            machine->reg[inst->rd] = machine->reg[inst->rs1] |
                machine->reg[inst->rs2];
        } else if(inst->funct3 == 0x7) {
            // and
            machine->reg[inst->rd] = machine->reg[inst->rs1] &
                machine->reg[inst->rs2];
        }
        machine->PC += 4;
    } else if(inst->opcode == OPCODE_OP_IMM) {
        if(inst->funct3 == 0) {
            //addi
            machine->reg[inst->rd] =
                machine->reg[inst->rs1] + inst->imm;
        } else if(inst->funct3 == 1) {
            //slli
            machine->reg[inst->rd] = machine->reg[inst->rs1] << (inst->imm & 0x1f);
        } else if(inst->funct3 == 2) {
            //slti
            int32_t rs1 = machine->reg[inst->rs1];
            int32_t imm = inst->imm;
            machine->reg[inst->rd] = rs1 < imm ? 1 : 0;
        } else if(inst->funct3 == 3) {
            // sltiu
            uint32_t rs1 = machine->reg[inst->rs1];
            uint32_t imm = inst->imm;
            machine->reg[inst->rd] = rs1 < imm ? 1 : 0;
        } else if(inst->funct3 == 4) {
            // xori
            machine->reg[inst->rd] = machine->reg[inst->rs1] ^ inst->imm;
        } else if(inst->funct3 == 5 && inst->funct7 == 0) {
            // srli
            machine->reg[inst->rd] = machine->reg[inst->rs1]
                >> (inst->imm & 0x1f);
        } else if(inst->funct3 == 5 && inst->funct7 == 0x20) {
            // srai
            machine->reg[inst->rd] =
                ((int32_t) machine->reg[inst->rs1])
                >> (inst->imm & 0x1f);
        } else if(inst->funct3 == 6) {
            // ori
            machine->reg[inst->rd] = machine->reg[inst->rs1] | inst->imm;
        } else if(inst->funct3 == 7) {
            // andi
            machine->reg[inst->rd] = machine->reg[inst->rs1] & inst->imm;
        }
        machine->PC += 4;
    } else if(inst->opcode == OPCODE_BRANCH) {
        if(inst->funct3 == 0) {
            // beq
            if( machine->reg[inst->rs1] == machine->reg[inst->rs2]) {
                machine->PC += (int32_t) inst->imm;
            } else {
                machine->PC += 4;
            }
        } else if(inst->funct3 == 1) {
            // bne
            if( machine->reg[inst->rs1] != machine->reg[inst->rs2]) {
                machine->PC += (int32_t) inst->imm;
            } else {
                machine->PC += 4;
            }
        } else if(inst->funct3 == 4) {
            // blt
            if( (int32_t) machine->reg[inst->rs1] < (int32_t) machine->reg[inst->rs2]) {
                machine->PC += (int32_t) inst->imm;
            } else {
                machine->PC += 4;
            }
        } else if(inst->funct3 == 5) {
            // bge
            if( (int32_t) machine->reg[inst->rs1] >= (int32_t) machine->reg[inst->rs2]) {
                machine->PC += (int32_t) inst->imm;
            } else {
                machine->PC += 4;
            }
        } else if(inst->funct3 == 6) {
            // bltu
            if( machine->reg[inst->rs1] < machine->reg[inst->rs2]) {
                machine->PC += (int32_t) inst->imm;
            } else {
                machine->PC += 4;
            }
        } else if(inst->funct3 == 7) {
            // bgeu
            if( machine->reg[inst->rs1] >= machine->reg[inst->rs2]) {
                machine->PC += (int32_t) inst->imm;
            } else {
                machine->PC += 4;
            }
        }
        else {
            machine->PC += 4;
        }
    } else if(inst->opcode == OPCODE_STORE) {
        uint32_t address = machine->reg[inst->rs1]+inst->imm;
        if(inst->funct3 == 0) {
            //SB
            machine->mem[address] = (machine->reg[inst->rs2] & 0xff);
        } else if(inst->funct3 == 1) {
            //SH
            machine->mem[address] = machine->reg[inst->rs2] & 0xff;
            machine->mem[address+1] = (machine->reg[inst->rs2] >> 8) & 0xff;
        } else if(inst->funct3 == 2) {
            //SW
            machine->mem[address] = machine->reg[inst->rs2] & 0xff;
            machine->mem[address+1] = (machine->reg[inst->rs2] >> 8) & 0xff;
            machine->mem[address+2] = (machine->reg[inst->rs2] >> 16) & 0xff;
            machine->mem[address+3] = (machine->reg[inst->rs2] >> 24) & 0xff;
        }
        machine->PC += 4;
    } else if(inst->opcode == OPCODE_LOAD) {
        uint32_t address = machine->reg[inst->rs1]+inst->imm;
        uint32_t w = get_word(machine, address);
       if(inst->funct3 == 0) {
            //LB
            machine->reg[inst->rd] = SEXT8(w);
       } else if(inst->funct3 == 1) {
            //LH
            machine->reg[inst->rd] = SEXT16(w);
       } else if(inst->funct3 == 2) {
            //LW
            machine->reg[inst->rd] = w;
       } else if(inst->funct3 == 4) {
            //LBU
            machine->reg[inst->rd] = w & 0xFF;
       } else if(inst->funct3 == 5) {
            //LHU
            machine->reg[inst->rd] = w & 0xFFFF;
       }
       machine->PC += 4;
    } else if(inst->opcode == OPCODE_SYSTEM &&
            inst->funct3 == 0 && inst->funct7 == 0) {
        //ecall
        machine->is_running = 0;
    } else if(inst->opcode == OPCODE_LUI) {
        //lui
        machine->reg[inst->rd] = inst->imm;
        machine->PC += 4;
    } else if(inst->opcode == OPCODE_JAL) {
        //jal
        machine->reg[inst->rd] = machine->PC + 4;
        machine->PC += inst->imm;
    } else if(inst->opcode == OPCODE_JALR) {
        //jalr
        machine->reg[inst->rd] = machine->PC;
        machine->PC = (inst->imm + machine->reg[inst->rs1]) & (~0x1);
    } else if(inst->opcode == OPCODE_AUIPC) {
        // auipc
        machine->reg[inst->rd] = machine->PC + inst->imm;
        machine->PC += 4;
    } else {
        machine->PC += 4;
    }
}

void memory_access(machine_t* machine) {

}

void write_back(machine_t* machine) {

}

