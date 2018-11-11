#include "frontend.h"
#include "core.h"

void cycle(machine_t* machine) {
  uint32_t raw = fetch(machine);
  instruction_t inst = decode(machine, raw);
  execute(machine, &inst);
  memory_access(machine);
  write_back(machine);
  machine->PC += 4;
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
    inst.opcode = raw_inst & 0x7F;
    inst.rd = (raw_inst >> 7) & 0x1f;
    inst.funct3 = (raw_inst >> 12) & 0x7;
    inst.rs1 = (raw_inst >> 15) & 0x1f;
    inst.rs2 = (raw_inst >> 20) & 0x1f;

    if(inst.opcode == 0x17 || inst.opcode == 0x37) {
        // auipc or lui (U)
        inst.format = U;
        inst.imm = sign_ext20(raw_inst >> 12);

    } else if((inst.opcode & 0x20) == 0 ||
            (inst.opcode & 0x70) == 0x70) {
        inst.format = I;
        inst.imm = sign_ext11(raw_inst >> 20);
    } else if(inst.opcode == 0x23) {
        inst.format = S;
        //Skip for now
    } else if(inst.opcode == 0x6f) {
        inst.format = UJ;
        //Skip for now
    } else if(inst.opcode == 0x63) {
        inst.format = SB;
        //Skip for now
    } else {
        inst.format = R;
        inst.funct7 = raw_inst >> 25;
    }

    print_inst(&inst);
    return inst;
}

void execute(machine_t* machine, instruction_t* inst) {
    if(inst->opcode == 0x33) {
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
                    ((int32_t) machine->reg[inst->rs1]) >> machine->reg[inst->rs2];
            }
    } else if(inst->opcode == 0x13 && inst->funct3 == 0) {
        //addi
        machine->reg[inst->rd] =
            machine->reg[inst->rs1] + inst->imm;
    } else if(inst->opcode == 0x73 &&
            inst->funct3 == 0 && inst->funct7 == 0) {
        //ecall
        machine->is_running = 0;
    } else if(inst->opcode == 0x37) {
        //lui
        machine->reg[inst->rd] = (inst->imm << 12);
    }
}

void memory_access(machine_t* machine) {

}

void write_back(machine_t* machine) {

}

uint32_t sign_ext11(uint32_t number) {
    number &= 0xFFF;
    if((number & 0x800) != 0) {
        number |= ~0xFFF;
    }
    return number;
}

uint32_t sign_ext20(uint32_t number) {
    number &= 0xFFFFF;
    if((number & 0x80000) != 0) {
        number |= ~0xFFFFF;
    }
    return number;
}
