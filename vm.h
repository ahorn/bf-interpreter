#ifndef _VM_H
#define _VM_H 1

#include <stdio.h>

typedef struct VM VM;
struct VM {
  /* Perform I/O with . and , instructions */
  FILE *in;
  FILE *out;

  /* Manipulate available slots with >, <, + and - instructions */
  unsigned int mem_size;
  int *mem;
};

/* Interpret BF instructions in instr[0..instr_len] */
int run(const VM *vm, const char *instr, const size_t instr_len);
#endif
