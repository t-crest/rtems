## Process this file with autoconf to produce a configure script.
##
## $Id: configure.ac,v 1.00 2013/06/14 15:17:12 André Rocha $

AC_PREREQ([2.68])
AC_INIT([rtems-c-src-lib-libbsp-patmos],[_RTEMS_VERSION],[http://www.rtems.org/bugzilla])
AC_CONFIG_SRCDIR([shared])
RTEMS_TOP(../../../../..)

RTEMS_CANONICAL_TARGET_CPU
AM_INIT_AUTOMAKE([no-define foreign 1.11.1])
AM_MAINTAINER_MODE

RTEMS_ENV_RTEMSBSP
RTEMS_PROJECT_ROOT

RTEMS_CHECK_BSPDIR([$RTEMS_BSP_FAMILY])

# Explicitly list all Makefiles here
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
