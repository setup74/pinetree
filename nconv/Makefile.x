# $Id: Makefile,v 1.3 1998/07/28 08:17:34 yjlee Exp yjlee $
# nconv - ncode cond conversion utility Makefile

BACKDIR = ../backup

PROJECT = nconv-src
PROGRAMS= nconv
LOCAL_FILES	= nconv.c README
SHARED_FILES	= ncode.h ncode.c
OBJS	= ncode.o nconv.o
LIBS	=
CFLAGS  =

include $(RCSMAKEFILE)

$(PROJECT): $(PROGRAMS)
	@# do nothing

nconv: nconv.o ncode.o
	$(CC) $(CFLAGS) -o $@ nconv.o ncode.o

# DO NOT DELETE
