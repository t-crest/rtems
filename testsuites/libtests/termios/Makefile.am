##
## $Id$
##

MANAGERS = io semaphore

rtems_tests_PROGRAMS = termios
termios_SOURCES = init.c

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

termios_LDADD = $(MANAGERS_NOT_WANTED:%=$(PROJECT_LIB)/no-%.rel)

AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(termios_OBJECTS) $(termios_LDADD)
LINK_LIBS = $(termios_LDLIBS)

termios$(EXEEXT): $(termios_OBJECTS) $(termios_DEPENDENCIES)
	@rm -f termios$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
