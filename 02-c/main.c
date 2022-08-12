#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include "../intcode.h"

int main() {
    instructions insts = parse("./input.txt");
    instructions insts1 = clone(insts);
    set(insts1.arr, 1, 12);
    set(insts1.arr, 2, 2);
    printf("Part 1: %d\n", exec(insts1));
    free(insts1.arr);

    for (int n = 1; n < 99; n++) {
        for (int v = 1; v < 99; v++) {
            instructions insts2 = clone(insts);
            set(insts1.arr, 1, n);
            set(insts1.arr, 2, v);

            if (exec(insts2) == 19690720) {
                printf("Part 2: %d\n", 100 * n + v);
                exit(0);
            }
            
            free(insts2.arr);
        }
    }
}
