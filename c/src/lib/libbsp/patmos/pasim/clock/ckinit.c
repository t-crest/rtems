/*
 *  Clock Tick Device Driver
 *
 *  This routine initializes PASIM timer 1 which used for the clock tick.
 *
 *  The tick frequency is directly programmed to the configured number of
 *  microseconds per tick.
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

#include <stdlib.h>

#include <rtems.h>
#include <bsp.h>

void Clock_exit( void );
rtems_isr Clock_isr( rtems_vector_number vector )__attribute__((naked));

/*
 *  Clock_driver_ticks is a monotonically increasing counter of the
 *  number of clock ticks since the driver was initialized.
 */

volatile uint32_t         Clock_driver_ticks;

/*
 *  Clock_isrs is the number of clock ISRs until the next invocation of
 *  the RTEMS clock tick routine.  The clock tick device driver
 *  gets an interrupt once a millisecond and counts down until the
 *  length of time between the user configured microseconds per tick
 *  has passed.
 */

uint32_t         Clock_isrs;              /* ISRs until next tick */

/*
 * These are set by clock driver during its init
 */

rtems_device_major_number rtems_clock_major = ~0;
rtems_device_minor_number rtems_clock_minor;
//uint32_t         usecs_offset;              /* microseconds from RTEMS start to Install_clock routine */

/*
 *  The previous ISR on this clock tick interrupt vector.
 */

rtems_isr_entry  Old_ticker;

void Clock_exit( void );

void set_cpu_cycles (u64 time_warp)
{

	__PATMOS_RTC_WR_CYCLE_LOW((unsigned int)time_warp);
	__PATMOS_RTC_WR_CYCLE_UP((unsigned int)(time_warp >> 32));

}

u64 get_cpu_cycles(void)
{
	unsigned int u;
	unsigned int l;

	__PATMOS_RTC_RD_CYCLE_LOW(l);
	__PATMOS_RTC_RD_CYCLE_UP(u);

	return (((u64)u) << 32) | l;
}

u64 get_cpu_time(void)
{
	unsigned int u;
	unsigned int l;

	__PATMOS_RTC_RD_TIME_LOW(l);
	__PATMOS_RTC_RD_TIME_UP(u);

	return (((u64)u) << 32) | l;
}

/*
uint32_t bsp_clock_nanoseconds_since_last_tick(void)
{
	uint32_t usecs;

	usecs = get_cpu_cycles()/PATMOS_FREQ_MHZ - Clock_driver_ticks*rtems_configuration_get_microseconds_per_tick() - usecs_offset;

	return usecs * 1000;
}

#define Clock_driver_nanoseconds_since_last_tick \
		bsp_clock_nanoseconds_since_last_tick
*/

/*
 *  Clock_isr
 *
 *  This is the clock tick interrupt handler.
 *
 *  Input parameters:
 *    vector - vector number
 *
 *  Output parameters:  NONE
 *
 *  Return values:      NONE
 */

rtems_isr Clock_isr(
		rtems_vector_number vector
)
{
	/*
	 * save general-purpose registers to the shadow stack
	 * copy the current stack to memory and save the stack size to the shadow stack
	 * save special-purpose registers to the shadow stack
	 * reset r0 and update function return address base and offset registers r30 and r31
	 */
	asm volatile("sub $r29 = $r29, %0 \n\t"			// offset shadow stack pointer
			"swm   [ $r29 + %1 ]  = $r0 \n\t"		//save r0
			"swm   [ $r29 + %2 ]  = $r1 \n\t"		//save r1
			"swm   [ $r29 + %3 ]  = $r2 \n\t"		//save r2
			"swm   [ $r29 + %4 ]  = $r3 \n\t"		//save r3
			"swm   [ $r29 + %5 ]  = $r4 \n\t"		//save r4
			"swm   [ $r29 + %6 ]  = $r5 \n\t"		//save r5
			"swm   [ $r29 + %7 ]  = $r6 \n\t"		//save r6
			"swm   [ $r29 + %8 ]  = $r7 \n\t"		//save r7
			"swm   [ $r29 + %9 ]  = $r8 \n\t"		//save r8
			"swm   [ $r29 + %10 ]  = $r9 \n\t"		//save r9
			"swm   [ $r29 + %11 ] = $r10 \n\t"		//save r10
			"swm   [ $r29 + %12 ] = $r11 \n\t"		//save r11
			"swm   [ $r29 + %13 ] = $r12 \n\t"		//save r12
			"swm   [ $r29 + %14 ] = $r13 \n\t"		//save r13
			"swm   [ $r29 + %15 ] = $r14 \n\t"		//save r14
			"swm   [ $r29 + %16 ] = $r15 \n\t"		//save r15
			"swm   [ $r29 + %17 ] = $r16 \n\t"		//save r16
			"swm   [ $r29 + %18 ] = $r17 \n\t"		//save r17
			"swm   [ $r29 + %19 ] = $r18 \n\t"		//save r18
			"swm   [ $r29 + %20 ] = $r19 \n\t"		//save r19
			"swm   [ $r29 + %21 ] = $r20 \n\t"		//save r20
			"swm   [ $r29 + %22 ] = $r21 \n\t"		//save r21
			"swm   [ $r29 + %23 ] = $r22 \n\t"		//save r22
			"swm   [ $r29 + %24 ] = $r23 \n\t"		//save r23
			"swm   [ $r29 + %25 ] = $r24 \n\t"		//save r24
			"swm   [ $r29 + %26 ] = $r25 \n\t"		//save r25
			"swm   [ $r29 + %27 ] = $r26 \n\t"		//save r26
			"swm   [ $r29 + %28 ] = $r27 \n\t"		//save r27
			"swm   [ $r29 + %29 ] = $r28 \n\t"		//save r28
			"swm   [ $r29 + %30 ] = $r29 \n\t"		//save r29
			"swm   [ $r29 + %31 ] = $r30 \n\t"		//save r30
			"swm   [ $r29 + %32 ] = $r31 \n\t"		//save r31
			"mfs $r5 = $s5 \n\t"
			"mfs $r6 = $s6 \n\t"
			"sub $r2 = $r5, $r6 \n\t"				// get stack size
			"sspill $r2 \n\t"
			"swm   [ $r29 + %33 ] = $r2 \n\t"		//save stack size to memory
			"mfs $r1 = $s0 \n\t"					//move s0 to r1
			"swm   [ $r29 + %34 ] = $r1 \n\t"	 	//save s0
			"mfs $r1 = $s1 \n\t"					//move s1 to r1
			"swm   [ $r29 + %35 ] = $r1 \n\t"		//save s1
			"mfs $r1 = $s2 \n\t"					//move s2 to r1
			"swm   [ $r29 + %36 ] = $r1 \n\t"		//save s2
			"mfs $r1 = $s3 \n\t"					//move s3 to r1
			"swm   [ $r29 + %37 ] = $r1 \n\t"		//save s3
			"mfs $r1 = $s4 \n\t"					//move s4 to r1
			"swm   [ $r29 + %38 ] = $r1 \n\t"		//save s4
			"mfs $r1 = $s5 \n\t"					//move s5 to r1
			"swm   [ $r29 + %39 ] = $r1 \n\t"		//save s5
			"mfs $r1 = $s6 \n\t"					//move s6 to r1
			"swm   [ $r29 + %40 ] = $r1 \n\t"		//save s6
			"mfs $r1 = $s7 \n\t"					//move s7 to r1
			"swm   [ $r29 + %41 ] = $r1 \n\t"		//save s7
			"mfs $r1 = $s8 \n\t"					//move s8 to r1
			"swm   [ $r29 + %42 ] = $r1 \n\t"		//save s8
			"mfs $r1 = $s9 \n\t"					//move s9 to r1
			"swm   [ $r29 + %43 ] = $r1 \n\t"		//save s9
			"mfs $r1 = $s10 \n\t"					//move s10 to r1
			"swm   [ $r29 + %44 ] = $r1 \n\t"		//save s10
			"mfs $r1 = $s11 \n\t"					//move s11 to r1
			"swm   [ $r29 + %45 ] = $r1 \n\t"		//save s11
			"mfs $r1 = $s12 \n\t"					//move s12 to r1
			"swm   [ $r29 + %46 ] = $r1 \n\t"		//save s12
			"mfs $r1 = $s13 \n\t"					//move s13 to r1
			"swm   [ $r29 + %47 ] = $r1 \n\t"		//save s13
			"mfs $r1 = $s14 \n\t"					//move s14 to r1
			"swm   [ $r29 + %48 ] = $r1 \n\t"		//save s14
			"mfs $r1 = $s15 \n\t"					//move s15 to r1
			"swm   [ $r29 + %49 ] = $r1 \n\t"		//save s15
			"and $r0 = $r0, 0\n\t"					//reset r0
			"li  $r30 = Clock_isr\n\t"				//set return address
			"li  $r31 = restore_context\n\t"
			: : "i" (CONTEXT_OFFSET), "i" (r0_OFFSET), "i" (r1_OFFSET),"i" (r2_OFFSET), "i" (r3_OFFSET),
			"i" (r4_OFFSET), "i" (r5_OFFSET), "i" (r6_OFFSET), "i" (r7_OFFSET), "i" (r8_OFFSET),
			"i" (r9_OFFSET), "i" (r10_OFFSET), "i" (r11_OFFSET), "i" (r12_OFFSET), "i" (r13_OFFSET),
			"i" (r14_OFFSET), "i" (r15_OFFSET), "i" (r16_OFFSET), "i" (r17_OFFSET), "i" (r18_OFFSET),
			"i" (r19_OFFSET), "i" (r20_OFFSET), "i" (r21_OFFSET), "i" (r22_OFFSET), "i" (r23_OFFSET),
			"i" (r24_OFFSET), "i" (r25_OFFSET), "i" (r26_OFFSET), "i" (r27_OFFSET), "i" (r28_OFFSET),
			"i" (r29_OFFSET), "i" (r30_OFFSET), "i" (r31_OFFSET), "i" (ssize_OFFSET), "i" (s0_OFFSET),
			"i" (s1_OFFSET), "i" (s2_OFFSET), "i" (s3_OFFSET), "i" (s4_OFFSET), "i" (s5_OFFSET),
			"i" (s6_OFFSET), "i" (s7_OFFSET), "i" (s8_OFFSET), "i" (s9_OFFSET), "i" (s10_OFFSET),
			"i" (s11_OFFSET), "i" (s12_OFFSET), "i" (s13_OFFSET), "i" (s14_OFFSET), "i" (s15_OFFSET));

	/*
	 *  Accurate count of ISRs
	 */
	Clock_driver_ticks += 1;

	__PATMOS_RTC_WR_INTERVAL(rtems_configuration_get_microseconds_per_tick() * PATMOS_FREQ_MHZ);

	rtems_clock_tick();

	/*
	 * load general-purpose registers from the shadow stack
	 * load special-purpose registers from the shadow stack cache
	 */
	asm volatile("restore_context: \n\t"
			"lwc   $r0  = [ $r29 + %0 ] \n\t"				//load r0
			"lwc   $r3  = [ $r29 + %1 ] \n\t"				//load r3
			"lwc   $r4  = [ $r29 + %2 ] \n\t"				//load r4
			"lwc   $r5  = [ $r29 + %3 ] \n\t"				//load r5
			"lwc   $r6  = [ $r29 + %4 ] \n\t"				//load r6
			"lwc   $r7  = [ $r29 + %5 ] \n\t"				//load r7
			"lwc   $r8  = [ $r29 + %6 ] \n\t"				//load r8
			"lwc   $r9  = [ $r29 + %7 ] \n\t"				//load r9
			"lwc   $r10 = [ $r29 + %8 ] \n\t"				//load r10
			"lwc   $r11 = [ $r29 + %9 ] \n\t"				//load r11
			"lwc   $r12 = [ $r29 + %10 ] \n\t"				//load r12
			"lwc   $r13 = [ $r29 + %11 ] \n\t"				//load r13
			"lwc   $r14 = [ $r29 + %12 ] \n\t"				//load r14
			"lwc   $r15 = [ $r29 + %13 ] \n\t"				//load r15
			"lwc   $r16 = [ $r29 + %14 ] \n\t"				//load r16
			"lwc   $r17 = [ $r29 + %15 ] \n\t"				//load r17
			"lwc   $r18 = [ $r29 + %16 ] \n\t"				//load r18
			"lwc   $r19 = [ $r29 + %17 ] \n\t"				//load r19
			"lwc   $r20 = [ $r29 + %18 ] \n\t"				//load r20
			"lwc   $r21 = [ $r29 + %19 ] \n\t"				//load r21
			"lwc   $r22 = [ $r29 + %20 ] \n\t"				//load r22
			"lwc   $r23 = [ $r29 + %21 ] \n\t"				//load r23
			"lwc   $r24 = [ $r29 + %22 ] \n\t"				//load r24
			"lwc   $r25 = [ $r29 + %23 ] \n\t"				//load r25
			"lwc   $r26 = [ $r29 + %24 ] \n\t"				//load r26
			"lwc   $r27 = [ $r29 + %25 ] \n\t"				//load r27
			"lwc   $r28 = [ $r29 + %26 ] \n\t"				//load r28
			"lwc   $r30 = [ $r29 + %27 ] \n\t"				//load r30
			"lwc   $r31 = [ $r29 + %28 ] \n\t"				//load r31
			"lwc   $r1  = [ $r29 + %29 ] \n\t nop \n\t"		//load s0
			"mts $s0 = $r1 \n\t"							//move r1 to s0
			"lwc   $r1  = [ $r29 + %30 ] \n\t nop \n\t"		//load s1
			"mts $s1 = $r1 \n\t"							//move r1 to s1
			"lwc   $r1  = [ $r29 + %31 ] \n\t nop \n\t"		//load s2
			"mts $s2 = $r1 \n\t"							//move r1 to s2
			"lwc   $r1  = [ $r29 + %32 ] \n\t nop \n\t"		//load s3
			"mts $s3 = $r1 \n\t"							//move r1 to s3
			"lwc   $r1  = [ $r29 + %33 ] \n\t nop \n\t"		//load s4
			"mts $s4 = $r1 \n\t"							//move r1 to s4
			"lwc   $r1  = [ $r29 + %34 ] \n\t nop \n\t"		//load s5
			"mts $s5 = $r1 \n\t"							//move r1 to s5
			"lwc   $r1  = [ $r29 + %35 ] \n\t nop \n\t"		//load s6
			"mts $s6 = $r1 \n\t"							//move r1 to s6
			"lwc   $r1  = [ $r29 + %36 ] \n\t nop \n\t"		//load s7
			"mts $s7 = $r1 \n\t"							//move r1 to s7
			"lwc   $r1  = [ $r29 + %37 ] \n\t nop \n\t"		//load s8
			"mts $s8 = $r1 \n\t"							//move r1 to s8
			"lwc   $r1  = [ $r29 + %38 ] \n\t nop \n\t"		//load s11
			"mts $s11 = $r1 \n\t"							//move r1 to s11
			"lwc   $r1  = [ $r29 + %39 ] \n\t nop \n\t"		//load s12
			"mts $s12 = $r1 \n\t"							//move r1 to s12
			"lwc   $r1  = [ $r29 + %40 ] \n\t nop \n\t"		//load s13
			"mts $s13 = $r1 \n\t"							//move r1 to s13
			"lwc   $r1  = [ $r29 + %41 ] \n\t nop \n\t"		//load s14
			"mts $s14 = $r1 \n\t"							//move r1 to s14
			"lwc   $r1  = [ $r29 + %42 ] \n\t nop \n\t"		//load s15
			"mts $s15 = $r1 \n\t"							//move r1 to s15
			"lwm   $r1  = [ $r29 + %43 ] \n\t nop \n\t"		//load ssize
			"sens $r1 \n\t"									//ensure the stack size in the stack cache
			"lwc   $r1  = [ $r29 + %44 ] \n\t"				//load s9
			"lwc   $r2  = [ $r29 + %45 ] \n\t"				//load s10
			"mts $s9 = $r1 \n\t"							//move r1 to s9
			"mts $s10 = $r2 \n\t"							//move r2 to s10
			"lwc   $r1  = [ $r29 + %46 ] \n\t"				//load r1
			"ret $r1, $r2 \n\t"								//return to s9, s10
			"lwc   $r29 = [ $r29 + %47 ] \n\t"				//load r29
			"lwc   $r2  = [ $r29 + %48 ] \n\t"				//load r2
			"add $r29 = $r29, %49 \n\t"						// reset shadow stack pointer
			: : "i" (r0_OFFSET), "i" (r3_OFFSET), "i" (r4_OFFSET), "i" (r5_OFFSET),	"i" (r6_OFFSET),
			"i" (r7_OFFSET), "i" (r8_OFFSET), "i" (r9_OFFSET), "i" (r10_OFFSET), "i" (r11_OFFSET),
			"i" (r12_OFFSET), "i" (r13_OFFSET), "i" (r14_OFFSET), "i" (r15_OFFSET),	"i" (r16_OFFSET),
			"i" (r17_OFFSET), "i" (r18_OFFSET), "i" (r19_OFFSET), "i" (r20_OFFSET),	"i" (r21_OFFSET),
			"i" (r22_OFFSET), "i" (r23_OFFSET), "i" (r24_OFFSET), "i" (r25_OFFSET),	"i" (r26_OFFSET),
			"i" (r27_OFFSET), "i" (r28_OFFSET), "i" (r30_OFFSET),	"i" (r31_OFFSET), "i" (s0_OFFSET),
			"i" (s1_OFFSET), "i" (s2_OFFSET), "i" (s3_OFFSET),	"i" (s4_OFFSET), "i" (s5_OFFSET),
			"i" (s6_OFFSET), "i" (s7_OFFSET), "i" (s8_OFFSET),	"i" (s11_OFFSET), "i" (s12_OFFSET),
			"i" (s13_OFFSET),	"i" (s14_OFFSET), "i" (s15_OFFSET),	"i" (ssize_OFFSET),	"i" (s9_OFFSET),
			"i" (s10_OFFSET), "i" (r1_OFFSET), "i" (r29_OFFSET), "i" (r2_OFFSET), "i" (CONTEXT_OFFSET));

}

/*
 *  Install_clock
 *
 *  Install a clock tick handler and reprograms the chip.  This
 *  is used to initially establish the clock tick.
 */

void Install_clock(
		rtems_isr_entry clock_isr
)
{
	/*
	 *  Initialize the clock tick device driver variables
	 */

	Clock_driver_ticks = 0;
	Clock_isrs = rtems_configuration_get_microseconds_per_tick() / 1000;

	__PATMOS_RTC_WR_ISR((uint32_t)clock_isr);

#if defined(Clock_driver_nanoseconds_since_last_tick)
	rtems_clock_set_nanoseconds_extension(
			Clock_driver_nanoseconds_since_last_tick
	);
#endif

	__PATMOS_RTC_WR_INTERVAL(rtems_configuration_get_microseconds_per_tick() * PATMOS_FREQ_MHZ);

	/*
	 * reset the cpu_cycles count to determine clock_nanoseconds_since_last_tick
	 */
	//usecs_offset = get_cpu_cycles()/PATMOS_FREQ_MHZ;

	/*
	 *  Schedule the clock cleanup routine to execute if the application exits.
	 */

	atexit( Clock_exit );
}

/*
 *  Clean up before the application exits
 */

void Clock_exit( void )
{
	/* XXX: turn off the timer interrupts */

	/* XXX: If necessary, restore the old vector */
}

/*
 *  Clock_initialize
 *
 *  Device driver entry point for clock tick driver initialization.
 */

rtems_device_driver Clock_initialize(
		rtems_device_major_number major,
		rtems_device_minor_number minor,
		void *pargp
)
{
	Install_clock( Clock_isr );

	rtems_clock_major = major;
	rtems_clock_minor = minor;

	return RTEMS_SUCCESSFUL;
}
