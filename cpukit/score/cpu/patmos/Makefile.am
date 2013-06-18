##
## $Id: Makefile.am,v 1.00 2013/06/14 15:17:12 André Rocha $
##

include $(top_srcdir)/automake/compile.am

include_rtemsdir = $(includedir)/rtems
include_rtems_HEADERS = rtems/asm.h

include_rtems_scoredir = $(includedir)/rtems/score
include_rtems_score_HEADERS = rtems/score/patmos.h rtems/score/cpu.h \
    rtems/score/types.h

noinst_LIBRARIES = libscorecpu.a
libscorecpu_a_SOURCES = cpu.c
libscorecpu_a_CPPFLAGS = $(AM_CPPFLAGS)

include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/local.am
