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

include_HEADERS += include/pasim.h

noinst_LIBRARIES = libbspstart.a
libbspstart_a_SOURCES = ../../patmos/shared/start.S
project_lib_DATA = start.$(OBJEXT)

dist_project_lib_DATA += ../shared/startup/linkcmds.base startup/linkcmds

noinst_LIBRARIES += libbsp.a
libbsp_a_SOURCES =

# startup
libbsp_a_SOURCES += ../../shared/bspclean.c ../../shared/bsplibc.c \
    ../../shared/bsppost.c ../../shared/bootcard.c startup/bspstart.c \
    ../../shared/bsppretaskinghook.c ../../shared/bsppredriverhook.c \
    ../../shared/bspgetworkarea.c ../../shared/sbrk.c
# console
libbsp_a_SOURCES += console/console.c
# debugio
libbsp_a_SOURCES += console/debugputs.c
# clock
libbsp_a_SOURCES += clock/ckinit.c ../../../shared/clockdrv_shell.h

include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../../automake/local.am
