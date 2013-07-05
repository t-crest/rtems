dnl
dnl $Id$
dnl 
dnl Check for target gcc
dnl

AC_DEFUN([RTEMS_PROG_CC],
[
AC_BEFORE([$0], [AC_PROG_CPP])dnl
AC_BEFORE([$0], [AC_PROG_CC])dnl
AC_BEFORE([$0], [RTEMS_CANONICALIZE_TOOLS])dnl

case "$host" in
*-*-rtems*) ac_cv_exeext=.exe;;
esac

RTEMS_CHECK_TOOL(CC,clang)
RTEMS_CHECK_TOOL(CC,gcc)
test -z "$CC" && \
  AC_MSG_ERROR([no acceptable cc found in \$PATH])
AC_PROG_CC
AC_PROG_CPP
])

AC_DEFUN([RTEMS_PROG_CC_FOR_TARGET],
[
dnl check target cc
RTEMS_PROG_CC

AS_IF([test x"$GCC" = xyes],[
AS_IF([test "${enable_cpukit_root+set}"],[
AS_IF([ test "$CC" = "patmos-unknown-rtems-clang"],[
GCCSPECS="-L=\$(CPUKIT_ROOT)/lib/ -I\$(CPUKIT_ROOT)/lib/include"
], [GCCSPECS="-B\$(CPUKIT_ROOT)/lib/"])])
AS_IF([test "${enable_project_root+set}"],[
AS_IF([ test "$CC" = "patmos-unknown-rtems-clang"],[
GCCSPECS="$GCCSPECS -L=\$(PROJECT_ROOT)/lib/ -I\$(PROJECT_ROOT)/lib/include"
], [GCCSPECS="$GCCSPECS -B\$(PROJECT_ROOT)/lib/"
GCCSPECS="${GCCSPECS} -specs bsp_specs -qrtems"])])])
AC_SUBST(GCCSPECS)
])
