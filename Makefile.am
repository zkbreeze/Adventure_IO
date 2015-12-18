########################################################################
# Copyright (C) 2000,2001,2002,2006                                    #
# Shinobu Yoshimura, University of Tokyo,                              #
# the Japan Society for the Promotion of Science (JSPS)                #
# All Rights Reserved                                                  #
########################################################################

AUTOMAKE_OPTIONS = foreign no-installman no-installinfo

SUBDIRS=@subdirs@

DOCDIR=@docdir@

pkgdatadir=$(DOCDIR)/@PACKAGE@

DISTCLEANFILES = @LOCAL_INCLUDEDIR@/Adv/advsys-config.h 

bin_SCRIPTS=advsys-config

pkgdata_DATA=README README.eucJP copyright

include_HEADERS=advsys-config.h

all:
	cp -pf $(top_builddir)/advsys-config.h @LOCAL_INCLUDEDIR@/Adv

EXTRA_DIST=README.eucJP  copyright

#============================================================
# IDL compilation
#============================================================

dist-hook:
	rm -f $(distdir)/ChangeLog

ifrepo:
	idl -I/usr/local/include --no-codegen-c++ --feed-ir /usr/local/include/mico/CosEventComm.idl
	idl -I/usr/local/include --no-codegen-c++ --feed-ir /usr/local/include/mico/ir_base.idl
	idl -I/usr/local/include --no-codegen-c++ --feed-ir /usr/local/include/mico/ir.idl
	idl -I/usr/local/include --no-codegen-c++ --feed-ir adv.idl

snap: dist
	mv $(distdir).tar.gz $(PACKAGE)-`date +%Y%m%d`.tar.gz
	cp $(PACKAGE)-`date +%Y%m%d`.tar.gz ~/pub
