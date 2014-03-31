/*
 *  start.s
 *
 *  Common start code for PATMOS.
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

.comm _loader_baseaddr,4,4
.comm _loader_off,4,4

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
  li      $r1 = _loader_baseaddr
  mfs     $r29 = $srb
  swm     [$r1] = $r29
  li      $r1 = _loader_off
  mfs     $r29 = $sro
  swm     [$r1] = $r29

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
  li      $r1 = _loader_baseaddr
  lwm     $r29 = [$r1]
  nop
  mts     $srb = $r29
  li      $r1 = _loader_off
  lwm     $r29 = [$r1]
  nop
  mts     $sro = $r29
  mov     $r1 = $r3 /* store exit code */
  ret     /* return to loader and/or halt */
  nop
  nop
  nop     
.Ltmp1:

/* end of file */
