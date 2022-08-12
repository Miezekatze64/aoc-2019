#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>

#define MIN 256310
#define MAX 732736

bool is_valid(int a, bool part) {
    if (a < 100000 || a > 999999) return false;
    
    bool has_same = false;
    int current = a;
    int last_digit = -1;
    for (int i = 0; i < 6; i++) {
        if (current % 10 == last_digit) {
            if (part) {
                if (i < 5) {
                    if ((current / 10) % 10 == last_digit) {
                        has_same = false;
                        while ((i < 6) && (current % 10 == last_digit)) {
                            last_digit = current % 10;
                            current /= 10;
                            i++;
                        }
                    } else {
                        has_same = true;
                        break;
                    }
                } else {
                    has_same = true;
                    break;
                }
            } else {
                has_same = true;
                break;
            }
        }
        last_digit = current % 10;
        current /= 10;
    }
    if (!has_same) return false;

    
    current = a;
    last_digit = 10;
    for (int i = 0; i < 6; i++) {
        if (current % 10 <= last_digit) {
            last_digit = current % 10;
            current /= 10;
        } else {
            return false;
        }
    }

    return true;
}

int main() {
    int p1 = 0, p2 = 0;
    for (int i = MIN; i <= MAX; i++) {
        if (is_valid(i, 0)) p1++;
        if (is_valid(i, 1)) p2++;
    }
    printf("Part 1: %d\n", p1);
    printf("Part 2: %d\n", p2);
}
