# GNU Makefile for PDCurses - Windows console
#
# Usage: make [-f path\Makefile] [DEBUG=Y] [DLL=Y] [WIDE=Y] [UTF8=Y]
#        [INFOEX=N] [tgt]
#
# where tgt can be any of:
# [all|demos|pdcurses.a|testcurs.exe...]

O = o
E = .exe

INFOEX=N

RM = rm -rf

ifndef PDCURSES_SRCDIR
	PDCURSES_SRCDIR = .
endif

osdir		= $(PDCURSES_SRCDIR)/wincon
common		= $(PDCURSES_SRCDIR)/common

BIN_DIR		= $(PDCURSES_SRCDIR)/bin
LIB_DIR		= $(PDCURSES_SRCDIR)/lib

include $(common)/libobjs.mif

LIBOBJS_WITH_DIR = $(addprefix $(BIN_DIR)/,$(LIBOBJS))
PDCOBJS_WITH_DIR = $(addprefix $(BIN_DIR)/,$(PDCOBJS))

PDCURSES_WIN_H	= $(osdir)/pdcwin.h

CC		= gcc
AR		= ar
STRIP		= strip
LINK		= gcc
WINDRES		= windres

ifeq ($(DEBUG),Y)
	CFLAGS  = -g -Wall -DPDCDEBUG
	LDFLAGS = -g
else
	CFLAGS  = -O2 -Wall
	LDFLAGS =
endif

CFLAGS += -I$(PDCURSES_SRCDIR)/include

ifeq ($(WIDE),Y)
	CFLAGS += -DPDC_WIDE
endif

ifeq ($(UTF8),Y)
	CFLAGS += -DPDC_FORCE_UTF8
endif

LIBFLAGS = rcv
LIBCURSES = $(LIB_DIR)/libpdcurses.a

ifeq ($(INFOEX),N)
	CFLAGS += -DHAVE_NO_INFOEX
endif

.PHONY: all libs clean

all:	libs

libs:	$(LIBCURSES)

clean:
	-$(RM) $(BIN_DIR) $(LIB_DIR)

$(BIN_DIR):
	mkdir -p $@
$(LIB_DIR):
	mkdir -p $@

$(LIBCURSES) : $(LIBOBJS_WITH_DIR) $(PDCOBJS_WITH_DIR) $(RESOURCE) | $(LIB_DIR)
	$(AR) $(LIBFLAGS) $@ $?

$(LIBOBJS_WITH_DIR) $(PDCOBJS_WITH_DIR) : $(PDCURSES_HEADERS) | $(BIN_DIR)
$(PDCOBJS_WITH_DIR) : $(PDCURSES_WIN_H) | $(BIN_DIR)

$(LIBOBJS_WITH_DIR) : $(BIN_DIR)/%.o: $(srcdir)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(PDCOBJS_WITH_DIR) : $(BIN_DIR)/%.o: $(osdir)/%.c
	$(CC) -c $(CFLAGS) $< -o $@
