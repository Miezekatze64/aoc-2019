#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include "../intcode.h"

int main() {
    fprintf(stderr, "Enter 1 for 'Part 1', 5 for 'Part 2': ");
    instructions insts = parse("./input.txt");
    exec(insts);
}
