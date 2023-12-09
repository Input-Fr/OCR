CC = gcc
CFLAGS = -g -Wall
LIBS = $(shell pkg-config --cflags --libs gtk+-3.0) -lm -lSDL2 -lSDL2_image

SRCS = main.c filters.c Rotate.c
OBJS = $(SRCS:.c=.o)

TARGET = image_loader

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(shell pkg-config --cflags gtk+-3.0)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
