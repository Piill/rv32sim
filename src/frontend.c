#include "frontend.h"

settings_t settings;
int main(int argc, char** argv) {
    load_settings(argc, argv);
    load_regs();
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
    while ((c = getopt (argc, argv, "f:r:o:i")) != -1) {
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
    return 0;
}

int load_program() {
    FILE* fp = fopen(settings.filename_program, "rb");
    if(fp == NULL) {
        printf("Error, file %s could not be opened\n", settings.filename_program);
        exit(-1);
    }

    uint8_t byte;
    int i;
    for(i = 0; i < MEM_SIZE && (byte = fgetc(fp)) != EOF; i++) {
        settings.machine.mem[i] = byte;
    }
    settings.machine.last_address = i;
    return 0;
}


void print_machine(machine_t* machine) {
    //    printf("\033[0;0H");
    //printf("\e[1;1H\e[2J");
    printf("\n\n\n\n");
    printf("PC: %x\n", machine->PC);
    printf("Memory             \t Regs \n");
    int addr = 0;
    for(int i = 0; i < 32; i++) {
        char line_start = ' ';
        if(addr == machine->PC) {
            line_start = '>';
        }

        printf("%c %02x : 0x%08x \t %d: %d\n",
                line_start, addr,
                get_word(machine, addr),
                i, machine->reg[i]);

        addr += 4;
    }
}

void print_inst(instruction_t* inst) {
    printf("Opcode: %x\n", inst->opcode);
    switch (inst->format) {
        case I:
            printf("Type: I\n");
            printf("rd: %d\n", inst->rd);
            printf("rs: %d\n", inst->rs1);
            printf("Imm: %d\n", inst->imm);
            printf("funct3: %x\n", inst->funct3);
            break;
        case R:
            printf("Type: R\n");
            printf("rd: %d\n", inst->rd);
            printf("rs1: %d\n", inst->rs1);
            printf("rs2: %d\n", inst->rs2);
            printf("funct3: %x\n", inst->funct3);
            printf("funct7: %x\n", inst->funct7);
            break;
        case U:
            printf("Type: U\n");
            printf("rd: %d\n", inst->rd);
            printf("Imm: %d\n", inst->imm);
            break;
        default:
            printf("Unknown instuction format\n");
    }
}


int write_out(machine_t* machine) {
    if(settings.filename_out[0] == '\0') {
        return -1;
    }
    FILE* fp = fopen(settings.filename_out, "w");
    if(fp == NULL) {
        printf("Error, file %s could not be opened\n", settings.filename_out);
        char buf[1024];
        getcwd(buf, 1024);
        printf("cwd: %s\n", buf);
        exit(-1);
    }

    for(int i = 0; i < 32; i++) {
        fprintf(fp, "%d: %d\n", i, settings.machine.reg[i]);
    }
    return 0;
}
