pwd=$(pwd)

mkdir -p $pwd/c/src/lib/libbsp/patmos/pasim/clock
mkdir -p $pwd/c/src/lib/libbsp/patmos/pasim/console
mkdir -p $pwd/c/src/lib/libbsp/patmos/pasim/include
mkdir -p $pwd/c/src/lib/libbsp/patmos/pasim/make/custom
mkdir -p $pwd/c/src/lib/libbsp/patmos/pasim/startup
mkdir -p $pwd/c/src/lib/libbsp/patmos/shared/startup
mkdir -p $pwd/c/src/lib/libbsp/patmos/tcrest
mkdir -p $pwd/c/src/lib/libcpu/patmos/cache
mkdir -p $pwd/cpukit/score/cpu/patmos/rtems/score


cp -f $pwd/c/src/make/compilers/gcc-target-default.cfg $pwd/c/src/make/compilers/patmos-clang.cfg

cp -f $pwd/c/src/lib/libbsp/sparc/acinclude.m4 $pwd/c/src/lib/libbsp/patmos/acinclude.m4
cp -f $pwd/c/src/lib/libbsp/sparc/aclocal.m4 $pwd/c/src/lib/libbsp/patmos/aclocal.m4
cp -f $pwd/c/src/lib/libbsp/sparc/configure $pwd/c/src/lib/libbsp/patmos/configure
cp -f $pwd/c/src/lib/libbsp/sparc/configure.ac $pwd/c/src/lib/libbsp/patmos/configure.ac
cp -f $pwd/c/src/lib/libbsp/sparc/Makefile.am $pwd/c/src/lib/libbsp/patmos/Makefile.am
cp -f $pwd/c/src/lib/libbsp/sparc/Makefile.in $pwd/c/src/lib/libbsp/patmos/Makefile.in
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/clock/ckinit.c $pwd/c/src/lib/libbsp/patmos/pasim/clock/ckinit.c
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/console/console.c $pwd/c/src/lib/libbsp/patmos/pasim/console/console.c
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/console/debugputs.c $pwd/c/src/lib/libbsp/patmos/pasim/console/debugputs.c
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/include/bsp.h $pwd/c/src/lib/libbsp/patmos/pasim/include/bsp.h
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/include/bspopts.h.in $pwd/c/src/lib/libbsp/patmos/pasim/include/bspopts.h.in
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/include/leon.h $pwd/c/src/lib/libbsp/patmos/pasim/include/pasim.h
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/make/custom/leon3.cfg $pwd/c/src/lib/libbsp/patmos/pasim/make/custom/pasim.cfg
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/startup/bspstart.c $pwd/c/src/lib/libbsp/patmos/pasim/startup/bspstart.c
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/startup/linkcmds $pwd/c/src/lib/libbsp/patmos/pasim/startup/linkcmds
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/aclocal.m4 $pwd/c/src/lib/libbsp/patmos/pasim/aclocal.m4
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/bsp_specs $pwd/c/src/lib/libbsp/patmos/pasim/bsp_specs
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/configure $pwd/c/src/lib/libbsp/patmos/pasim/configure
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/configure.ac $pwd/c/src/lib/libbsp/patmos/pasim/configure.ac
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/Makefile.am $pwd/c/src/lib/libbsp/patmos/pasim/Makefile.am
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/Makefile.in $pwd/c/src/lib/libbsp/patmos/pasim/Makefile.in
cp -f $pwd/c/src/lib/libbsp/sparc/leon3/preinstall.am $pwd/c/src/lib/libbsp/patmos/pasim/preinstall.am
cp -f $pwd/c/src/lib/libbsp/sparc/shared/startup/linkcmds.base $pwd/c/src/lib/libbsp/patmos/shared/startup/linkcmds.base
cp -f $pwd/c/src/lib/libbsp/sparc/shared/start.S $pwd/c/src/lib/libbsp/patmos/shared/start.S

cp -f $pwd/c/src/lib/libcpu/sparc/aclocal.m4 $pwd/c/src/lib/libcpu/patmos/aclocal.m4
cp -f $pwd/c/src/lib/libcpu/sparc/configure $pwd/c/src/lib/libcpu/patmos/configure
cp -f $pwd/c/src/lib/libcpu/sparc/configure.ac $pwd/c/src/lib/libcpu/patmos/configure.ac
cp -f $pwd/c/src/lib/libcpu/sparc/Makefile.am $pwd/c/src/lib/libcpu/patmos/Makefile.am
cp -f $pwd/c/src/lib/libcpu/sparc/Makefile.in $pwd/c/src/lib/libcpu/patmos/Makefile.in
cp -f $pwd/c/src/lib/libcpu/sparc/preinstall.am $pwd/c/src/lib/libcpu/patmos/preinstall.am
cp -f $pwd/c/src/lib/libcpu/sparc/cache/cache.c $pwd/c/src/lib/libcpu/patmos/cache/cache.c
cp -f $pwd/c/src/lib/libcpu/sparc/cache/cache_.h $pwd/c/src/lib/libcpu/patmos/cache/cache_.h

cp -f $pwd/cpukit/score/cpu/sparc/cpu.c $pwd/cpukit/score/cpu/patmos/cpu.c
cp -f $pwd/cpukit/score/cpu/sparc/cpu_asm.S $pwd/cpukit/score/cpu/patmos/cpu_asm.S
cp -f $pwd/cpukit/score/cpu/sparc/Makefile.am $pwd/cpukit/score/cpu/patmos/Makefile.am
cp -f $pwd/cpukit/score/cpu/sparc/Makefile.in $pwd/cpukit/score/cpu/patmos/Makefile.in
cp -f $pwd/cpukit/score/cpu/sparc/preinstall.am $pwd/cpukit/score/cpu/patmos/preinstall.am
cp -f $pwd/cpukit/score/cpu/sparc/rtems/asm.h $pwd/cpukit/score/cpu/patmos/rtems/asm.h
cp -f $pwd/cpukit/score/cpu/sparc/rtems/score/cpu.h $pwd/cpukit/score/cpu/patmos/rtems/score/cpu.h
cp -f $pwd/cpukit/score/cpu/sparc/rtems/score/sparc.h $pwd/cpukit/score/cpu/patmos/rtems/score/patmos.h
cp -f $pwd/cpukit/score/cpu/sparc/rtems/score/types.h $pwd/cpukit/score/cpu/patmos/rtems/score/types.h

cp -f $pwd/doc/cpu_supplement/sparc.t $pwd/doc/cpu_supplement/patmos.t

cp -f $pwd/make/compilers/gcc-no_bsp.cfg $pwd/make/compilers/patmos-clang.cfg
