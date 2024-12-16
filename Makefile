IDIR=include_files
CC=gcc
CFLAGS=-Wall -Wextra -I$(IDIR)


ODIR=src

_DEPS = fileHandler.h throwError.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = fileHandler.o throwError.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


_MAIN = main.c

$(ODIR)/%.o: %.c $(DEPS)
					$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
					$(CC) -o $@ $^ $(_MAIN) $(CFLAGS)


.PHONY: clean
