# Copyright (C) 2002 John Todd Larason <jtl@molehill.org>

#    This Makefile is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

BIGFILE=-DBIGFILE_BY_STDIO
LARGEFILE_SUPPORT=-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
PLATFORM_SUPPORT=-D__unix__ 
PLATFORM=$(shell /usr/libexec/config.guess)

ifneq ($(findstring darwin,$(PLATFORM)),)
  MD5LIB=-lcrypto
else
  ifneq ($(findstring freebsd,$(PLATFORM)),)
    MD5LIB=-lcrypto
  else
    MD5LIB=-lssl
  endif
endif

# compiler-specific items:
# -Wwrite-strings
WARN_ME_HARDER=-Wall -W -Wshadow -Wpointer-arith \
	       -Wcast-align -Wstrict-prototypes \
	       -Wmissing-prototypes -Wmissing-declarations 
OPTIMIZE=-O
#  -DVERBOSE_OBFUSC -DEXPECT_CAN_EXIT
DEBUG=-g
WARNINGS=$(WARN_ME_HARDER)

# it's hoped you won't need to modify anything below here to compile
# on a Unix system

VER=0.4.0

CPPFLAGS=$(BIGFILE) $(LARGEFILE_SUPPORT) $(PLATFORM_SUPPORT)
CFLAGS=$(WARN_ME_HARDER) $(OPTIMIZE) $(DEBUG)
LDFLAGS=-L.
LDLIBS=-lreplaypc $(MD5LIB)

PROGRAMS=ReplayPC httpfs mark-commercial rddns dump-guide rtvguide
EXTRA_PROGRAMS=find-GOPs build-ndx ndx-dump un-protect crypt-test dump-cg2 dump-headend dump-zipcode2 dump-filedarray dump-replaychannels dump-addressbook dump-channelset dump-programset dump-replayshows version-test
SCRIPTS=rns
DOCS=Authors COPYING Readme.txt

LIB=libreplaypc.a

LIBOBJS=			\
	addressbook.o		\
	bigfile.o		\
	cg.o			\
	crypt.o			\
	dump.o			\
	filedarray.o		\
	guide.o			\
	guideclient.o		\
	headend.o		\
	httpclient.o		\
	httpfsclient.o		\
	ndx.o			\
	netclient.o 		\
	program.o		\
	rddnsclient.o		\
	rtv.o			\
	sleep.o			\
	zipcode.o		\
	channelset.o		\

all: $(LIB) $(PROGRAMS) $(EXTRA_PROGRAMS)
$(PROGRAMS) $(EXTRA_PROGRAMS): $(LIB)
$(LIB): $(LIBOBJS)
	ar r $@ $^
	ranlib $(LIB)

bindist: all
	-mkdir replaypc-$(VER)-$(PLATFORM)
	cp $(PROGRAMS) $(SCRIPTS) $(DOCS) replaypc-$(VER)-$(PLATFORM)
	tar cf - replaypc-$(VER)-$(PLATFORM) | gzip > replaypc-$(VER)-$(PLATFORM).tar.gz

clean:
	-rm *.o $(PROGRAMS) $(EXTRA_PROGRAMS) $(LIB)

realclean: clean
	-rm -rf replaypc-$(VER)-$(PLATFORM) replaypc-$(VER)-$(PLATFORM).tar.gz
