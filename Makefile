M68K = m68k-unknown-elf

CC		= $(M68K)-cc
AS		= $(M68K)-as
AR		= $(M68K)-ar
LD		= $(M68K)-ld
RANLIB	= $(M68K)-ranlib

SHELL	= /bin/sh

SRC		= ./src/syscalls.c
OBJS	= $(SRC:.c=.o)

AMX_BSP	= libamx.a

INCLUDES = -I.

all: ${AMX_BSP}

.c.o:
	$(CC) -O2 $(INCLUDES) -c $(CFLAGS) $< -o $@

${AMX_BSP}: $(OBJS)
	${AR} ${ARFLAGS} $@ $(OBJS)
	${RANLIB} $@

clean:
	-rm -f $(OBJS)
	-rm -f ${AMX_BSP}
