########################################################################
# Copyright (C) 2000,2001,2002,2006                                    #
# Shinobu Yoshimura, University of Tokyo,                              #
# the Japan Society for the Promotion of Science (JSPS)                #
# All Rights Reserved                                                  #
########################################################################

AUTOMAKE_OPTIONS = foreign no-installman no-installinfo

CLEANFILES = @LOCAL_LIBDIR@/libAdvBase.a 

DISTCLEANFILES = @LOCAL_INCLUDEDIR@/Adv/AdvStat.h \
             @LOCAL_INCLUDEDIR@/Adv/AdvTypes.h \
             @LOCAL_INCLUDEDIR@/Adv/AdvUtil.h 

CFLAGS   =  @CFLAGS@

#============================================================
# libraries
#============================================================

lib_LIBRARIES = libAdvBase.a

libAdvBase_a_SOURCES = AdvStat.c AdvUtil.c

include_HEADERS = \
AdvStat.h \
AdvTypes.h \
AdvUtil.h

all:
	for header in ${include_HEADERS}; do \
	  cp -pf $(srcdir)/$${header} @LOCAL_INCLUDEDIR@/Adv; \
	done
	(lib=`pwd`/libAdvBase.a &&  cd @LOCAL_LIBDIR@ && \
         rm -f libAdvBase.a &&  $(LN_S) $${lib} .)

