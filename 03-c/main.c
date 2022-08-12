#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#define BUF_CAP 500

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction;

typedef struct  {
    direction dir;
    int len;
} step;

typedef step *path;

path *parse(const char *file_path) {
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    char *buf;
    path *arr = malloc(2 * sizeof(path));
    arr[0] = malloc(BUF_CAP * sizeof(step));
    arr[1] = malloc(BUF_CAP * sizeof(step));
    
    size_t len = 0;
    int read = 0;
    for (int lnum = 0; (read = getline(&buf, &len, f)) > 0; lnum++) {
        if (read == 0) continue;
        if (errno != 0) {
            fprintf(stderr, "Error reading line: %s\n", strerror(errno));
            exit(1);
        }
        buf[read-1] = '\0';
        
        int index = 0;
        direction current_dir;
        for (size_t i = 0; i < strlen(buf); i++) {
            switch (buf[i]) {
            case 'U':
                current_dir = UP;
                break;
            case 'D':
                current_dir = DOWN;
                break;
            case 'L':
                current_dir = LEFT;
                break;
            case 'R':
                current_dir = RIGHT;
                break;
            default:
                printf("'%c' is not a valid direction\n", buf[i]);
                exit(1);
                break;
            }
            i++;
            char num_buf[5];
            int num_pos = 0;
            for (;buf[i] != ',' && buf[i] != '\0'; num_pos++, i++) {
                num_buf[num_pos] = buf[i];
            }
            
            num_buf[num_pos] = '\0';
            arr[lnum][index++] = (step){
                .dir = current_dir,
                .len = atoi(num_buf)
            };
        }
        arr[lnum][index].len = 0;
    }

    free(buf);
    fclose(f);

    return arr;
}

typedef struct {
    int x;
    int y;
} pos;

int main() {
    path *paths = parse("./input.txt");
    pos positions[2][200000];
    int pos_index[2] = {0};

    for (int path = 0; path < 2; path++) {
        int x = 0, y = 0;
        
        for (int i = 0; paths[path][i].len != 0; i++) {
            fflush(stdout);
            int len = paths[path][i].len;
            switch (paths[path][i].dir) {
            case UP:
                for (int i = 1; i <= len; i++) {
                    positions[path][pos_index[path]++] = (pos){x, y-i};
                }
                y -= len;
                break;
            case DOWN:
                for (int i = 1; i <= len; i++) {
                    positions[path][pos_index[path]++] = (pos){x, y+i};
                }
                y += len;
                break;
            case LEFT:
                for (int i = 1; i <= len; i++) {
                    positions[path][pos_index[path]++] = (pos){x-i, y};
                }
                x -= len;
                break;
            case RIGHT:
                for (int i = 1; i <= len; i++) {
                    positions[path][pos_index[path]++] = (pos){x+i, y};
                }
                x += len;
                break;
            }
        }
    }

    size_t min = ULONG_MAX;
    for (int i = 0; i < pos_index[0]; i++) {
        if (i % 200 == 0) {
            printf("\r                       \r%d / %d", i, pos_index[0]);
            fflush(stdout);
        }
        for (int j = 0; j < pos_index[1]; j++) {
            if (positions[0][i].x == positions[1][j].x &&
                positions[0][i].y == positions[1][j].y) {
                size_t dist = abs(positions[0][i].x) + abs(positions[1][j].y);
                if (dist < min) min = dist;
            }
        }
    }
    printf("\r                             \r%d / %d\n", pos_index[0], pos_index[0]);

    printf("Part 1: %ld\n", min);


    min = ULONG_MAX;
    for (int i = 0; i < pos_index[0]; i++) {
        if (i % 200 == 0) {
            printf("\r                       \r%d / %d", i, pos_index[0]);
            fflush(stdout);
        }
        for (int j = 0; j < pos_index[1]; j++) {
            if (positions[0][i].x == positions[1][j].x &&
                positions[0][i].y == positions[1][j].y) {
                int a_dist = 0, b_dist = 0;
                for (int a = 0; a < pos_index[0]; a++) {
                    if (positions[0][a].x == positions[0][i].x &&
                        positions[0][a].y == positions[0][i].y) {
                        a_dist = a+1;
                    }
                }
                for (int b = 0; b < pos_index[1]; b++) {
                    if (positions[1][b].x == positions[1][j].x &&
                        positions[1][b].y == positions[1][j].y) {
                        b_dist = b+1;
                    }
                }
                
                size_t dist = a_dist + b_dist;
                if (dist < min) min = dist;
            }
        }
    }
    printf("\r                             \r%d / %d\n", pos_index[0], pos_index[0]);

    printf("Part 2: %ld\n", min);    
}
