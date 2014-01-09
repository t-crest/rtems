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

.type   hard_reset, @function
.size   hard_reset, .Ltmp0-hard_reset
.fstart hard_reset, .Ltmp0-hard_reset, 4
 
  /*
   *  This is the hard reset code.
   */
	PUBLIC(hard_reset)
SYM(hard_reset):

  and      $r0 = $r0, 0x0 /* Reset r0 to 0 */
	
  /*
   * setup stack cache and stack frame    
   */
  li	  $r29 = PASIM_SHADOW_STACK_BASE /* Initialize shadow stack pointer */
  li	  $r9 = PASIM_STACK_CACHE_BASE
  mts	  $s5 = $r9 /* Initialize the stack cache's top pointer */
  mts	  $s6 = $r9 /* Initialize the stack cache's spill top pointer */
  
 /*
  *  Copy the initialized data from ROM to RAM
  *
  *  FROM:   _etext
  *  TO:     _data_start
  *  LENGTH: (__bss_start - _data_start) bytes
  */
  
//  li	  $r9 = _etext /* r9 = start of initialized data in ROM */
//  li	  $r10 = _data_start /* r10 = start of initialized data in RAM */
//  li	  $r11 = _bss_start /* r11 = end of initialized data in RAM */
  
//  cmplt   $p2 = $r10, $r11         /* Is the pointer past the end of dest? */
//  (!$p2) br      clear_bss
//  nop
//  nop
//  nop

//copy_data:
//  lwc     $r12 = [ $r9 + 0 ]
//  swc     [ $r10 + 0 ] = $r12     /* copy this word */
//  add     $r9 = $r9, 4            /* bump the source pointer */
//  add     $r10 = $r10, 4          /* bump the destination pointer */
//  cmplt   $p2 = $r10, $r11         /* Is the pointer past the end of dest? */
//  ($p2) br      copy_data
//  nop
//  nop
//  nop

//clear_bss:	/* clear the bss */
//  li	  $r9 = _edata /* r9 = start of bss */
//  li	  $r10 = _end /* r10 = end of bss */
  
//  cmple   $p2 = $r9, $r10
//  (!$p2) br      call_boot
//  nop
//  nop
//  nop
  
//zero_bss:
//  swc     [ $r9 + 0 ] = $r0      /* zero the bss */
//  add     $r9 = $r9, 4           /* bump the bss pointer */
// cmple   $p2 = $r9, $r10
//  ($p2) br      zero_bss
//  nop
//  nop
  
  /*
   * invoke main function                    
   */
 
//call_boot:
  li	  $r30 = hard_reset /* set function return address */
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
  mov $r1 = $r3 /* store exit code */
  mov $r30 = $r0 /* clear sb and return == halt */
  ret $r0, $r0
  nop
  nop
  nop     
.Ltmp1:

/* end of file */
