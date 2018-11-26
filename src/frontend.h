#ifndef FRONTEND_H
#define FRONTEND_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "core.h"
#define FILE_MAX 1024

typedef struct settings_s {
  char filename_program[FILE_MAX];
  char filename_regs[FILE_MAX];
  char filename_out[FILE_MAX];
  int is_interactive;
  int use_stack;
  machine_t machine;
} settings_t;

int load_settings(int argc, char** argv);
int load_program();
int load_regs();

void print_machine(machine_t*);
void inst_string(instruction_t, char* s);

int write_out(machine_t*);

#endif
