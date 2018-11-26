#include "frontend.h"

settings_t settings;
int main(int argc, char** argv) {
    load_settings(argc, argv);
    load_regs();
    initialize(&settings.machine);
    load_program();


    while(settings.machine.is_running) {
        if(settings.is_interactive) {
            print_machine(&settings.machine);
            getchar();
        }
        cycle(&settings.machine);
    }
    write_out(&settings.machine);
}

int load_settings(int argc, char** argv) {
    int c;
    while ((c = getopt (argc, argv, "f:r:o:is")) != -1) {
        switch (c)
        {
            case 'f':
                if(optarg != NULL) {
                    strcpy(settings.filename_program, optarg);
                } else {
                    printf("No filename specified\n");
                    return -1;
                }
                break;
            case 'r':
                if(optarg != NULL) {
                    strcpy(settings.filename_regs, optarg);
                } else {
                    printf("No filename specified\n");
                    return -1;
                }
                break;
            case 'o':
                if(optarg != NULL) {
                    strcpy(settings.filename_out, optarg);
                } else {
                    printf("No filename specified\n");
                    return -1;
                }
                break;
            case 'i':
                settings.is_interactive = 1;
                break;
            case 's':
                settings.use_stack = 1;
                break;
        }
    }
    settings.machine.is_running = 1;
    return 0;
}

int load_regs() {
    if(settings.filename_regs[0] == '\0') {
        return -1;
    }
    FILE* fp = fopen(settings.filename_regs, "r");
    if(fp == NULL) {
        printf("Error, file %s could not be opened\n", settings.filename_regs);
        exit(-1);
    }

    uint32_t r;
    uint32_t data;
    while(fscanf(fp, "%d:%x\n", &r, &data) != EOF) {
        settings.machine.reg[r] = data;
    }
    fclose(fp);
    return 0;
}

int load_program() {
    FILE* fp = fopen(settings.filename_program, "rb");
    if(fp == NULL) {
        printf("Error, file %s could not be opened\n", settings.filename_program);
        exit(-1);
    }

    settings.machine.last_address = fread(settings.machine.mem, sizeof(uint8_t), MEM_SIZE, fp);
    fclose(fp);
    return 0;
}


void print_machine(machine_t* machine) {
    printf("\033[0;0H"); // Clear screen
    printf("\e[1;1H\e[2J"); // Set cursor to coordinate (0,0)

    printf("PC: %x (of %x) \t %x\n", machine->PC, machine->last_address, fetch(machine) );
    printf("Memory             \t\t\t Regs \n");


    uint32_t addr = machine->PC;
    for(int i = 0; i < 32; i++) {
        char line_start = ' ';
        if(addr == machine->PC) {
            line_start = '>';
        }
        char inst_s[255];
        instruction_t inst = decode(machine,
                get_word(machine, addr));
        inst_string(inst, inst_s);
        printf("%c %3x : %-30s \t %d: %x\n",
                line_start, addr,
                inst_s,i, machine->reg[i]);

        addr += 4;
    }
}

void inst_string(instruction_t inst, char* s) {
    char opcode[10];
    if(inst.opcode == OPCODE_OP) {
        if(inst.funct3 == 0 && inst.funct7 == 0) {
            //add
            strcpy(opcode, "add");
        } else if(inst.funct3 == 0 && inst.funct7 == 0x20) {
            //sub
            strcpy(opcode, "sub");
        } else if(inst.funct3 == 0x1) {
            //sll
            strcpy(opcode, "sll");
        } else if(inst.funct3 == 0x2) {
            //slt
            strcpy(opcode, "slt");
        } else if(inst.funct3 == 0x3) {
            //sltu
            strcpy(opcode, "sltu");
        } else if(inst.funct3 == 0x4) {
            //xor
            strcpy(opcode, "xor");
        } else if(inst.funct3 == 0x5 && inst.funct7 == 0x0) {
            //srl
            strcpy(opcode, "srl");
        } else if(inst.funct3 == 0x5 && inst.funct7 == 0x20) {
            //sra
            strcpy(opcode, "sra");
        } else if(inst.funct3 == 0x6) {
            strcpy(opcode, "or");
        } else if(inst.funct3 == 0x7) {
            strcpy(opcode, "and");
        }
        sprintf(s, "%-5s r[%2d], r[%2d], r[%2d]",
                opcode, inst.rd, inst.rs1, inst.rs2);

    } else if(inst.opcode == OPCODE_OP_IMM) {
        if(inst.funct3 == 0) {
            //addi
            strcpy(opcode, "addi");
        } else if(inst.funct3 == 1) {
            //slli
            strcpy(opcode, "slli");
        } else if(inst.funct3 == 2) {
            //slti
            strcpy(opcode, "slti");
        } else if(inst.funct3 == 3) {
            // sltiu
            strcpy(opcode, "sltiu");
        } else if(inst.funct3 == 4) {
            // xori
            strcpy(opcode, "xori");
        } else if(inst.funct3 == 5 && inst.funct7 == 0) {
            // srli
            strcpy(opcode, "srli");
        } else if(inst.funct3 == 5 && inst.funct7 == 0x20) {
            // srai
            strcpy(opcode, "srai");
        } else if(inst.funct3 == 6) {
            // ori
            strcpy(opcode, "ori");
        } else if(inst.funct3 == 7) {
            // andi
            strcpy(opcode, "andi");
        }
        sprintf(s, "%-5s r[%2d], r[%2d], %8d",
                opcode, inst.rd, inst.rs1, inst.imm);
    } else if(inst.opcode == OPCODE_BRANCH) {
        if(inst.funct3 == 0) {
            // beq
            strcpy(opcode, "beq");
        } else if(inst.funct3 == 1) {
            // bne
            strcpy(opcode, "bne");
        } else if(inst.funct3 == 4) {
            // blt
            strcpy(opcode, "blt");
        } else if(inst.funct3 == 5) {
            // bge
            strcpy(opcode, "bge");
        } else if(inst.funct3 == 6) {
            // bltu
            strcpy(opcode, "bltu");
        } else if(inst.funct3 == 7) {
            // bgeu
            strcpy(opcode, "bgeu");
        }
        sprintf(s, "%-5s r[%2d], r[%2d], %8d",
                opcode, inst.rs1, inst.rs2, inst.imm);

    } else if(inst.opcode == OPCODE_STORE) {
        if(inst.funct3 == 0) {
            //SB
            strcpy(opcode, "sb");
        } else if(inst.funct3 == 1) {
            //SH
            strcpy(opcode, "sh");
        } else if(inst.funct3 == 2) {
            //SW
            strcpy(opcode, "sw");
        }
        sprintf(s, "%-5s r[%2d], r[%2d], %8d",
                opcode, inst.rs1, inst.rs2, inst.imm);
    } else if(inst.opcode == OPCODE_LOAD) {
       if(inst.funct3 == 0) {
            //LB
            strcpy(opcode, "lb");
       } else if(inst.funct3 == 1) {
            //LH
            strcpy(opcode, "lh");
       } else if(inst.funct3 == 2) {
            //LW
            strcpy(opcode, "lw");
       } else if(inst.funct3 == 4) {
            //LBU
            strcpy(opcode, "lbu");
       } else if(inst.funct3 == 5) {
            //LHU
            strcpy(opcode, "lhu");
       }
        sprintf(s, "%-5s r[%2d], r[%2d], %8d",
                opcode, inst.rd, inst.rs1, inst.imm);
    } else if(inst.opcode == OPCODE_SYSTEM &&
            inst.funct3 == 0 && inst.funct7 == 0) {
        //ecall
        strcpy(opcode, "ecall");
        sprintf(s, "%-5s", opcode);
    } else if(inst.opcode == OPCODE_LUI) {
        //lui
        strcpy(opcode, "lui");
        sprintf(s, "%-5s r[%d], %x", opcode, inst.rd, inst.imm);
    } else if(inst.opcode == OPCODE_JAL) {
        strcpy(opcode, "jal");
        sprintf(s, "%-5s %x", opcode, inst.imm);
    } else if(inst.opcode == OPCODE_JALR) {
        strcpy(opcode, "jalr");
        sprintf(s, "%-5s r[%d], %x(r[%d])", opcode, inst.rd, inst.imm, inst.rs1);
    } else if(inst.opcode == OPCODE_AUIPC) {
        strcpy(opcode, "auipc");
        sprintf(s, "%-5s r[%d], %x", opcode, inst.rd, inst.imm);
    } else {
        sprintf(s, "Unknown");
    }

}

int write_out(machine_t* machine) {
    if(settings.filename_out[0] == '\0') {
        return -1;
    }
    if(settings.use_stack == 1) {
        settings.machine.reg[SP] = 0;
    }
    FILE* fp = fopen(settings.filename_out, "wb");
    if(fp == NULL) {
        printf("Error, file %s could not be opened, with errno: %d\n", settings.filename_out, errno);
        char buf[1024];
        getcwd(buf, 1024);
        printf("cwd: %s\n", buf);
        exit(-1);
    }

    fwrite(settings.machine.reg, sizeof(uint32_t), 32, fp);
    fclose(fp);
    return 0;
}
