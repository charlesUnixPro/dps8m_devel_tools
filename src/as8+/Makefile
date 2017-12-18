# Copyright 2012-2015 by Harry Reed
#
# All rights reserved.
#
# This software is made available under the terms of the
# ICU License -- ICU 1.8.1 and later. 
# See the LICENSE file at the top-level directory of this distribution and
# at https://sourceforge.net/p/dps8m/code/ci/master/tree/LICENSE

CFLAGS = -O0 -g

# 'make -DCROSS=MINGW64'
ifeq ($(CROSS),MINGW64)
  CFLAGS += -DCROSS_MINGW64 -Wno-misleading-indentation 
  CFLAGS += -I../mingw_include
  CFLAGS += -DHAVE_DLOPEN=so
  LDFLAGS += -L../mingw_lib
  LIBS += -lwinmm -lws2_32 -lmingwex -luv -lwsock32 -lws2_32 
  CC = x86_64-w64-mingw32-gcc
  LD = x86_64-w64-mingw32-gcc
  EXE = .exe
ifeq ($(msys_version),0)
  AR = x86_64-w64-mingw32-ar
else
  AR = ar
endif
endif

# simh requires -ldl
# Cygwin seems to need...
ifeq ($(OS),Windows_NT)
    LIBS += -lgcc -lm  -lwinmm -lws2_32 -lmingwex $(LIBUV) -lwsock32 -lpthread -lgnurx -lpsapi -liphlpapi  -luserenv
else
  OSTYPE = $(shell uname -s)
  ifeq ($(OSTYPE),FreeBSD)
    LIBS += -lm
  endif
  ifeq ($(OSTYPE),Linux)
    ifeq ($(CROSS),MINGW64)
      LIBS += -lm
    else
      LIBS += -lm -ldl
    endif
  endif
endif

C_SRCS += \
./as8.l \
./as8.y \
./as.c \
./asUtils.c \
./asMisc.c \
./expr.c \
./opcodeTable.c \
./opCodes.c \
./literals.c \
./main.c \
./pseudoOps.c 
#ifeq ($(CROSS),MINGW64)
#C_SRCS  += ./strcasestr.c
#endif

OBJS += \
./as.o \
./asUtils.o \
./asMisc.o \
./expr.o \
./opcodeTable.o \
./opCodes.o \
./literals.o \
./main.o \
./pseudoOps.o \
./y.tab.o \
./lex.yy.o
#ifeq ($(CROSS),MINGW64)
#OBJS  += ./strcasestr.o
#endif

CFLAGS += -std=c99

YACC = bison
LEX = flex

# rules

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: as8+ as8pp

# Tool invocations
as8+: $(OBJS) y.tab.o lex.yy.o 
	$(CC) $(LDFLAGS) -o as8+$(EXE) $(OBJS) 

opcodeTable.o: y.tab.h

y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c y.tab.c

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -Wno-unused -c lex.yy.c

y.tab.c: as8.y
	$(YACC) -ydv as8.y

y.tab.h: y.tab.c

lex.yy.c: as8.l
	$(LEX) -i as8.l	
	
as8pp : as8pp.c
	$(CC) as8pp.c -o as8pp $(CFLAGS) -U__STRICT_ANSI__ -O0 -g

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) y.tab.c y.tab.h lex.yy.c y.output as8+
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

