# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: pictureProcess

SRC = pictureProcess.c 
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

pictureProcess: pictureProcess.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
