##
## $Id: Makefile.am,v 1.00 2013/06/14 15:17:12 André Rocha $
##

ACLOCAL_AMFLAGS = -I ../../../aclocal

# Descend into the @RTEMS_BSP_FAMILY@ directory
SUBDIRS = @RTEMS_BSP_FAMILY@

EXTRA_DIST =

EXTRA_DIST += shared/bspclean.c
EXTRA_DIST += shared/bspstart.c
EXTRA_DIST += shared/start.S

# PCI bus
EXTRA_DIST += shared/include/pci.h
EXTRA_DIST += shared/pci/pcifinddevice.c

include $(top_srcdir)/../../../automake/subdirs.am
include $(top_srcdir)/../../../automake/local.am
