/*
 *  start.s
 *
 *  Common start code for Patmos.
 *
 *  Project: T-CREST - Time-Predictable Multi-Core Architecture for Embedded Systems
 *
 *  Copyright (C) GMVIS Skysoft S.A., 2013
 *  @author Andre Rocha
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 * 
 */

#include <rtems/asm.h>
#include <bsp.h>

/* Arrays for 64 return addresses */
.comm _loader_baseaddr,256,4
.comm _loader_off,256,4

.type   hard_reset, @function
.size   hard_reset, .Ltmp0-hard_reset
.fstart hard_reset, .Ltmp0-hard_reset, 4
 
  /*
   *  This is the hard reset code.
   */
	PUBLIC(hard_reset)
SYM(hard_reset):

  /*
   * save loader return information
   */
  li      $r1 = _cpuinfo_base
  lwl     $r1 = [$r1]
  mfs     $r29 = $srb
  shadd2  $r2 = $r1, _loader_baseaddr
  swm     [$r2] = $r29
  mfs     $r29 = $sro
  shadd2  $r2 = $r1, _loader_off
  swm     [$r2] = $r29


#ifndef PATMOS_MULTICORE

  /*
   * Exit immediately with exit code -1 if not on core 0.
   * Stack setup and RTEMS setup code need to be adapted when moving to multicore.
   */
  cmpeq   $p1 = $r1, 0
(!$p1) br .Lexit
  li      $r1 = -1
  nop

 #endif /* PATMOS_MULTICORE */

  /*
   * setup stack cache and stack frame    
   */
  li	  $r31 = _shadow_stack_base /* Initialize shadow stack pointer */
  li	  $r9 = _stack_cache_base
  mts	  $s5 = $r9 /* Initialize the stack cache's top pointer */
  mts	  $s6 = $r9 /* Initialize the stack cache's spill top pointer */

  /*
   * invoke main function                    
   */

  call	  SYM(boot_card) /* invoke boot_card function */
  li	  $r3 = 0 /* pass argument argc */
  li	  $r4 = 0 /* pass argument argv */
  li	  $r5 = 0 /* pass argument envp */

  /*
   * exit
   */
.Lexit:
  call	  SYM(__exit) /* terminate program and invoke exit */
  mov	  $r3 = $r1 /* get exit code (in first delay slot) */
  nop
  nop
.Ltmp0:
  
  /*
   * Halt the CPU. If boot_card returns, also halt the CPU.
   */
.type   __exit, @function
.size   __exit, .Ltmp1-__exit
.fstart __exit, .Ltmp1-__exit, 4
 
        PUBLIC(__exit)
SYM(__exit):
  li      $r1 = _cpuinfo_base
  lwl     $r1 = [$r1]
  nop
  shadd2  $r2 = $r1, _loader_baseaddr
  lwm     $r29 = [$r2]
  nop
  mts     $srb = $r29
  shadd2  $r2 = $r1, _loader_off
  lwm     $r29 = [$r2]
  nop
  mts     $sro = $r29
  mov     $r1 = $r3 /* store exit code */
  ret     /* return to loader and/or halt */
  nop
  nop
  nop     
.Ltmp1:

/* end of file */
