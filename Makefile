# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: solver XORNN filters hough

SRC = solver.c XORNN.c filters.c hough.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

solver: solver.o
XORNN: XORNN.o
filters: filters.o
hough : hough.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
