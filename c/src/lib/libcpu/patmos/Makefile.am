##
## $Id: Makefile.am,v 1.00 2013/06/14 15:17:12 Andre Rocha $
##

ACLOCAL_AMFLAGS = -I ../../../aclocal

include $(top_srcdir)/../../../automake/compile.am


noinst_PROGRAMS =

include_libcpudir = $(includedir)/libcpu

## cache
include_libcpu_HEADERS = ../shared/include/cache.h

noinst_PROGRAMS += cache.rel
cache_rel_SOURCES = cache/cache.c cache/cache_.h \
    ../shared/src/cache_aligned_malloc.c ../shared/src/cache_manager.c
cache_rel_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/cache
cache_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

if has_instruction_cache
cache_rel_CPPFLAGS += -DHAS_INSTRUCTION_CACHE
endif

## exceptions
include_libcpu_HEADERS += exceptions/exceptions.h

noinst_PROGRAMS += exceptions.rel
exceptions_rel_SOURCES = exceptions/exceptions.c
exceptions_rel_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/exceptions
exceptions_rel_LDFLAGS = $(RTEMS_RELLDFLAGS)

include $(srcdir)/preinstall.am
include $(top_srcdir)/../../../automake/local.am