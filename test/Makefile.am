# [open]lipc - Makefile.am
# Copyright (c) 2016 Arkadiusz Bokowy

AM_CFLAGS = -I$(top_srcdir)/include
LDADD = -llipc

check_PROGRAMS = \
	lipc-test-init \
	lipc-test-hasharray \
	lipc-test-prop \
	lipc-test-event

if ENABLE_KINDLE_ENV
AM_LDFLAGS = \
	-L$(KINDLE_ROOTDIR)/lib \
	-L$(KINDLE_ROOTDIR)/usr/lib \
	-Wl,-rpath=$(KINDLE_ROOTDIR)/lib \
	-Wl,-rpath=$(KINDLE_ROOTDIR)/usr/lib
endif
