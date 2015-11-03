C = gcc

CFLAGS = -Wall -Werror -ansi -pedantic

CFLAGS += -D_XOPN_SOURCE=500

CFLAGS += -g

## Cibles principales

all: do2

makeargv.o: makeargv.c
	${CC} ${CFLAGS} -c $<

do2.o: do2.c
	${CC} ${CFLAGS} -c $<

do: do2.o makeargv.o
	${CC} ${CFLAGS} -o $@ $^
