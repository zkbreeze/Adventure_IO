########################################################################
# Copyright (C) 2000,2001,2002,2006                                    #
# Shinobu Yoshimura, University of Tokyo,                              #
# the Japan Society for the Promotion of Science (JSPS)                #
# All Rights Reserved                                                  #
########################################################################

#CFLAGS=@CFLAGS@ -Wmissing-prototypes -Wstrict-prototypes

CLEANFILES = stamp @LOCAL_LIBDIR@/libAdvDocIO.a 

DISTCLEANFILES = @LOCAL_INCLUDEDIR@/Adv/AdvDocument.h \
	     @LOCAL_INCLUDEDIR@/Adv/AdvFDocument.h \
	     @LOCAL_INCLUDEDIR@/Adv/AdvMDocument.h \
	     @LOCAL_INCLUDEDIR@/Adv/AdvIDocument.h \
	     @LOCAL_INCLUDEDIR@/Adv/AdvDocumentPrivate.h \
	     @LOCAL_INCLUDEDIR@/Adv/AdvNDocumentServ.h

stamp: ${include_HEADERS}
	for header in ${include_HEADERS}; do \
	  cp -pf $(srcdir)/$${header} @LOCAL_INCLUDEDIR@/Adv; \
	done
	(lib=`pwd`/libAdvDocIO.a &&  cd @LOCAL_LIBDIR@ && \
        rm -f libAdvDocIO.a &&  $(LN_S) $${lib} .)
	touch stamp

#if HAVE_MICO
#SOURCES_CXX = AdvNDocumentServ.cc
#HEADERS_CXX = AdvNDocumentServ.h
#else
SOURCES_CXX =
HEADERS_CXX =
#endif

CFLAGS  =@CFLAGS@ @ORBIT_CLIENT_CFLAGS@
CXXFLAGS=@CXXFLAGS@
#LIBS=-L@LOCAL_LIBDIR@ -lAdvIDL -lAdvFileIO -lAdvBase

SOURCES_ALL = AdvDocument.c AdvFDocument.c AdvMDocument.c AdvIDocument.c $(SOURCES_CXX)
HEADERS_ALL = AdvDocument.h AdvFDocument.h AdvMDocument.h AdvIDocument.h AdvDocumentPrivate.h $(HEADERS_CXX)

lib_LIBRARIES      = libAdvDocIO.a
include_HEADERS        = $(HEADERS_ALL)
libAdvDocIO_a_SOURCES = $(SOURCES_ALL)
libAdvDocIO_a_DEPENDENCIES = stamp

EXTRA_PROGRAMS = doctest docclient

PROGRAMS_HAVE_ORBIT_yes =  $(EXTRA_PROGRAMS)
PROGRAMS_HAVE_ORBIT_no  =  

noinst_PROGRAMS = testadvio $(PROGRAMS_HAVE_ORBIT_@HAVE_ORBIT@) 


testadvio_LDADD = @ADV_DOCIO_LIBS@ @ADV_FILEIO_LIBS@ @ADV_BASE_LIBS@
#testadvio_DEPENDENCIES=${top_buiddir}/lib/libAdvDocIO.a

doctest_LDADD = @ADV_DOCIO_LIBS@ @ADV_FILEIO_LIBS@ @ADV_BASE_LIBS@ 
doctest_DEPENDENCIES=libAdvDocIO.a

docclient_LDADD = @ADV_DOCIO_LIBS@ @ADV_FILEIO_LIBS@ @ADV_BASE_LIBS@
docclient_DEPENDENCIES=libAdvDocIO.a

