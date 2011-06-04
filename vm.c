#include "vm.h"

/* 
 * Emulate the instruction cycle in a machine.
 * The available memory has been dynamically allocated in vm->mem.
 * It is the responsibility of the caller to free this memory.
 * The following abbreviations are used throughout the implementation:
 *   PC  - Program Counter
 *   MAR - Memory Address Register
 *   MBR - Memory Buffer Register
 */
int run(const VM *vm, const char *instr, const int instr_len) {
  int io, nest;
  unsigned int mar = 0, pc;
  for(pc = 0; pc < instr_len; pc++) {
    switch(instr[pc]) {
    case '>':
      mar++;
      break;
    case '<':
      mar--;
      break;
    case '+':
      if(mar < 0 || vm->mem_size <= mar) {
        return 0;
      }
      vm->mem[mar]++;
      break;
    case '-':
      if(mar < 0 || vm->mem_size <= mar) {
        return 0;
      }
      vm->mem[mar]--;
      break;
    case '.':
      io = fputc(vm->mem[mar], vm->out);
      if(io == EOF) {
        return 0;
      }
      break;
    case ',':
      io = fgetc(vm->in);
      if(io == EOF) {
        return 0;
      }
      break;
    case '[':
      if (!vm->mem[mar]) {
        for(nest = 1; nest > 0; pc++) {
          switch(instr[pc+1]) {
          case ']':
            nest--;
            break;
          case '[':
            nest++;
            break;
          }
        }
      }
      break;
    case ']':
      if (vm->mem[mar]) {
        for(nest = -1; nest < 0; pc--) {
          switch(instr[pc-1]) {
          case ']':
            nest--;
            break;
          case '[':
            nest++;
            break;
          }
        }
      }
      break;
    }
  }

  return 1;
}

