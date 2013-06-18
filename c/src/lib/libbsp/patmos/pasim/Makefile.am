##
## $Id: Makefile.am,v 1.00 2013/06/14 15:17:12 André Rocha $
##

ACLOCAL_AMFLAGS = -I ../../../../aclocal

include $(top_srcdir)/../../../../automake/compile.am

include_bspdir = $(includedir)/bsp

dist_project_lib_DATA = bsp_specs

include_HEADERS = include/bsp.h

nodist_include_HEADERS = include/bspopts.h
nodist_include_bsp_HEADERS = ../../shared/include/bootcard.h
DISTCLEANFILES = include/bspopts.h

noinst_PROGRAMS =

# wrapup is the one that actually builds and installs the library
# from the individual .rel files built in other directories
SUBDIRS = . tools

include_HEADERS += include/pasim.h
include_HEADERS += ../../shared/include/coverhd.h

noinst_LIBRARIES = libbspstart.a
libbspstart_a_SOURCES = ../../patmos/shared/start.S
project_lib_DATA = start.$(OBJEXT)

dist_project_lib_DATA += startup/linkcmds ../shared/startup/linkcmds.base

noinst_LIBRARIES += libbsp.a
libbsp_a_SOURCES =

# startup
libbsp_a_SOURCES += ../../shared/bspclean.c ../../shared/bsplibc.c \
    ../../shared/bsppost.c ../../shared/bootcard.c startup/bspstart.c \
    ../../sparc/shared/bsppretaskinghook.c ../../shared/bsppredriverhook.c \
    ../../sparc/shared/bspgetworkarea.c ../../shared/sbrk.c \
    startup/spurious.c startup/bspidle.S
# console
libbsp_a_SOURCES += console/console.c
# debugio
libbsp_a_SOURCES += console/debugputs.c
# clock
libbsp_a_SOURCES += clock/ckinit.c ../../../shared/clockdrv_shell.h
# PCI
include_HEADERS += ../../sparc/shared/include/pci.h
libbsp_a_SOURCES += pci/pci.c ../../sparc/shared/pci/pcifinddevice.c

# timer
libbsp_a_SOURCES += timer/timer.c

EXTRA_DIST = shmsupp/README
if HAS_MP
# shmsupp
libbsp_a_SOURCES += shmsupp/getcfg.c shmsupp/lock.c  
endif

EXTRA_DIST += times

include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../../automake/local.am
