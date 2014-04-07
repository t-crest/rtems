/*
 *  Clock Tick Device Driver
 *
 *  This routine initializes Patmos timer 1 which used for the clock tick.
 *
 *  The tick frequency is directly programmed to the configured number of
 *  microseconds per tick.
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

#include <stdlib.h>

#include <rtems.h>
#include <bsp.h>

void Clock_exit( void );
rtems_isr Clock_isr( rtems_vector_number vector )__attribute__((naked,noinline));

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

/* usecs from RTEMS start to Install_clock routine */
uint64_t usecs_offset;


/*
 *  The previous ISR on this clock tick interrupt vector.
 */

rtems_isr_entry  Old_ticker;

void Clock_exit( void );

/*
 * Get the current RTC microsecond value
 */
uint64_t get_cpu_usecs(void) {
  unsigned ulo, uhi;

  _iodev_ptr_t hi_usec = (_iodev_ptr_t)(__PATMOS_RTC_TIME_UP_ADDR);
  _iodev_ptr_t lo_usec = (_iodev_ptr_t)(__PATMOS_RTC_TIME_LOW_ADDR);

  // Order is important here
  ulo = *lo_usec;
  uhi = *hi_usec;

  return (((unsigned long long) uhi) << 32) | ulo;
}

/*
 * Set the timeout for the clock timer. The RTC will trigger an interrupt once
 * the cycle counter reaches the given value.
 */

static inline void arm_usec_timer(uint64_t timestamp) {

  _iodev_ptr_t hi_usec = (_iodev_ptr_t)(__PATMOS_RTC_TIME_UP_ADDR);
  _iodev_ptr_t lo_usec = (_iodev_ptr_t)(__PATMOS_RTC_TIME_LOW_ADDR);

  // Order is important here
  *lo_usec = (unsigned)timestamp;
  *hi_usec = (unsigned)(timestamp>>32);
}

static inline void set_usec_timer(uint64_t time_warp) {
  arm_usec_timer(get_cpu_usecs() + time_warp);
}

uint32_t bsp_clock_nanoseconds_since_last_tick(void)
{
	uint64_t nsecs = (get_cpu_usecs() - usecs_offset - Clock_driver_ticks*rtems_configuration_get_microseconds_per_tick())*1000;

	return (uint32_t) nsecs;
}

/*
 * 	Problem: Interrupts overlap and, consequently, variable Clock_driver_ticks is not
 * 			updated between consecutive isr. Thus, bsp_clock_nanoseconds_since_last_tick
 * 			overestimates the elapsed nanoseconds since last tick
 *
 * 	Solution:	(1) Comment the #define below
 * 				(2)	Increase CONFIGURE_MICROSECONDS_PER_TICK
 */

//#define Clock_driver_nanoseconds_since_last_tick bsp_clock_nanoseconds_since_last_tick

/*
 * C code to be executed in Clock_isr goes here
 * This function cannot be inlined
 */

void __attribute__ ((noinline)) Clock_isr_no_inline(void){

	set_usec_timer(rtems_configuration_get_microseconds_per_tick());

	/*
	 *  Accurate count of ISRs
	 */
	Clock_driver_ticks += 1;

	rtems_clock_tick();
}


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
	 * do not need to save the shadow stack pointer r31
	 * do not need to save the stack spill pointer s5, given that s5 = s6
	 */
	asm volatile("sub $r31 = $r31, %0 \n\t"				//offset shadow stack pointer
			"swc   [ $r31 + %1 ]  = $r1 \n\t"			//save r1
			"swc   [ $r31 + %2 ]  = $r2 \n\t"			//save r2
			"swc   [ $r31 + %3 ]  = $r3 \n\t"			//save r3
			"swc   [ $r31 + %4 ]  = $r4 \n\t"			//save r4
			"swc   [ $r31 + %5 ]  = $r5 \n\t"			//save r5
			"swc   [ $r31 + %6 ]  = $r6 \n\t"			//save r6
			"swc   [ $r31 + %7 ]  = $r7 \n\t"			//save r7
			"swc   [ $r31 + %8 ]  = $r8 \n\t"			//save r8
			"swc   [ $r31 + %9 ]  = $r9 \n\t"			//save r9
			"swc   [ $r31 + %10 ] = $r10 \n\t"			//save r10
			"swc   [ $r31 + %11 ] = $r11 \n\t"			//save r11
			"swc   [ $r31 + %12 ] = $r12 \n\t"			//save r12
			"swc   [ $r31 + %13 ] = $r13 \n\t"			//save r13
			"swc   [ $r31 + %14 ] = $r14 \n\t"			//save r14
			"swc   [ $r31 + %15 ] = $r15 \n\t"			//save r15
			"swc   [ $r31 + %16 ] = $r16 \n\t"			//save r16
			"swc   [ $r31 + %17 ] = $r17 \n\t"			//save r17
			"swc   [ $r31 + %18 ] = $r18 \n\t"			//save r18
			"swc   [ $r31 + %19 ] = $r19 \n\t"			//save r19
			"swc   [ $r31 + %20 ] = $r20 \n\t"			//save r20
			"swc   [ $r31 + %21 ] = $r21 \n\t"			//save r21
			"swc   [ $r31 + %22 ] = $r22 \n\t"			//save r22
			"swc   [ $r31 + %23 ] = $r23 \n\t"			//save r23
			"swc   [ $r31 + %24 ] = $r24 \n\t"			//save r24
			"swc   [ $r31 + %25 ] = $r25 \n\t"			//save r25
			"swc   [ $r31 + %26 ] = $r26 \n\t"			//save r26
			"swc   [ $r31 + %27 ] = $r27 \n\t"			//save r27
			"swc   [ $r31 + %28 ] = $r28 \n\t"			//save r28
			"swc   [ $r31 + %29 ] = $r29 \n\t"			//save r29
			"swc   [ $r31 + %30 ] = $r30 \n\t"			//save r30
			"mfs $r5 = $ss \n\t"
			"mfs $r6 = $st \n\t"
			"sub $r2 = $r5, $r6 \n\t"					// get stack size
			"sspill $r2 \n\t"
			"swc   [ $r31 + %31 ] = $r2 \n\t"			//save stack size to memory
			"mfs $r1 = $s0 \n\t"						//move s0 to r1
			"swc   [ $r31 + %32 ] = $r1 \n\t"	 		//save s0
			"mfs $r1 = $s1 \n\t"						//move s1 to r1
			"swc   [ $r31 + %33 ] = $r1 \n\t"			//save s1
			"mfs $r1 = $s2 \n\t"						//move s2 to r1
			"swc   [ $r31 + %34 ] = $r1 \n\t"			//save s2
			"mfs $r1 = $s3 \n\t"						//move s3 to r1
			"swc   [ $r31 + %35 ] = $r1 \n\t"			//save s3
			"mfs $r1 = $s4 \n\t"						//move s4 to r1
			"swc   [ $r31 + %36 ] = $r1 \n\t"			//save s4
			"mfs $r1 = $s6 \n\t"						//move s6 to r1
			"swc   [ $r31 + %37 ] = $r1 \n\t"			//save s6
			"mfs $r1 = $s7 \n\t"						//move s7 to r1
			"swc   [ $r31 + %38 ] = $r1 \n\t"			//save s7
			"mfs $r1 = $s8 \n\t"						//move s8 to r1
			"swc   [ $r31 + %39 ] = $r1 \n\t"			//save s8
			"mfs $r1 = $s9 \n\t"						//move s9 to r1
			"swc   [ $r31 + %40 ] = $r1 \n\t"			//save s9
			"mfs $r1 = $s10 \n\t"						//move s10 to r1
			"swc   [ $r31 + %41 ] = $r1 \n\t"			//save s10
			"mfs $r1 = $s11 \n\t"						//move s11 to r1
			"swc   [ $r31 + %42 ] = $r1 \n\t"			//save s11
			"mfs $r1 = $s12 \n\t"						//move s12 to r1
			"swc   [ $r31 + %43 ] = $r1 \n\t"			//save s12
			"mfs $r1 = $s13 \n\t"						//move s13 to r1
			"swc   [ $r31 + %44 ] = $r1 \n\t"			//save s13
			"mfs $r1 = $s14 \n\t"						//move s14 to r1
			"swc   [ $r31 + %45 ] = $r1 \n\t"			//save s14
			"mfs $r1 = $s15 \n\t"						//move s15 to r1
			"swc   [ $r31 + %46 ] = $r1 \n\t"			//save s15
			"li    $r2 = %47 \n\t"
			"lwl   $r1  = [ $r2 + 0 ] \n\t nop \n\t"	//load exceptions status register to r1
			"swc   [ $r31 + %48 ] = $r1 \n\t"	 		//save exceptions status register
			: : "i" (ISR_CONTEXT_CONTROL_SIZE), "i" (ISR_r1_OFFSET),"i" (ISR_r2_OFFSET), "i" (ISR_r3_OFFSET), "i" (ISR_r4_OFFSET),
			"i" (ISR_r5_OFFSET), "i" (ISR_r6_OFFSET), "i" (ISR_r7_OFFSET), "i" (ISR_r8_OFFSET), "i" (ISR_r9_OFFSET),
			"i" (ISR_r10_OFFSET), "i" (ISR_r11_OFFSET), "i" (ISR_r12_OFFSET), "i" (ISR_r13_OFFSET), "i" (ISR_r14_OFFSET),
			"i" (ISR_r15_OFFSET), "i" (ISR_r16_OFFSET), "i" (ISR_r17_OFFSET), "i" (ISR_r18_OFFSET), "i" (ISR_r19_OFFSET),
			"i" (ISR_r20_OFFSET), "i" (ISR_r21_OFFSET), "i" (ISR_r22_OFFSET), "i" (ISR_r23_OFFSET), "i" (ISR_r24_OFFSET),
			"i" (ISR_r25_OFFSET), "i" (ISR_r26_OFFSET), "i" (ISR_r27_OFFSET), "i" (ISR_r28_OFFSET), "i" (ISR_r29_OFFSET),
			"i" (ISR_r30_OFFSET), "i" (ISR_ssize_OFFSET), "i" (ISR_s0_OFFSET), "i" (ISR_s1_OFFSET), "i" (ISR_s2_OFFSET),
			"i" (ISR_s3_OFFSET), "i" (ISR_s4_OFFSET), "i" (ISR_s6_OFFSET), "i" (ISR_s7_OFFSET), "i" (ISR_s8_OFFSET),
			"i" (ISR_s9_OFFSET), "i" (ISR_s10_OFFSET), "i" (ISR_s11_OFFSET), "i" (ISR_s12_OFFSET), "i" (ISR_s13_OFFSET),
			"i" (ISR_s14_OFFSET), "i" (ISR_s15_OFFSET), "i" (&_excunit_base), "i" (ISR_exc_OFFSET));


	Clock_isr_no_inline();

	/*
	 * load general-purpose registers from the shadow stack
	 * load special-purpose registers from the shadow stack cache
	 */
	asm volatile("lwc   $r3  = [ $r31 + %0 ] \n\t"			//load r3
			"lwc   $r4  = [ $r31 + %1 ] \n\t"				//load r4
			"lwc   $r5  = [ $r31 + %2 ] \n\t"				//load r5
			"lwc   $r6  = [ $r31 + %3 ] \n\t"				//load r6
			"lwc   $r7  = [ $r31 + %4 ] \n\t"				//load r7
			"lwc   $r8  = [ $r31 + %5 ] \n\t"				//load r8
			"lwc   $r9  = [ $r31 + %6 ] \n\t"				//load r9
			"lwc   $r10 = [ $r31 + %7 ] \n\t"				//load r10
			"lwc   $r11 = [ $r31 + %8 ] \n\t"				//load r11
			"lwc   $r12 = [ $r31 + %9 ] \n\t"				//load r12
			"lwc   $r13 = [ $r31 + %10 ] \n\t"				//load r13
			"lwc   $r14 = [ $r31 + %11 ] \n\t"				//load r14
			"lwc   $r15 = [ $r31 + %12 ] \n\t"				//load r15
			"lwc   $r16 = [ $r31 + %13 ] \n\t"				//load r16
			"lwc   $r17 = [ $r31 + %14 ] \n\t"				//load r17
			"lwc   $r18 = [ $r31 + %15 ] \n\t"				//load r18
			"lwc   $r19 = [ $r31 + %16 ] \n\t"				//load r19
			"lwc   $r20 = [ $r31 + %17 ] \n\t"				//load r20
			"lwc   $r21 = [ $r31 + %18 ] \n\t"				//load r21
			"lwc   $r22 = [ $r31 + %19 ] \n\t"				//load r22
			"lwc   $r23 = [ $r31 + %20 ] \n\t"				//load r23
			"lwc   $r24 = [ $r31 + %21 ] \n\t"				//load r24
			"lwc   $r25 = [ $r31 + %22 ] \n\t"				//load r25
			"lwc   $r26 = [ $r31 + %23 ] \n\t"				//load r26
			"lwc   $r27 = [ $r31 + %24 ] \n\t"				//load r27
			"lwc   $r28 = [ $r31 + %25 ] \n\t"				//load r28
			"lwc   $r29 = [ $r31 + %26 ] \n\t"				//load r29
			"lwc   $r30 = [ $r31 + %27 ] \n\t"				//load r30
			"lwc   $r1  = [ $r31 + %28 ] \n\t nop \n\t"		//load s0
			"mts $s0 = $r1 \n\t"							//move r1 to s0
			"lwc   $r1  = [ $r31 + %29 ] \n\t nop \n\t"		//load s1
			"mts $s1 = $r1 \n\t"							//move r1 to s1
			"lwc   $r1  = [ $r31 + %30 ] \n\t nop \n\t"		//load s2
			"mts $s2 = $r1 \n\t"							//move r1 to s2
			"lwc   $r1  = [ $r31 + %31 ] \n\t nop \n\t"		//load s3
			"mts $s3 = $r1 \n\t"							//move r1 to s3
			"lwc   $r1  = [ $r31 + %32 ] \n\t nop \n\t"		//load s4
			"mts $s4 = $r1 \n\t"							//move r1 to s4
			"lwc   $r1  = [ $r31 + %33 ] \n\t nop \n\t"		//load s6
			"mts $s5 = $r1 \n\t"							//set the spill pointer to top, we fill back with sens
			"mts $s6 = $r1 \n\t"							//move r1 to st
			"lwc   $r1  = [ $r31 + %34 ] \n\t nop \n\t"		//load s7
			"mts $s7 = $r1 \n\t"							//move r1 to s7
			"lwc   $r1  = [ $r31 + %35 ] \n\t nop \n\t"		//load s8
			"mts $s8 = $r1 \n\t"							//move r1 to s8
			"lwc   $r1  = [ $r31 + %36 ] \n\t nop \n\t"		//load s9
			"mts $s9 = $r1 \n\t"							//move r1 to s9
			"lwc   $r1  = [ $r31 + %37 ] \n\t nop \n\t"		//load s10
			"mts $s10 = $r1 \n\t"							//move r1 to s10
			"lwc   $r1  = [ $r31 + %38 ] \n\t nop \n\t"		//load s11
			"mts $s11 = $r1 \n\t"							//move r1 to s11
			"lwc   $r1  = [ $r31 + %39 ] \n\t nop \n\t"		//load s12
			"mts $s12 = $r1 \n\t"							//move r1 to s12
			"lwc   $r1  = [ $r31 + %40 ] \n\t nop \n\t"		//load s13
			"mts $s13 = $r1 \n\t"							//move r1 to s13
			"lwc   $r1  = [ $r31 + %41 ] \n\t nop \n\t"		//load s14
			"mts $s14 = $r1 \n\t"							//move r1 to s14
			"lwc   $r1  = [ $r31 + %42 ] \n\t nop \n\t"		//load s15
			"mts $s15 = $r1 \n\t"							//move r1 to s15
			"lwc   $r1  = [ $r31 + %43 ] \n\t nop \n\t"		//load ssize
			"sens $r1 \n\t"									//ensure the stack size in the stack cache
			"lwc   $r1  = [ $r31 + %44 ] \n\t"				//load exceptions status register
			"li    $r2 = %45 \n\t"
			"swl   [ $r2 + 0 ] = $r1 \n\t"	 				//restore exceptions status register
			"xret \n\t"										//return to sxb, sxo
			"lwc   $r1  = [ $r31 + %46 ] \n\t"				//load r1
			"lwc   $r2  = [ $r31 + %47 ] \n\t"				//load r2
			"add $r31 = $r31, %48 \n\t"						// reset shadow stack pointer
			: : "i" (ISR_r3_OFFSET), "i" (ISR_r4_OFFSET), "i" (ISR_r5_OFFSET), "i" (ISR_r6_OFFSET), "i" (ISR_r7_OFFSET),
			"i" (ISR_r8_OFFSET), "i" (ISR_r9_OFFSET), "i" (ISR_r10_OFFSET), "i" (ISR_r11_OFFSET), "i" (ISR_r12_OFFSET),
			"i" (ISR_r13_OFFSET), "i" (ISR_r14_OFFSET), "i" (ISR_r15_OFFSET), "i" (ISR_r16_OFFSET), "i" (ISR_r17_OFFSET),
			"i" (ISR_r18_OFFSET), "i" (ISR_r19_OFFSET), "i" (ISR_r20_OFFSET), "i" (ISR_r21_OFFSET), "i" (ISR_r22_OFFSET),
			"i" (ISR_r23_OFFSET), "i" (ISR_r24_OFFSET), "i" (ISR_r25_OFFSET), "i" (ISR_r26_OFFSET), "i" (ISR_r27_OFFSET),
			"i" (ISR_r28_OFFSET), "i" (ISR_r29_OFFSET),	"i" (ISR_r30_OFFSET), "i" (ISR_s0_OFFSET), "i" (ISR_s1_OFFSET),
			"i" (ISR_s2_OFFSET), "i" (ISR_s3_OFFSET), "i" (ISR_s4_OFFSET), "i" (ISR_s6_OFFSET), "i" (ISR_s7_OFFSET),
			"i" (ISR_s8_OFFSET), "i" (ISR_s9_OFFSET), "i" (ISR_s10_OFFSET), "i" (ISR_s11_OFFSET), "i" (ISR_s12_OFFSET),
			"i" (ISR_s13_OFFSET), "i" (ISR_s14_OFFSET), "i" (ISR_s15_OFFSET), "i" (ISR_ssize_OFFSET), "i" (ISR_exc_OFFSET),
			"i" (&_excunit_base), "i" (ISR_r1_OFFSET), "i" (ISR_r2_OFFSET), "i" (ISR_CONTEXT_CONTROL_SIZE));

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

	// set the isr routine
	set_exc_handler(EXC_INTR_USEC, (uint32_t)clock_isr);
	// clear pending flags
	intr_clear_all_pending();
	// unmask interrupt
	intr_unmask(EXC_INTR_USEC);
	// enable interrupts
	patmos_enable_interrupts(EXC_STATUS|CPU_MODES_INTERRUPT_MASK);

#if defined(Clock_driver_nanoseconds_since_last_tick)
	rtems_clock_set_nanoseconds_extension(
			Clock_driver_nanoseconds_since_last_tick
	);
#endif

	/*
	 * reset the cpu_usecs count to determine clock_nanoseconds_since_last_tick
	 */

	usecs_offset = get_cpu_usecs();

	set_usec_timer(rtems_configuration_get_microseconds_per_tick());

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
	/* turn off the timer interrupts */

	patmos_disable_interrupts();

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
