CC = clang
CFLAGS = -Wall -g
#override CFLAGS += -fsanitize=address -fno-omit-frame-pointer

project = logfind

sources = $(wildcard *.c)
objects = ${sources:.c=.o}

all: $(project)

$(project): $(objects)
	$(CC) $(CFLAGS) $(objects) -o $(project)

clean: 
	rm -rf $(project) $(objects)
