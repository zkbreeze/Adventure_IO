########################################################################
# Copyright (C) 2000,2001,2002,2006                                    #
# Shinobu Yoshimura, University of Tokyo,                              #
# the Japan Society for the Promotion of Science (JSPS)                #
# All Rights Reserved                                                  #
########################################################################

AUTOMAKE_OPTIONS = foreign

CLEANFILES = @LOCAL_LIBDIR@/libAdvFileIO.a 

DISTCLEANFILES = @LOCAL_INCLUDEDIR@/Adv/AdvFileIO.h 

CFLAGS=@CFLAGS@
CXXFLAGS=@CXXFLAGS@
LDADD=-L. -L@LOCAL_LIBDIR@ -lAdvFileIO -lAdvBase

document_DATAS = toadv.txt advinfo.txt

include_HEADERS = AdvFileIO.h

lib_LIBRARIES = libAdvFileIO.a
libAdvFileIO_a_SOURCES = AdvFileIO.c

all:
	for header in ${include_HEADERS}; do \
	  cp -pf $(srcdir)/$${header} @LOCAL_INCLUDEDIR@/Adv; \
	done
	(lib=`pwd`/libAdvFileIO.a &&  cd @LOCAL_LIBDIR@ && \
         rm -f libAdvFileIO.a &&  $(LN_S) $${lib} .)

