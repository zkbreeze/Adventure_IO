########################################################################
# Copyright (C) 2000,2001,2002,2006                                    #
# Shinobu Yoshimura, University of Tokyo,                              #
# the Japan Society for the Promotion of Science (JSPS)                #
# All Rights Reserved                                                  #
########################################################################

CXXFLAGS=@CXXFLAGS@
LIBS=-L. -L@LOCAL_LIBDIR@ @ADV_DOCIO_LIBS@ @ADV_FILEIO_LIBS@ @ADV_BASE_LIBS@ @LIBZ_LIBS@

EXTRA_DIST=Adv.py pyadv.c advinfo.py toshio.py advinspect.py advcheck.py

EXTRA_PROGRAMS = m8bizer advprop

PROGRAMS_HAVE_CURSES_yes =  m8bizer
PROGRAMS_HAVE_CURSES_no  =

CLEANFILES= $(EXTRA_PROGRAMS) pyadv.so

# noinst_PROGRAMS=advinfo advcat toadv toshio tofem advprop $(PROGRAMS_HAVE_CURSES_@HAVE_CURSES@)

bin_PROGRAMS = advcat advinfo toadv tofem toshio 

toadv_SOURCES=toadv.c

advcat_SOURCES=advcat.c

toshio_SOURCES=toshio.c

tofem_SOURCES=tofem.c

advinfo_SOURCES=advinfo.c

advprop_SOURCES=advprop.cc

m8bizer_SOURCES= m8bizer.cc mesh.cc m8bizer.h mesh.h
m8bizer_LDADD=@CURSES_LIBS@

if HAVE_PYTHON

# all: pyadv.so

pyadv.so:	pyadv.c
	$(CC) -shared -o $@ $^ @PYTHON_CFLAGS@ -I@LOCAL_INCLUDEDIR@ \
	-L@LOCAL_LIBDIR@ @ADV_DOCIO_LIBS@ @ADV_FILEIO_LIBS@ @ADV_BASE_LIBS@

endif
