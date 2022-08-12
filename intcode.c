#define _GNU_SOURCE
#include "./intcode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

// #define DEBUG

instructions parse(const char *file_path) {
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    int count = 0;

    char *buf;
    size_t len = 0;
    int read;
    while ((read = getdelim(&buf, &len, ',', f)) > 0) {
        if (read == 0) continue;
        if (errno != 0) {
            fprintf(stderr, "Error reading line: %s\n", strerror(errno));
            exit(1);
        }
        count++;
    }
    rewind(f);
    instruction *arr = malloc(count * sizeof(instruction));
    len = 0;
    read = 0;
    for (int pos = 0; (read = getdelim(&buf, &len, ',', f)) > 0; pos++) {
        if (read == 0) continue;
        if (errno != 0) {
            fprintf(stderr, "Error reading line: %s\n", strerror(errno));
            exit(1);
        }
        
        buf[read-1] = '\0';
        int i = atoi(buf);
        int value = i;
        opcode oc = (opcode)(i % 100);
        i /= 100;
        int *mask = malloc(10 * sizeof(mask));
        for (int ind = 0; i > 0; ind++) {
            mask[ind] = i % 10;
            i /= 10;
        }
        arr[pos] = (instruction){
            .code = oc,
            .mask = mask,
            .val = value,
        };
    }

    free(buf);
    fclose(f);

    instructions ret = {
        .arr = arr,
        .len = count,
    };
    
    return ret;
}

instructions clone(instructions insts) {
    instruction *arr = insts.arr;
    instruction *arr2 = malloc(insts.len * sizeof(instruction));
    for (int i = 0; i < insts.len; i++) {
        arr2[i] = arr[i];
    }

    instructions ret = {
        .arr = arr2,
        .len = insts.len,
    };
    return ret;
}

void set(instruction *arr, int index, int val) {
    int i = val;
    opcode oc = (opcode)(i % 100);
    i /= 100;
    int *mask = malloc(10 * sizeof(int));
    for (int ind = 0; i > 0; ind++) {
        mask[ind] = i % 10;
        i /= 10;
    }
    arr[index] = (instruction){
        .val = val,
        .code = oc,
        .mask = mask,
    };
}

static int get(instruction *arr, int ip, int off) {
    instruction inst = arr[ip];
    switch (inst.mask[off]) {
    case 0:
        return arr[arr[ip+off+1].val].val;
    case 1:
        return arr[ip+off+1].val;
    default:
        fprintf(stderr, "invalid mode `%d`, aborting...", inst.mask[off]);
        abort();
    }
}

int exec(instructions insts) {
    instruction *arr = insts.arr;
    int ip = 0;
    bool halted = false;
    while (! halted) {
        if (ip >= insts.len) {
            fprintf(stderr, "Overflow at position: %x, aborting...\n", ip);
            abort();
        }
        switch (arr[ip].code) {
        case ADD: {
            int a = get(arr, ip, 0); //arr[arr[ip+1]];
            int b = get(arr, ip, 1); //arr[arr[ip+2]];
            int res = arr[ip+3].val;
            set(arr, res, a + b);
            ip += 4;
#ifdef DEBUG
            printf("ADD: %d + %d -> %d\n", a, b, res);
#endif
            break;
        }
        case MUL: {
            int a = get(arr, ip, 0); //arr[arr[ip+1]];
            int b = get(arr, ip, 1); //arr[arr[ip+2]];
            int res = arr[ip+3].val;
            set(arr, res, a * b);
            ip += 4;
#ifdef DEBUG
            printf("MUL: %d * %d -> %d\n", a, b, res);
#endif
            break;
          }
        case INPUT: {
            char *buf = NULL;
            int read;
            size_t len = 0;
            read = getline(&buf, &len, stdin);
            if (read == -1) {
                printf("No line read...\n");
                exit(1);
            } else {
                buf[read-1] = '\0';
                int num = atoi(buf);
                int res = arr[ip+1].val;
                set(arr, res, num);
                ip += 2;
#ifdef DEBUG
            printf("INPUT: %d -> %d\n",  num, res);
#endif
            }
            break;
        }
        case OUTPUT: {
            int a = get(arr, ip, 0);
#ifdef DEBUG
            printf("OUT: %d\n", a);
#endif
            printf("%d\n", a);
            ip += 2;
            break;
        }
        case JIT: {
            int a = get(arr, ip, 0);
            int b = get(arr, ip, 1);
#ifdef DEBUG
            printf("JIT: %d -> %d\n", a, b);
#endif
            if (a != 0) {
                ip = b;
            } else {
                ip += 3;
            }
            break;
        }
        case JIF: {
            int a = get(arr, ip, 0);
            int b = get(arr, ip, 1);
#ifdef DEBUG
            printf("JIF: %d -> %d\n", a, b);
#endif
            if (a == 0) {
                ip = b;
            } else {
                ip += 3;
            }
            break;
        }
        case LT: {
            int a   = get(arr, ip, 0);
            int b   = get(arr, ip, 1);
            int res = arr[ip+3].val;
#ifdef DEBUG
            printf("LT: (%d < %d) -> %d\n", a, b, res);
#endif
            set(arr, res, (a < b));
            ip += 4;
            break;
        }
        case EQ: {
            int a   = get(arr, ip, 0);
            int b   = get(arr, ip, 1);
            int res = arr[ip+3].val;
#ifdef DEBUG
            printf("EQ: (%d == %d) -> %d\n", a, b, res);
#endif
            set(arr, res, (a == b));
            ip += 4;
            break;
        }
        case END:
#ifdef DEBUG
            printf("HALT\n");
#endif
            printf("halted\n");
            halted = true;
            break;
        default:
            fprintf(stderr, "Invalid instruction: %d, aborting...\n", arr[ip].code);
            abort();
            break;
        }
    }
    
    return arr[0].val;
}
