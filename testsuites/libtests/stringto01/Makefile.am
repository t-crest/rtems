##
## $Id$
##

MANAGERS = all

rtems_tests_PROGRAMS = stringto01
stringto01_SOURCES = init.c

dist_rtems_tests_DATA = stringto01.scn stringto01.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

stringto01_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(stringto01_OBJECTS) $(stringto01_LDADD)
LINK_LIBS = $(stringto01_LDLIBS)

stringto01$(EXEEXT): $(stringto01_OBJECTS) $(stringto01_DEPENDENCIES)
	@rm -f stringto01$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
