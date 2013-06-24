/*
 *  start.s
 *
 *  Common start code for PATMOS.
 *
 *  Project: T-CREST - Time-Predictable Multi-Core Architecture for Embedded Systems
 *
 *  Copyright (C) GMVIS Skysoft S.A., 2013
 *  @author André Rocha
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 * 
 */

#include <rtems/asm.h>
#include <pasim.h>
 
  /*
   *  This is the hard reset code.
   */
	PUBLIC(hard_reset)
SYM(hard_reset):

  /**
  * setup stack cache and stack frame    
  **/
  mov	  %r29 = PASIM_SHADOW_STACK_BASE /* Initialize shadow stack pointer */
  mts	  %s5 = PASIM_STACK_CACHE_BASE /* Initialize the stack cache's spill bottom pointer */
  mts	  %s6 = PASIM_STACK_CACHE_BASE /* Initialize the stack cache's spill top pointer */
  
  /*
   * invoke main function                    
   */
  
  li	  %r30 = PASIM_START /* set function base */  
  call	  SYM(boot_card) /* invoke boot_card function */
  li	  %r3 = 0 /* pass argument argc */
  li	  %r4 = 0 /* pass argument argv */
  li	  %r5 = 0 /* pass argument envp */
  call	  SYM(exit) /* terminate program and invoke exit */
  mov	  %r3 = %r1 /* get exit code (in first delay slot) */
  nop
  nop
  
  /*
   * Halt the CPU. If boot_card returns, also halt the CPU.
   */
        PUBLIC(exit)
SYM(exit):
  mov %r1 = %r3 /* store exit code */
  mov %r30 = %r0 /* clear sb and return == halt */
  ret %r0, %r0
  nop
  nop
  nop     

/* end of file */
