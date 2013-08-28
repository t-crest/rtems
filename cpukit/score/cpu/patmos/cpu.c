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
#include <bsp.h>

/*PAGE
 *
 *  _CPU_Initialize
 *
 *  This routine performs processor dependent initialization.
 *
 *  Input Parameters: NONE
 *
 *  Output Parameters: NONE
 *
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
 *    stack_base   - address of memory for the Patmos stack
 *    size         - size in bytes of the stack area
 *    new_level    - interrupt level for this context area
 *    entry_point  - the starting execution point for this this context
 *    shadow_stack_base   - address of memory for the Patmos shadow stack
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
	uint32_t     stack_high;  /* highest "stack aligned" address */

	/*
	 *  On CPUs with stacks which grow down (i.e. PATMOS), we build the stack
	 *  based on the stack_high address.
	 */

	stack_high = ((uint32_t)(stack_base) + size);
	stack_high &= ~(CPU_STACK_ALIGNMENT - 1);

	/* set the shadow stack pointer */
	the_context->r29 = (uint32_t)shadow_stack_base;

	/* set the stack pointer and spill pointer */
	the_context->s5 = stack_high;
	the_context->s6 = stack_high;

	/* set the return address */
	the_context->r30 = (uint32_t)entry_point;

	/* set the stack size */
	the_context->ssize = 0;
}

void _CPU_Context_switch(
		Context_Control *run ,
		Context_Control *heir
)
{
	/* 
	 * save general-purpose registers (skip r0 which is always 0) 
	 * address of the previous task is passed as function argument in register r3
	 */	
	asm volatile("swm   [ %0 + %1 ]  = $r1 \n\t"	//save r1
			"swm   [ %0 + %2 ]  = $r2 \n\t"			//save r2
			"swm   [ %0 + %3 ]  = $r3 \n\t"			//save r3
			"swm   [ %0 + %4 ]  = $r4 \n\t"			//save r4
			"swm   [ %0 + %5 ]  = $r5 \n\t"			//save r5
			"swm   [ %0 + %6 ]  = $r6 \n\t"			//save r6
			"swm   [ %0 + %7 ]  = $r7 \n\t"			//save r7
			"swm   [ %0 + %8 ]  = $r8 \n\t"			//save r8
			"swm   [ %0 + %9 ]  = $r9 \n\t"			//save r9
			"swm   [ %0 + %10 ] = $r10 \n\t"		//save r10
			"swm   [ %0 + %11 ] = $r11 \n\t"		//save r11
			"swm   [ %0 + %12 ] = $r12 \n\t"		//save r12
			"swm   [ %0 + %13 ] = $r13 \n\t"		//save r13
			"swm   [ %0 + %14 ] = $r14 \n\t"		//save r14
			"swm   [ %0 + %15 ] = $r15 \n\t"		//save r15
			"swm   [ %0 + %16 ] = $r16 \n\t"		//save r16
			"swm   [ %0 + %17 ] = $r17 \n\t"		//save r17
			"swm   [ %0 + %18 ] = $r18 \n\t"		//save r18
			"swm   [ %0 + %19 ] = $r19 \n\t"		//save r19
			"swm   [ %0 + %20 ] = $r20 \n\t"		//save r20
			"swm   [ %0 + %21 ] = $r21 \n\t"		//save r21
			"swm   [ %0 + %22 ] = $r22 \n\t"		//save r22
			"swm   [ %0 + %23 ] = $r23 \n\t"		//save r23
			"swm   [ %0 + %24 ] = $r24 \n\t"		//save r24
			"swm   [ %0 + %25 ] = $r25 \n\t"		//save r25
			"swm   [ %0 + %26 ] = $r26 \n\t"		//save r26
			"swm   [ %0 + %27 ] = $r27 \n\t"		//save r27
			"swm   [ %0 + %28 ] = $r28 \n\t"		//save r28
			"swm   [ %0 + %29 ] = $r29 \n\t"		//save r29
			"swm   [ %0 + %30 ] = $r30 \n\t"		//save r30
			"swm   [ %0 + %31 ] = $r31 \n\t"		//save r31
			: : "r" (run), "i" (r1_OFFSET),"i" (r2_OFFSET), "i" (r3_OFFSET), "i" (r4_OFFSET),
			"i" (r5_OFFSET), "i" (r6_OFFSET), "i" (r7_OFFSET), "i" (r8_OFFSET), "i" (r9_OFFSET),
			"i" (r10_OFFSET), "i" (r11_OFFSET), "i" (r12_OFFSET), "i" (r13_OFFSET), "i" (r14_OFFSET),
			"i" (r15_OFFSET), "i" (r16_OFFSET), "i" (r17_OFFSET), "i" (r18_OFFSET), "i" (r19_OFFSET),
			"i" (r20_OFFSET), "i" (r21_OFFSET), "i" (r22_OFFSET), "i" (r23_OFFSET), "i" (r24_OFFSET),
			"i" (r25_OFFSET), "i" (r26_OFFSET), "i" (r27_OFFSET), "i" (r28_OFFSET), "i" (r29_OFFSET),
			"i" (r30_OFFSET), "i" (r31_OFFSET));

	/*
	 * copy the current stack to memory and save the stack size to the Context_Control struct in memory
	 */
	asm volatile("mfs $r5 = $s5 \n\t"
			"mfs $r6 = $s6 \n\t"
			"sub $r2 = $r5, $r6 \n\t"				// get stack size
			"sspill $r2 \n\t"
			"swm   [ %0 + %1 ] = $r2 \n\t"			//save stack size to memory
			: : "r" (run), "i" (ssize_OFFSET));

	/* 
	 * save special-purpose registers 
	 * use r1 as intermediate register to save special-purpose registers (no instruction to do it directly)
	 */
	asm volatile("mfs $r1 = $s0 \n\t"				//move s0 to r1
			"swm   [ %0 + %1 ] = $r1 \n\t"	 		//save s0
			"mfs $r1 = $s1 \n\t"					//move s1 to r1
			"swm   [ %0 + %2 ] = $r1 \n\t"			//save s1
			"mfs $r1 = $s2 \n\t"					//move s2 to r1
			"swm   [ %0 + %3 ] = $r1 \n\t"			//save s2
			"mfs $r1 = $s3 \n\t"					//move s3 to r1
			"swm   [ %0 + %4 ] = $r1 \n\t"			//save s3
			"mfs $r1 = $s4 \n\t"					//move s4 to r1
			"swm   [ %0 + %5 ] = $r1 \n\t"			//save s4
			"mfs $r1 = $s5 \n\t"					//move s5 to r1
			"swm   [ %0 + %6 ] = $r1 \n\t"			//save s5
			"mfs $r1 = $s6 \n\t"					//move s6 to r1
			"swm   [ %0 + %7 ] = $r1 \n\t"			//save s6
			"mfs $r1 = $s7 \n\t"					//move s7 to r1
			"swm   [ %0 + %8 ] = $r1 \n\t"			//save s7
			"mfs $r1 = $s8 \n\t"					//move s8 to r1
			"swm   [ %0 + %9 ] = $r1 \n\t"			//save s8
			"mfs $r1 = $s9 \n\t"					//move s9 to r1
			"swm   [ %0 + %10 ] = $r1 \n\t"			//save s9
			"mfs $r1 = $s10 \n\t"					//move s10 to r1
			"swm   [ %0 + %11 ] = $r1 \n\t"			//save s10
			"mfs $r1 = $s11 \n\t"					//move s11 to r1
			"swm   [ %0 + %12 ] = $r1 \n\t"			//save s11
			"mfs $r1 = $s12 \n\t"					//move s12 to r1
			"swm   [ %0 + %13 ] = $r1 \n\t"			//save s12
			"mfs $r1 = $s13 \n\t"					//move s13 to r1
			"swm   [ %0 + %14 ] = $r1 \n\t"			//save s13
			"mfs $r1 = $s14 \n\t"					//move s14 to r1
			"swm   [ %0 + %15 ] = $r1 \n\t"			//save s14
			"mfs $r1 = $s15 \n\t"					//move s15 to r1
			"swm   [ %0 + %16 ] = $r1 \n\t"			//save s15
			: : "r" (run), "i" (s0_OFFSET), "i" (s1_OFFSET), "i" (s2_OFFSET), "i" (s3_OFFSET),
			"i" (s4_OFFSET), "i" (s5_OFFSET), "i" (s6_OFFSET), "i" (s7_OFFSET), "i" (s8_OFFSET),
			"i" (s9_OFFSET), "i" (s10_OFFSET),"i" (s11_OFFSET), "i" (s12_OFFSET), "i" (s13_OFFSET),
			"i" (s14_OFFSET), "i" (s15_OFFSET));

	/* 
	 * load general-purpose registers (skip r0 which is always 0) 
	 * address of the current task is passed as function argument in register r4
	 * r4 is the last register to be loaded so that the memory address of the current task is not lost
	 * r1 will be used as auxiliary register, so it is not loaded yet
	 */	
	asm volatile("lwc   $r2  = [ %0 + %1 ] \n\t"	//load r2
			"lwc   $r3  = [ %0 + %2 ] \n\t"			//load r3
			"lwc   $r5  = [ %0 + %3 ] \n\t"			//load r5
			"lwc   $r6  = [ %0 + %4 ] \n\t"			//load r6
			"lwc   $r7  = [ %0 + %5 ] \n\t"			//load r7
			"lwc   $r8  = [ %0 + %6 ] \n\t"			//load r8
			"lwc   $r9  = [ %0 + %7 ] \n\t"			//load r9
			"lwc   $r10 = [ %0 + %8 ] \n\t"			//load r10
			"lwc   $r11 = [ %0 + %9 ] \n\t"			//load r11
			"lwc   $r12 = [ %0 + %10 ] \n\t"		//load r12
			"lwc   $r13 = [ %0 + %11 ] \n\t"		//load r13
			"lwc   $r14 = [ %0 + %12 ] \n\t"		//load r14
			"lwc   $r15 = [ %0 + %13 ] \n\t"		//load r15
			"lwc   $r16 = [ %0 + %14 ] \n\t"		//load r16
			"lwc   $r17 = [ %0 + %15 ] \n\t"		//load r17
			"lwc   $r18 = [ %0 + %16 ] \n\t"		//load r18
			"lwc   $r19 = [ %0 + %17 ] \n\t"		//load r19
			"lwc   $r20 = [ %0 + %18 ] \n\t"		//load r20
			"lwc   $r21 = [ %0 + %19 ] \n\t"		//load r21
			"lwc   $r22 = [ %0 + %20 ] \n\t"		//load r22
			"lwc   $r23 = [ %0 + %21 ] \n\t"		//load r23
			"lwc   $r24 = [ %0 + %22 ] \n\t"		//load r24
			"lwc   $r25 = [ %0 + %23 ] \n\t"		//load r25
			"lwc   $r26 = [ %0 + %24 ] \n\t"		//load r26
			"lwc   $r27 = [ %0 + %25 ] \n\t"		//load r27
			"lwc   $r28 = [ %0 + %26 ] \n\t"		//load r28
			"lwc   $r29 = [ %0 + %27 ] \n\t"		//load r29
			"lwc   $r30 = [ %0 + %28 ] \n\t"		//load r30
			"lwc   $r31 = [ %0 + %29 ] \n\t"		//load r31
			: : "r" (heir), "i" (r2_OFFSET), "i" (r3_OFFSET), "i" (r5_OFFSET), "i" (r6_OFFSET),
			"i" (r7_OFFSET), "i" (r8_OFFSET), "i" (r9_OFFSET), "i" (r10_OFFSET), "i" (r11_OFFSET),
			"i" (r12_OFFSET), "i" (r13_OFFSET), "i" (r14_OFFSET), "i" (r15_OFFSET), "i" (r16_OFFSET),
			"i" (r17_OFFSET), "i" (r18_OFFSET), "i" (r19_OFFSET), "i" (r20_OFFSET), "i" (r21_OFFSET),
			"i" (r22_OFFSET), "i" (r23_OFFSET), "i" (r24_OFFSET), "i" (r25_OFFSET), "i" (r26_OFFSET),
			"i" (r27_OFFSET), "i" (r28_OFFSET), "i" (r29_OFFSET), "i" (r30_OFFSET), "i" (r31_OFFSET));

	/* 
	 * load special-purpose registers
	 * use r1 as intermediate register to load special-purpose registers (no instruction to do it directly)
	 */
	asm volatile("lwc   $r1  = [ %0 + %1 ] \n\t nop \n\t"	//load s0
			"mts $s0 = $r1 \n\t"							//move r1 to s0
			"lwc   $r1  = [ %0 + %2 ] \n\t nop \n\t"		//load s1
			"mts $s1 = $r1 \n\t"							//move r1 to s1
			"lwc   $r1  = [ %0 + %3 ] \n\t nop \n\t"		//load s2
			"mts $s2 = $r1 \n\t"							//move r1 to s2
			"lwc   $r1  = [ %0 + %4 ] \n\t nop \n\t"		//load s3
			"mts $s3 = $r1 \n\t"							//move r1 to s3
			"lwc   $r1  = [ %0 + %5 ] \n\t nop \n\t"		//load s4
			"mts $s4 = $r1 \n\t"							//move r1 to s4
			"lwc   $r1  = [ %0 + %6 ] \n\t nop \n\t"		//load s5
			"mts $s5 = $r1 \n\t"							//move r1 to s5
			"lwc   $r1  = [ %0 + %7 ] \n\t nop \n\t"		//load s6
			"mts $s6 = $r1 \n\t"							//move r1 to s6
			"lwc   $r1  = [ %0 + %8 ] \n\t nop \n\t"		//load s7
			"mts $s7 = $r1 \n\t"							//move r1 to s7
			"lwc   $r1  = [ %0 + %9 ] \n\t nop \n\t"		//load s8
			"mts $s8 = $r1 \n\t"							//move r1 to s8
			"lwc   $r1 = [ %0 + %10 ] \n\t nop \n\t"		//load s9
			"mts $s9 = $r1 \n\t"							//move r1 to s9
			"lwc   $r1 = [ %0 + %11 ] \n\t nop \n\t"		//load s10
			"mts $s10 = $r1 \n\t"							//move r1 to s10
			"lwc   $r1 = [ %0 + %12 ] \n\t nop \n\t"		//load s11
			"mts $s11 = $r1 \n\t"							//move r1 to s11
			"lwc   $r1 = [ %0 + %13 ] \n\t nop \n\t"		//load s12
			"mts $s12 = $r1 \n\t"							//move r1 to s12
			"lwc   $r1 = [ %0 + %14 ] \n\t nop \n\t"		//load s13
			"mts $s13 = $r1 \n\t"							//move r1 to s13
			"lwc   $r1 = [ %0 + %15 ] \n\t nop \n\t"		//load s14
			"mts $s14 = $r1 \n\t"							//move r1 to s14
			"lwc   $r1 = [ %0 + %16 ] \n\t nop \n\t"		//load s15
			"mts $s15 = $r1 \n\t"							//move r1 to s15
			: : "r" (heir), "i" (s0_OFFSET), "i" (s1_OFFSET), "i" (s2_OFFSET), "i" (s3_OFFSET),
			"i" (s4_OFFSET), "i" (s5_OFFSET), "i" (s6_OFFSET), "i" (s7_OFFSET), "i" (s8_OFFSET),
			"i" (s9_OFFSET), "i" (s10_OFFSET), "i" (s11_OFFSET), "i" (s12_OFFSET), "i" (s13_OFFSET),
			"i" (s14_OFFSET), "i" (s15_OFFSET));

	asm volatile("lwm   $r1  = [ %0 + %1 ] \n\t nop \n\t"	//load ssize
			"sens $r1 \n\t"									//ensure the stack size in the stack cache
			"lwc   $r1  = [ %0 + %2 ] \n\t"					//load r1
			"lwc   $r4  = [ %0 + %3 ] \n\t"					//load r4
			: : "r" (heir), "i" (ssize_OFFSET), "i" (r1_OFFSET), "i" (r4_OFFSET));

}		

void _CPU_Context_restore(
		Context_Control *new_context
)
{
	/* 
	 * load general-purpose registers (skip r0 which is always 0) 
	 * address of the current task is passed as function argument in register r3
	 * r3 is the last register to be loaded so that the memory address of the current task is not lost 
	 * r1 will be used as auxiliary register, so it is not loaded yet
	 */	
	asm volatile("lwc   $r2  = [ %0 + %1 ] \n\t"	//load r2
			"lwc   $r4  = [ %0 + %2 ] \n\t"			//load r4
			"lwc   $r5  = [ %0 + %3 ] \n\t"			//load r5
			"lwc   $r6  = [ %0 + %4 ] \n\t"			//load r6
			"lwc   $r7  = [ %0 + %5 ] \n\t"			//load r7
			"lwc   $r8  = [ %0 + %6 ] \n\t"			//load r8
			"lwc   $r9  = [ %0 + %7 ] \n\t"			//load r9
			"lwc   $r10 = [ %0 + %8 ] \n\t"			//load r10
			"lwc   $r11 = [ %0 + %9 ] \n\t"			//load r11
			"lwc   $r12 = [ %0 + %10 ] \n\t"		//load r12
			"lwc   $r13 = [ %0 + %11 ] \n\t"		//load r13
			"lwc   $r14 = [ %0 + %12 ] \n\t"		//load r14
			"lwc   $r15 = [ %0 + %13 ] \n\t"		//load r15
			"lwc   $r16 = [ %0 + %14 ] \n\t"		//load r16
			"lwc   $r17 = [ %0 + %15 ] \n\t"		//load r17
			"lwc   $r18 = [ %0 + %16 ] \n\t"		//load r18
			"lwc   $r19 = [ %0 + %17 ] \n\t"		//load r19
			"lwc   $r20 = [ %0 + %18 ] \n\t"		//load r20
			"lwc   $r21 = [ %0 + %19 ] \n\t"		//load r21
			"lwc   $r22 = [ %0 + %20 ] \n\t"		//load r22
			"lwc   $r23 = [ %0 + %21 ] \n\t"		//load r23
			"lwc   $r24 = [ %0 + %22 ] \n\t"		//load r24
			"lwc   $r25 = [ %0 + %23 ] \n\t"		//load r25
			"lwc   $r26 = [ %0 + %24 ] \n\t"		//load r26
			"lwc   $r27 = [ %0 + %25 ] \n\t"		//load r27
			"lwc   $r28 = [ %0 + %26 ] \n\t"		//load r28
			"lwc   $r29 = [ %0 + %27 ] \n\t"		//load r29
			"lwc   $r30 = [ %0 + %28 ] \n\t"		//load r30
			"lwc   $r31 = [ %0 + %29 ] \n\t"		//load r31
			: : "r" (new_context), "i" (r2_OFFSET), "i" (r4_OFFSET), "i" (r5_OFFSET), "i" (r6_OFFSET),
			"i" (r7_OFFSET), "i" (r8_OFFSET), "i" (r9_OFFSET), "i" (r10_OFFSET), "i" (r11_OFFSET),
			"i" (r12_OFFSET), "i" (r13_OFFSET), "i" (r14_OFFSET), "i" (r15_OFFSET), "i" (r16_OFFSET),
			"i" (r17_OFFSET), "i" (r18_OFFSET), "i" (r19_OFFSET), "i" (r20_OFFSET), "i" (r21_OFFSET),
			"i" (r22_OFFSET), "i" (r23_OFFSET), "i" (r24_OFFSET), "i" (r25_OFFSET), "i" (r26_OFFSET),
			"i" (r27_OFFSET), "i" (r28_OFFSET), "i" (r29_OFFSET), "i" (r30_OFFSET), "i" (r31_OFFSET));

	/*
	 * load special-purpose registers
	 * use r1 as intermediate register to load special-purpose registers (no instruction to do it directly)
	 */
	asm volatile("lwc   $r1  = [ %0 + %1 ] \n\t nop \n\t"	//load s0
			"mts $s0 = $r1 \n\t"							//move r1 to s0
			"lwc   $r1  = [ %0 + %2 ] \n\t nop \n\t"		//load s1
			"mts $s1 = $r1 \n\t"							//move r1 to s1
			"lwc   $r1  = [ %0 + %3 ] \n\t nop \n\t"		//load s2
			"mts $s2 = $r1 \n\t"							//move r1 to s2
			"lwc   $r1  = [ %0 + %4 ] \n\t nop \n\t"		//load s3
			"mts $s3 = $r1 \n\t"							//move r1 to s3
			"lwc   $r1  = [ %0 + %5 ] \n\t nop \n\t"		//load s4
			"mts $s4 = $r1 \n\t"							//move r1 to s4
			"lwc   $r1  = [ %0 + %6 ] \n\t nop \n\t"		//load s5
			"mts $s5 = $r1 \n\t"							//move r1 to s5
			"lwc   $r1  = [ %0 + %7 ] \n\t nop \n\t"		//load s6
			"mts $s6 = $r1 \n\t"							//move r1 to s6
			"lwc   $r1  = [ %0 + %8 ] \n\t nop \n\t"		//load s7
			"mts $s7 = $r1 \n\t"							//move r1 to s7
			"lwc   $r1  = [ %0 + %9 ] \n\t nop \n\t"		//load s8
			"mts $s8 = $r1 \n\t"							//move r1 to s8
			"lwc   $r1 = [ %0 + %10 ] \n\t nop \n\t"		//load s9
			"mts $s9 = $r1 \n\t"							//move r1 to s9
			"lwc   $r1 = [ %0 + %11 ] \n\t nop \n\t"		//load s10
			"mts $s10 = $r1 \n\t"							//move r1 to s10
			"lwc   $r1 = [ %0 + %12 ] \n\t nop \n\t"		//load s11
			"mts $s11 = $r1 \n\t"							//move r1 to s11
			"lwc   $r1 = [ %0 + %13 ] \n\t nop \n\t"		//load s12
			"mts $s12 = $r1 \n\t"							//move r1 to s12
			"lwc   $r1 = [ %0 + %14 ] \n\t nop \n\t"		//load s13
			"mts $s13 = $r1 \n\t"							//move r1 to s13
			"lwc   $r1 = [ %0 + %15 ] \n\t nop \n\t"		//load s14
			"mts $s14 = $r1 \n\t"							//move r1 to s14
			"lwc   $r1 = [ %0 + %16 ] \n\t nop \n\t"		//load s15
			"mts $s15 = $r1 \n\t"							//move r1 to s15
			: : "r" (new_context), "i" (s0_OFFSET), "i" (s1_OFFSET), "i" (s2_OFFSET), "i" (s3_OFFSET),
			"i" (s4_OFFSET), "i" (s5_OFFSET), "i" (s6_OFFSET), "i" (s7_OFFSET), "i" (s8_OFFSET),
			"i" (s9_OFFSET), "i" (s10_OFFSET), "i" (s11_OFFSET), "i" (s12_OFFSET), "i" (s13_OFFSET),
			"i" (s14_OFFSET), "i" (s15_OFFSET));

	asm volatile("lwm   $r1  = [ %0 + %1 ] \n\t nop \n\t"	//load ssize
			"sens $r1 \n\t"									//ensure the stack size in the stack cache
			"lwc   $r1  = [ %0 + %2 ] \n\t"					//load r1
			"lwc   $r3  = [ %0 + %3 ] \n\t"					//load r3
			: : "r" (new_context), "i" (ssize_OFFSET), "i" (r1_OFFSET), "i" (r3_OFFSET));

}

void abort_trap()
{
}

