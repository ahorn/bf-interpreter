/* BF interpreter */

#include <stdio.h>
#include <stdlib.h> 
#include <stdarg.h> 
#include "vm.h"

/* Default number of available integer slots */
#define BF_MEM_SIZE 300

static const char bf_usage[] = "bf [-m <available-slots>] <file>\n"
                               "Options:\n"
                               " -m" "     " "Allocate specified number of slots for integers";

static void error(const char *err, ...)
{
  va_list params;

  va_start(params, err);
  vfprintf(stderr, err, params);
  fprintf(stderr, "\n");
  va_end(params);
}

static void usage(const char *err)
{
  error("Usage: %s", err);
  exit(1);
}

/* signed int in case the command line option is a negative number */
static int mem_size = BF_MEM_SIZE;
static char* filename = NULL;

/* Parse command line argument */
static void parse_args(int argc, char **argv)
{
  char *arg;
  while(argc > 0) {
    arg = *argv;

    if('-' == arg[0]) {
      if('m' == arg[1] && argc > 0) {
        argv++;
        argc--;

        mem_size = atoi(*argv);
      } else {
        usage(bf_usage);
      }
    } else {
      filename = arg;
    }

    argv++;
    argc--;
  }
}

/* Read an entire file as a string. Returns the length of the string. */
unsigned int read(FILE *file, char **buffer)
{
  size_t size;

  fseek(file, 0, SEEK_END);
  size = ftell(file);
  rewind(file);

  // add an extra slot for '\0'
  *buffer = (char *)malloc(size+1);
  if (!*buffer) {
    fclose(file);
    return 0;
  }

  fread(*buffer, size, sizeof(char), file);
  fclose(file);

  // fread() operates on binary files
  buffer[size] = '\0';

  return size;
}

/* Interpret filename argument as BF program */
int main (int argc, char **argv)
{
  argv++;
  argc--;
  parse_args(argc, argv);

  if(filename == NULL || mem_size <= 0) {
    usage(bf_usage);
  }

  FILE *file = fopen(filename, "r");
  if(file == NULL) {
    error("failed to open %s", filename);
    return EXIT_FAILURE;
  }

  int *mem = malloc(mem_size * sizeof(int));
  const VM vm = { stdin, stdout, mem_size, mem };
  size_t instr_len;
  char *instr;
  instr_len = read(file, &instr);
  if(instr_len == 0) {
    return EXIT_FAILURE;
  }

  const int ok = run(&vm, instr, instr_len);

  free(instr);
  free(mem);

  if(!ok) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
