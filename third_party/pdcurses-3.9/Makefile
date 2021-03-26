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

bindir		= $(PDCURSES_SRCDIR)/bin
libdir		= $(PDCURSES_SRCDIR)/lib

include $(common)/libobjs.mif

LIBOBJS_WITH_DIR = $(addprefix $(bindir)/,$(LIBOBJS))
PDCOBJS_WITH_DIR = $(addprefix $(bindir)/,$(PDCOBJS))

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
LIBCURSES = $(libdir)/pdcurses.a

ifeq ($(INFOEX),N)
	CFLAGS += -DHAVE_NO_INFOEX
endif

.PHONY: all libs clean directories

all:	libs

libs:	directories $(LIBCURSES)

clean:
	-$(RM) $(bindir) $(libdir)

directories: $(bindir) $(libdir)

$(bindir):
	mkdir -p $@
$(libdir):
	mkdir -p $@

$(LIBCURSES) : $(LIBOBJS_WITH_DIR) $(PDCOBJS_WITH_DIR) $(RESOURCE)
	$(AR) $(LIBFLAGS) $@ $?

pdcurses.o: $(common)/pdcurses.rc
	$(WINDRES) -i $(common)/pdcurses.rc pdcurses.o

$(LIBOBJS_WITH_DIR) $(PDCOBJS_WITH_DIR) : $(PDCURSES_HEADERS)
$(PDCOBJS_WITH_DIR) : $(PDCURSES_WIN_H)
panel.o : $(PANEL_HEADER)

$(LIBOBJS_WITH_DIR) : $(bindir)/%.o: $(srcdir)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(PDCOBJS_WITH_DIR) : $(bindir)/%.o: $(osdir)/%.c
	$(CC) -c $(CFLAGS) $< -o $@
