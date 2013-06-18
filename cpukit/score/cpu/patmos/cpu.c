/*
 *  PATMOS Dependent Source
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/rtems/cache.h>

/*
 *  This initializes the set of opcodes placed in each trap
 *  table entry.  The routine which installs a handler is responsible
 *  for filling in the fields for the _handler address and the _vector
 *  trap type.
 *
 *  The constants following this structure are masks for the fields which
 *  must be filled in when the handler is installed.
 */

const CPU_Trap_table_entry _CPU_Trap_slot_template = {
  0xa1480000,      /* mov   %psr, %l0           */
  0x29000000,      /* sethi %hi(_handler), %l4  */
  0x81c52000,      /* jmp   %l4 + %lo(_handler) */
  0xa6102000       /* mov   _vector, %l3        */
};

/*PAGE
 *
 *  _CPU_Initialize
 *
 *  This routine performs processor dependent initialization.
 *
 *  INPUT PARAMETERS: NONE
 *
 *  Output Parameters: NONE
 *
 *  NOTE: There is no need to save the pointer to the thread dispatch routine.
 *        The PATMOS's assembly code can reference it directly with no problems.
 */

void _CPU_Initialize(void)
{
}

/*PAGE
 *
 *  _CPU_ISR_Get_level
 *
 *  Input Parameters: NONE
 *
 *  Output Parameters:
 *    returns the current interrupt level (PIL field of the PSR)
 */

uint32_t   _CPU_ISR_Get_level( void )
{
  return 0;
}

/*PAGE
 *
 *  _CPU_ISR_install_raw_handler
 *
 *  This routine installs the specified handler as a "raw" non-executive
 *  supported trap handler (a.k.a. interrupt service routine).
 *
 *  Input Parameters:
 *    vector      - trap table entry number plus synchronous
 *                    vs. asynchronous information
 *    new_handler - address of the handler to be installed
 *    old_handler - pointer to an address of the handler previously installed
 *
 *  Output Parameters: NONE
 *    *new_handler - address of the handler previously installed
 *
 *  NOTE:
 *
 *  On the SPARC, there are really only 256 vectors.  However, the executive
 *  has no easy, fast, reliable way to determine which traps are synchronous
 *  and which are asynchronous.  By default, synchronous traps return to the
 *  instruction which caused the interrupt.  So if you install a software
 *  trap handler as an executive interrupt handler (which is desirable since
 *  RTEMS takes care of window and register issues), then the executive needs
 *  to know that the return address is to the trap rather than the instruction
 *  following the trap.
 *
 *  So vectors 0 through 255 are treated as regular asynchronous traps which
 *  provide the "correct" return address.  Vectors 256 through 512 are assumed
 *  by the executive to be synchronous and to require that the return address
 *  be fudged.
 *
 *  If you use this mechanism to install a trap handler which must reexecute
 *  the instruction which caused the trap, then it should be installed as
 *  an asynchronous trap.  This will avoid the executive changing the return
 *  address.
 */

void _CPU_ISR_install_raw_handler(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
)
{ 
}

/*PAGE
 *
 *  _CPU_ISR_install_vector
 *
 *  This kernel routine installs the RTEMS handler for the
 *  specified vector.
 *
 *  Input parameters:
 *    vector       - interrupt vector number
 *    new_handler  - replacement ISR for this vector number
 *    old_handler  - pointer to former ISR for this vector number
 *
 *  Output parameters:
 *    *old_handler - former ISR for this vector number
 *
 */

void _CPU_ISR_install_vector(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
)
{   
}

/*PAGE
 *
 *  _CPU_Context_Initialize
 *
 *  This kernel routine initializes the basic non-FP context area associated
 *  with each thread.
 *
 *  Input parameters:
 *    the_context  - pointer to the context area
 *    stack_base   - address of memory for the SPARC
 *    size         - size in bytes of the stack area
 *    new_level    - interrupt level for this context area
 *    entry_point  - the starting execution point for this this context
 *    is_fp        - TRUE if this context is associated with an FP thread
 *
 *  Output parameters: NONE
 */

void _CPU_Context_Initialize(
                             Context_Control *the_context ,
                             uint32_t *stack_base ,
                             uint32_t size ,
                             uint32_t new_level ,
                             void *entry_point ,
                             uint32_t *shadow_stack_base
                             )
{
    /* highest "stack aligned" address */
    uint32_t stack_high;

    /* On CPUs with stacks which grow down (i.e. PATMOS), we build the stack
     * based on the stack_high address */

    /* add the to the stack address the size of the stack */
    stack_high = ((uint32_t) (stack_base) + size);

    /* then align it */
    stack_high &= ~(CPU_STACK_ALIGNMENT - 1);

    /* see the README in this directory for a diagram of the stack */

    /* set the entry point */
    the_context->s9 = ((uint32_t) entry_point) - 8;

    /* set the stack pointer */
    the_context->s6 = stack_high;  
	
	/* set the shadow stack pointer */
    the_context->r29 = (uint32_t)shadow_stack_base;  
}

void _CPU_Context_switch(
                            Context_Control *run ,
                            Context_Control *heir
                            )
{
	/* save general-purpose registers (skip r0 which is always 0) 
	 * address of the previous task is passed as function argument in register r3*/	
	asm volatile("\tsres %0 \n\t" 						//reserve space in cache
				 "swc   [ $r3 + %1 ]  = $r1 \n\t"		//save r1
				 "swc   [ $r3 + %2 ]  = $r2 \n\t"		//save r2
				 "swc   [ $r3 + %3 ]  = $r3 \n\t"		//save r3
				 "swc   [ $r3 + %4 ]  = $r4 \n\t"		//save r4
				 "swc   [ $r3 + %5 ]  = $r5 \n\t"		//save r5
				 "swc   [ $r3 + %6 ]  = $r6 \n\t"		//save r6
				 "swc   [ $r3 + %7 ]  = $r7 \n\t"		//save r7
				 "swc   [ $r3 + %8 ]  = $r8 \n\t"		//save r8
				 "swc   [ $r3 + %9 ]  = $r9 \n\t"		//save r9
				 "swc   [ $r3 + %10 ] = $r10 \n\t"		//save r10
				 "swc   [ $r3 + %11 ] = $r11 \n\t"		//save r11
				 "swc   [ $r3 + %12 ] = $r12 \n\t"		//save r12
				 "swc   [ $r3 + %13 ] = $r13 \n\t"		//save r13
				 "swc   [ $r3 + %14 ] = $r14 \n\t"		//save r14
				 "swc   [ $r3 + %15 ] = $r15 \n\t"		//save r15
				 "swc   [ $r3 + %16 ] = $r16 \n\t"		//save r16
				 "swc   [ $r3 + %17 ] = $r17 \n\t"		//save r17
				 "swc   [ $r3 + %18 ] = $r18 \n\t"		//save r18
				 "swc   [ $r3 + %19 ] = $r19 \n\t"		//save r19
				 "swc   [ $r3 + %20 ] = $r20 \n\t"		//save r20
				 "swc   [ $r3 + %21 ] = $r21 \n\t"		//save r21
				 "swc   [ $r3 + %22 ] = $r22 \n\t"		//save r22
				 "swc   [ $r3 + %23 ] = $r23 \n\t"		//save r23
				 "swc   [ $r3 + %24 ] = $r24 \n\t"		//save r24
				 "swc   [ $r3 + %25 ] = $r25 \n\t"		//save r25
				 "swc   [ $r3 + %26 ] = $r26 \n\t"		//save r26
				 "swc   [ $r3 + %27 ] = $r27 \n\t"		//save r27
				 "swc   [ $r3 + %28 ] = $r28 \n\t"		//save r28
				 "swc   [ $r3 + %29 ] = $r29 \n\t"		//save r29
				 "swc   [ $r3 + %30 ] = $r30 \n\t"		//save r30
				 "swc   [ $r3 + %31 ] = $r31 \n\t"		//save r31
				 : : "i" (CONTEXT_CONTROL_SIZE), "i" (r1_OFFSET),"i" (r2_OFFSET), "i" (r3_OFFSET),
				 "i" (r4_OFFSET), "i" (r5_OFFSET), "i" (r6_OFFSET), "i" (r7_OFFSET), "i" (r8_OFFSET),
				 "i" (r9_OFFSET), "i" (r10_OFFSET), "i" (r11_OFFSET), "i" (r12_OFFSET), "i" (r13_OFFSET),
				 "i" (r14_OFFSET), "i" (r15_OFFSET), "i" (r16_OFFSET), "i" (r17_OFFSET), "i" (r18_OFFSET),
				 "i" (r19_OFFSET), "i" (r20_OFFSET), "i" (r21_OFFSET), "i" (r22_OFFSET), "i" (r23_OFFSET),
				 "i" (r23_OFFSET), "i" (r24_OFFSET), "i" (r25_OFFSET), "i" (r26_OFFSET), "i" (r27_OFFSET),
				 "i" (r28_OFFSET), "i" (r29_OFFSET), "i" (r30_OFFSET), "i" (r31_OFFSET));
		
	/* save special-purpose registers 
	 * use r1 as intermediate register to save special-purpose registers (no instruction to do it directly)*/
	 asm volatile("mfs $r1 = $s0 \n\t"					//move s0 to r1
				  "swc   [ $r3 + %0 ] = $r1 \n\t"	 	//save s0
				  "mfs $r1 = $s1 \n\t"					//move s1 to r1
				  "swc   [ $r3 + %1 ] = $r1 \n\t"		//save s1
				  "mfs $r1 = $s2 \n\t"					//move s2 to r1
				  "swc   [ $r3 + %2 ] = $r1 \n\t"		//save s2
				  "mfs $r1 = $s3 \n\t"					//move s3 to r1
				  "swc   [ $r3 + %3 ] = $r1 \n\t"		//save s3
				  "mfs $r1 = $s4 \n\t"					//move s4 to r1
				  "swc   [ $r3 + %4 ] = $r1 \n\t"		//save s4
				  "mfs $r1 = $s5 \n\t"					//move s5 to r1
				  "swc   [ $r3 + %5 ] = $r1 \n\t"		//save s5
				  "mfs $r1 = $s6 \n\t"					//move s6 to r1
				  "swc   [ $r3 + %6 ] = $r1 \n\t"		//save s6
				  "mfs $r1 = $s7 \n\t"					//move s7 to r1
				  "swc   [ $r3 + %7 ] = $r1 \n\t"		//save s7
				  "mfs $r1 = $s8 \n\t"					//move s8 to r1
				  "swc   [ $r3 + %8 ] = $r1 \n\t"		//save s8
				  "mfs $r1 = $s9 \n\t"					//move s9 to r1
				  "swc   [ $r3 + %9 ] = $r1 \n\t"		//save s9
				  "mfs $r1 = $s10 \n\t"					//move s10 to r1
				  "swc   [ $r3 + %10 ] = $r1 \n\t"		//save s10
				  "mfs $r1 = $s11 \n\t"					//move s11 to r1
				  "swc   [ $r3 + %11 ] = $r1 \n\t"		//save s11
				  "mfs $r1 = $s12 \n\t"					//move s12 to r1
				  "swc   [ $r3 + %12 ] = $r1 \n\t"		//save s12
				  "mfs $r1 = $s13 \n\t"					//move s13 to r1
				  "swc   [ $r3 + %13 ] = $r1 \n\t"		//save s13
				  "mfs $r1 = $s14 \n\t"					//move s14 to r1
				  "swc   [ $r3 + %14 ] = $r1 \n\t"		//save s14
				  "mfs $r1 = $s15 \n\t"					//move s15 to r1
				  "swc   [ $r3 + %15 ] = $r1 \n\t"		//save s15				 
				 : : "i" (s0_OFFSET), "i" (s1_OFFSET), "i" (s2_OFFSET), "i" (s3_OFFSET),
				 "i" (s4_OFFSET), "i" (s5_OFFSET), "i" (s6_OFFSET), "i" (s7_OFFSET),
				 "i" (s8_OFFSET), "i" (s9_OFFSET), "i" (s10_OFFSET),"i" (s11_OFFSET),
				 "i" (s12_OFFSET), "i" (s13_OFFSET), "i" (s14_OFFSET), "i" (s15_OFFSET));
				 
	/* load general-purpose registers (skip r0 which is always 0) 
	 * address of the current task is passed as function argument in register r4
	 * r4 is the last register to be loaded so that the memory address of the current task is not lost */	
	asm volatile("lwc   $r2  = [ $r4 + %0 ] \n\t"		//load r2
				 "lwc   $r3  = [ $r4 + %1 ] \n\t"		//load r3				 
				 "lwc   $r5  = [ $r4 + %2 ] \n\t"		//load r5
				 "lwc   $r6  = [ $r4 + %3 ] \n\t"		//load r6
				 "lwc   $r7  = [ $r4 + %4 ] \n\t"		//load r7
				 "lwc   $r8  = [ $r4 + %5 ] \n\t"		//load r8
				 "lwc   $r9  = [ $r4 + %6 ] \n\t"		//load r9
				 "lwc   $r10 = [ $r4 + %7 ] \n\t"		//load r10
				 "lwc   $r11 = [ $r4 + %8 ] \n\t"		//load r11
				 "lwc   $r12 = [ $r4 + %9 ] \n\t"		//load r12
				 "lwc   $r13 = [ $r4 + %10 ] \n\t"		//load r13
				 "lwc   $r14 = [ $r4 + %11 ] \n\t"		//load r14
				 "lwc   $r15 = [ $r4 + %12 ] \n\t"		//load r15
				 "lwc   $r16 = [ $r4 + %13 ] \n\t"		//load r16
				 "lwc   $r17 = [ $r4 + %14 ] \n\t"		//load r17
				 "lwc   $r18 = [ $r4 + %15 ] \n\t"		//load r18
				 "lwc   $r19 = [ $r4 + %16 ] \n\t"		//load r19
				 "lwc   $r20 = [ $r4 + %17 ] \n\t"		//load r20
				 "lwc   $r21 = [ $r4 + %18 ] \n\t"		//load r21
				 "lwc   $r22 = [ $r4 + %19 ] \n\t"		//load r22
				 "lwc   $r23 = [ $r4 + %20 ] \n\t"		//load r23
				 "lwc   $r24 = [ $r4 + %21 ] \n\t"		//load r24
				 "lwc   $r25 = [ $r4 + %22 ] \n\t"		//load r25
				 "lwc   $r26 = [ $r4 + %23 ] \n\t"		//load r26
				 "lwc   $r27 = [ $r4 + %24 ] \n\t"		//load r27
				 "lwc   $r28 = [ $r4 + %25 ] \n\t"		//load r28
				 "lwc   $r29 = [ $r4 + %26 ] \n\t"		//load r29
				 "lwc   $r30 = [ $r4 + %27 ] \n\t"		//load r30
				 "lwc   $r31 = [ $r4 + %28 ] \n\t"		//load r31
				  : : "i" (r2_OFFSET), "i" (r3_OFFSET), "i" (r5_OFFSET), "i" (r6_OFFSET), "i" (r7_OFFSET),
				 "i" (r8_OFFSET), "i" (r9_OFFSET), "i" (r10_OFFSET), "i" (r11_OFFSET), "i" (r12_OFFSET),
				 "i" (r13_OFFSET), "i" (r14_OFFSET), "i" (r15_OFFSET), "i" (r16_OFFSET), "i" (r17_OFFSET),
				 "i" (r18_OFFSET), "i" (r19_OFFSET), "i" (r20_OFFSET), "i" (r21_OFFSET), "i" (r22_OFFSET),
				 "i" (r23_OFFSET), "i" (r23_OFFSET), "i" (r24_OFFSET), "i" (r25_OFFSET), "i" (r26_OFFSET),
				 "i" (r27_OFFSET), "i" (r28_OFFSET), "i" (r29_OFFSET), "i" (r30_OFFSET), "i" (r31_OFFSET));
				 
	/* load special-purpose registers
	 * use r1 as intermediate register to load special-purpose registers (no instruction to do it directly)*/
	asm volatile("lwc   $r1  = [ $r4 + %0 ] \n\t"		//load s0
				 "mts $s0 = $r1 \n\t"					//move r1 to s0
				 "lwc   $r1  = [ $r4 + %1 ] \n\t"		//load s1
				 "mts $s1 = $r1 \n\t"					//move r1 to s1
				 "lwc   $r1  = [ $r4 + %2 ] \n\t"		//load s2
				 "mts $s2 = $r1 \n\t"					//move r1 to s2
				 "lwc   $r1  = [ $r4 + %3 ] \n\t"		//load s3
				 "mts $s3 = $r1 \n\t"					//move r1 to s3
				 "lwc   $r1  = [ $r4 + %4 ] \n\t"		//load s4
				 "mts $s4 = $r1 \n\t"					//move r1 to s4
				 "lwc   $r1  = [ $r4 + %5 ] \n\t"		//load s5
				 "mts $s5 = $r1 \n\t"					//move r1 to s5
				 "lwc   $r1  = [ $r4 + %6 ] \n\t"		//load s6
				 "mts $s6 = $r1 \n\t"					//move r1 to s6
				 "lwc   $r1  = [ $r4 + %7 ] \n\t"		//load s7
				 "mts $s7 = $r1 \n\t"					//move r1 to s7
				 "lwc   $r1 = [ $r4 + %8 ] \n\t"		//load s8
				 "mts $s8 = $r1 \n\t"					//move r1 to s8
				 "lwc   $r1 = [ $r4 + %9 ] \n\t"		//load s9
				 "mts $s9 = $r1 \n\t"					//move r1 to s9
				 "lwc   $r1 = [ $r4 + %10 ] \n\t"		//load s10
				 "mts $s10 = $r1 \n\t"					//move r1 to s10
				 "lwc   $r1 = [ $r4 + %11 ] \n\t"		//load s11
				 "mts $s11 = $r1 \n\t"					//move r1 to s11
				 "lwc   $r1 = [ $r4 + %12 ] \n\t"		//load s12
				 "mts $s12 = $r1 \n\t"					//move r1 to s12
				 "lwc   $r1 = [ $r4 + %13 ] \n\t"		//load s13
				 "mts $s13 = $r1 \n\t"					//move r1 to s13
				 "lwc   $r1 = [ $r4 + %14 ] \n\t"		//load s14
				 "mts $s14 = $r1 \n\t"					//move r1 to s14
				 "lwc   $r1 = [ $r4 + %15 ] \n\t"		//load s15
				 "mts $s15 = $r1 \n\t"					//move r1 to s15				 
				 "lwc   $r1  = [ $r4 + %16 ] \n\t"		//load r1
				 "lwc   $r4  = [ $r4 + %17] \n\t"		//load r4
				 "sfree %18 \n\t"						//free space in cache
				 : : "i" (s0_OFFSET), "i" (s1_OFFSET), "i" (s2_OFFSET), "i" (s3_OFFSET), "i" (s4_OFFSET),
				 "i" (s5_OFFSET), "i" (s6_OFFSET), "i" (s7_OFFSET), "i" (s8_OFFSET), "i" (s9_OFFSET),
				 "i" (s10_OFFSET), "i" (s11_OFFSET), "i" (s12_OFFSET), "i" (s13_OFFSET), "i" (s14_OFFSET),
				 "i" (s15_OFFSET), "i" (r1_OFFSET), "i" (r4_OFFSET), "i" (CONTEXT_CONTROL_SIZE));
				 
	 /* Return from the function */
	 asm volatile("ret $r30, $r31 \n\t"
				 : : );
				 				 				 				 
}		

void _CPU_Context_restore(
                             Context_Control *new_context
                             )
{

	/* load general-purpose registers (skip r0 which is always 0) 
	 * address of the current task is passed as function argument in register r3
	 * r3 is the last register to be loaded so that the memory address of the current task is not lost */	
	asm volatile("lwc   $r2  = [ $r3 + %0 ] \n\t"		//load r2
				 "lwc   $r4  = [ $r3 + %1 ] \n\t"		//load r4				 
				 "lwc   $r5  = [ $r3 + %2 ] \n\t"		//load r5
				 "lwc   $r6  = [ $r3 + %3 ] \n\t"		//load r6
				 "lwc   $r7  = [ $r3 + %4 ] \n\t"		//load r7
				 "lwc   $r8  = [ $r3 + %5 ] \n\t"		//load r8
				 "lwc   $r9  = [ $r3 + %6 ] \n\t"		//load r9
				 "lwc   $r10 = [ $r3 + %7 ] \n\t"		//load r10
				 "lwc   $r11 = [ $r3 + %8 ] \n\t"		//load r11
				 "lwc   $r12 = [ $r3 + %9 ] \n\t"		//load r12
				 "lwc   $r13 = [ $r3 + %10 ] \n\t"		//load r13
				 "lwc   $r14 = [ $r3 + %11 ] \n\t"		//load r14
				 "lwc   $r15 = [ $r3 + %12 ] \n\t"		//load r15
				 "lwc   $r16 = [ $r3 + %13 ] \n\t"		//load r16
				 "lwc   $r17 = [ $r3 + %14 ] \n\t"		//load r17
				 "lwc   $r18 = [ $r3 + %15 ] \n\t"		//load r18
				 "lwc   $r19 = [ $r3 + %16 ] \n\t"		//load r19
				 "lwc   $r20 = [ $r3 + %17 ] \n\t"		//load r20
				 "lwc   $r21 = [ $r3 + %18 ] \n\t"		//load r21
				 "lwc   $r22 = [ $r3 + %19 ] \n\t"		//load r22
				 "lwc   $r23 = [ $r3 + %20 ] \n\t"		//load r23
				 "lwc   $r24 = [ $r3 + %21 ] \n\t"		//load r24
				 "lwc   $r25 = [ $r3 + %22 ] \n\t"		//load r25
				 "lwc   $r26 = [ $r3 + %23 ] \n\t"		//load r26
				 "lwc   $r27 = [ $r3 + %24 ] \n\t"		//load r27
				 "lwc   $r28 = [ $r3 + %25 ] \n\t"		//load r28
				 "lwc   $r29 = [ $r3 + %26 ] \n\t"		//load r29
				 "lwc   $r30 = [ $r3 + %27 ] \n\t"		//load r30
				 "lwc   $r31 = [ $r3 + %28 ] \n\t"		//load r31
				  : : "i" (r2_OFFSET), "i" (r4_OFFSET), "i" (r5_OFFSET), "i" (r6_OFFSET), "i" (r7_OFFSET),
				 "i" (r8_OFFSET), "i" (r9_OFFSET), "i" (r10_OFFSET), "i" (r11_OFFSET), "i" (r12_OFFSET),
				 "i" (r13_OFFSET), "i" (r14_OFFSET), "i" (r15_OFFSET), "i" (r16_OFFSET), "i" (r17_OFFSET),
				 "i" (r18_OFFSET), "i" (r19_OFFSET), "i" (r20_OFFSET), "i" (r21_OFFSET), "i" (r22_OFFSET),
				 "i" (r23_OFFSET), "i" (r23_OFFSET), "i" (r24_OFFSET), "i" (r25_OFFSET), "i" (r26_OFFSET),
				 "i" (r27_OFFSET), "i" (r28_OFFSET), "i" (r29_OFFSET), "i" (r30_OFFSET), "i" (r31_OFFSET));
				 
	/* load special-purpose registers
	 * use r1 as intermediate register to load special-purpose registers (no instruction to do it directly)*/
	asm volatile("lwc   $r1  = [ $r3 + %0 ] \n\t"		//load s0
				 "mts $s0 = $r1 \n\t"					//move r1 to s0
				 "lwc   $r1  = [ $r3 + %1 ] \n\t"		//load s1
				 "mts $s1 = $r1 \n\t"					//move r1 to s1
				 "lwc   $r1  = [ $r3 + %2 ] \n\t"		//load s2
				 "mts $s2 = $r1 \n\t"					//move r1 to s2
				 "lwc   $r1  = [ $r3 + %3 ] \n\t"		//load s3
				 "mts $s3 = $r1 \n\t"					//move r1 to s3
				 "lwc   $r1  = [ $r3 + %4 ] \n\t"		//load s4
				 "mts $s4 = $r1 \n\t"					//move r1 to s4
				 "lwc   $r1  = [ $r3 + %5 ] \n\t"		//load s5
				 "mts $s5 = $r1 \n\t"					//move r1 to s5
				 "lwc   $r1  = [ $r3 + %6 ] \n\t"		//load s6
				 "mts $s6 = $r1 \n\t"					//move r1 to s6
				 "lwc   $r1  = [ $r3 + %7 ] \n\t"		//load s7
				 "mts $s7 = $r1 \n\t"					//move r1 to s7
				 "lwc   $r1 = [ $r3 + %8 ] \n\t"		//load s8
				 "mts $s8 = $r1 \n\t"					//move r1 to s8
				 "lwc   $r1 = [ $r3 + %9 ] \n\t"		//load s9
				 "mts $s9 = $r1 \n\t"					//move r1 to s9
				 "lwc   $r1 = [ $r3 + %10 ] \n\t"		//load s10
				 "mts $s10 = $r1 \n\t"					//move r1 to s10
				 "lwc   $r1 = [ $r3 + %11 ] \n\t"		//load s11
				 "mts $s11 = $r1 \n\t"					//move r1 to s11
				 "lwc   $r1 = [ $r3 + %12 ] \n\t"		//load s12
				 "mts $s12 = $r1 \n\t"					//move r1 to s12
				 "lwc   $r1 = [ $r3 + %13 ] \n\t"		//load s13
				 "mts $s13 = $r1 \n\t"					//move r1 to s13
				 "lwc   $r1 = [ $r3 + %14 ] \n\t"		//load s14
				 "mts $s14 = $r1 \n\t"					//move r1 to s14
				 "lwc   $r1 = [ $r3 + %15 ] \n\t"		//load s15
				 "mts $s15 = $r1 \n\t"					//move r1 to s15				 
				 "lwc   $r1  = [ $r3 + %16 ] \n\t"		//load r1
				 "lwc   $r3  = [ $r3 + %17] \n\t"		//load r3
				 "sfree %18 \n\t"						//free space in cache
				 : : "i" (s0_OFFSET), "i" (s1_OFFSET), "i" (s2_OFFSET), "i" (s3_OFFSET), "i" (s4_OFFSET),
				 "i" (s5_OFFSET), "i" (s6_OFFSET), "i" (s7_OFFSET), "i" (s8_OFFSET), "i" (s9_OFFSET),
				 "i" (s10_OFFSET), "i" (s11_OFFSET), "i" (s12_OFFSET), "i" (s13_OFFSET), "i" (s14_OFFSET),
				 "i" (s15_OFFSET), "i" (r1_OFFSET), "i" (r3_OFFSET), "i" (CONTEXT_CONTROL_SIZE));
				 
	 /* Return from the function */
	 asm volatile("ret $r30, $r31 \n\t"
				 : : );

}							 

void abort_trap() __attribute__((used))
{
}

