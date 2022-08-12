#ifndef __INTCODE_H_
#define __INTCODE_H_

typedef enum {
  ADD    = 1,
  MUL    = 2,
  INPUT  = 3,
  OUTPUT = 4,
  JIT    = 5,
  JIF    = 6,
  LT     = 7,
  EQ     = 8,
  END    = 99,
} opcode;

typedef struct {
    int val;
    opcode code;
    int *mask;
} instruction;

typedef struct {
    instruction *arr;
    int len;
} instructions;

instructions parse(const char *file_path);
instructions clone(instructions insts);
int exec(instructions insts);
void set(instruction *arr, int index, int val);

#endif //__INTCODE_H_
