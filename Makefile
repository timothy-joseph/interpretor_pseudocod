CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c99 -Iinclude/ -g -Og
LIBS=-lm

EXEC_FILE = pseudocod
CFILES = main.c
CFILES += src/errors.c
CFILES += src/execute.c
CFILES += src/garbage.c
CFILES += src/helpers.c
CFILES += src/lex.c
CFILES += src/map.c
CFILES += src/parse.c
CFILES += src/stack.c
CFILES += src/variables.c

OFILES_DIR = build/
OFILES = $(CFILES:%.c=build/%.o)

build: $(EXEC_FILE)

$(EXEC_FILE): $(OFILES)
	$(CC) $(LIBS) -o $@ $(OFILES) $(LDFLAGS)

$(OFILES): build/%.o : %.c

build/%.o : %.c
	mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OFILES) $(EXEC_FILE) $(OFILES_DIR)

