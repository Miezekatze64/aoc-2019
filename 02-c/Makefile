CFLAGS=-O3 -Wall -Wextra -Wformat=2 -W -Werror -std=c99 -pedantic
LDFLAGS=
CC=gcc
INPUTS=main.c ../intcode.c
OUTPUT=main

${OUTPUT}: ${INPUTS}
	${CC} ${INPUTS} -o ${OUTPUT} ${CFLAGS} ${LDFLAGS} ${OPTIONS}

.PHNOY run:
run: ${OUTPUT}
	./${OUTPUT}
