#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>

#define max(a, b) ((a>b)?a:b)

__attribute_pure__ int calc1(int i) { return i / 3 - 2; }

__attribute_pure__ int calc2(int i) {
    if (i == 0) return 0;
    if (i < 0) assert(false && "unreachable!");
    
    int res = max(0, calc1(i));
    return res + calc2(res);
}

void solve(const char *file_path) {
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    int part1 = 0, part2 = 0;
    
    char *buf;
    size_t len = 0;
    int read;
    while ((read = getline(&buf, &len, f)) > 0) {
        if (read == 0) continue;
        if (errno != 0) {
            fprintf(stderr, "Error reading line: %s\n", strerror(errno));
            exit(1);
        }
        buf[read-1] = '\0';
        int i = atoi(buf);
        part1 += calc1(i);
        part2 += calc2(i);
    }
    
    free(buf);
    fclose(f);

    printf("Part 1: %d\nPart2 : %d\n", part1, part2);
}

int main() {
    solve("./input.txt");
}
