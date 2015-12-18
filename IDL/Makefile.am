########################################################################
# Copyright (C) 2000,2001,2002,2006                                    #
# Shinobu Yoshimura, University of Tokyo,                              #
# the Japan Society for the Promotion of Science (JSPS)                #
# All Rights Reserved                                                  #
########################################################################

C_IDL    = orbit-idl
CXX_IDL  = idl --hh-suffix=hh --poa --no-boa

# EXTRA_DIST=adv.idl

CLEANFILES = @LOCAL_LIBDIR@/libAdvIDL.a 

DISTCLEANFILES = @LOCAL_INCLUDEDIR@/Adv/adv.idl  \
             @LOCAL_INCLUDEDIR@/Adv/adv.h  \
             @LOCAL_INCLUDEDIR@/Adv/adv.hh  \
             @LOCAL_INCLUDEDIR@/Adv/adv-idl.h  \
             @LOCAL_INCLUDEDIR@/Adv/adv-idl.hh  


#============================================================

if HAVE_ORBIT

CFLAGS_ORBIT = @ORBIT_CLIENT_CFLAGS@
SOURCES_C    = adv-common.c adv-stubs.c adv-skels.c
HEADERS_C    = adv-idl.h 
GENERATE_C   = $(SOURCES_C) $(HEADERS_C) adv.h

$(GENERATE_C): adv.idl
	$(C_IDL) $<
	rm -f  adv-idl.h
	$(LN_S)  adv.h adv-idl.h

else
HEADERS_C    = adv-idl.h 
adv.h adv-idl.h:
	echo "This is a dymmy file" > adv.h
	echo "This is a dymmy file" > adv-idl.h
endif

#============================================================

if HAVE_MICO

SOURCES_CXX  = adv.cc
HEADERS_CXX  = adv-idl.hh
GENERATE_CXX = $(SOURCES_CXX) $(HEADERS_CXX) adv.hh
CXX=mico-c++ -c

$(GENERATE_CXX): adv.idl
	$(CXX_IDL) $<
	ln -sf adv.hh adv-idl.hh

else
HEADERS_CXX  = adv-idl.hh
adv.hh adv-idl.hh:
	echo "This is a dymmy file" > adv.hh
	echo "This is a dymmy file" > adv-idl.hh
endif

#============================================================

CFLAGS   = @CFLAGS@ $(CFLAGS_ORBIT)
CXXFLAGS = @CXXFLAGS@

SOURCES_ALL=$(SOURCES_C) $(SOURCES_CXX)
HEADERS_ALL=$(HEADERS_C) $(HEADERS_CXX)

lib_LIBRARIES = libAdvIDL.a
include_HEADERS   = $(HEADERS_ALL) adv.h adv.hh adv.idl
libAdvIDL_a_SOURCES = $(SOURCES_ALL)
libAdvIDL_a_DEPENDENCIES = $(HEADERS_ALL)

clean:
	rm -f $(GENERATE_C) $(GENERATE_CXX)

all:
	for header in "${include_HEADERS}"; do \
	  if test ! -z "$${header}"; then cp -pf $(top_builddir)/IDL/$${header} @LOCAL_INCLUDEDIR@/Adv; fi; \
	done
	(lib=`pwd`/libAdvIDL.a &&  cd @LOCAL_LIBDIR@ && \
         rm -f libAdvIDL.a &&  $(LN_S) $${lib} .)

