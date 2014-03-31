/*  timer.c
 *
 *  This file implements a benchmark timer using timer 2.
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Project: T-CREST - Time-Predictable Multi-Core Architecture for Embedded Systems
 *
 *  Copyright (C) GMVIS Skysoft S.A., 2013
 *  @author André Rocha
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */


#include <bsp.h>

bool benchmark_timer_find_average_overhead;

#define AVG_OVERHEAD      0  /* It typically takes 0 microseconds */
/*     to start/stop the timer. */
#define LEAST_VALID       1  /* Don't trust a value lower than this */

uint64_t timer_offset;

void benchmark_timer_initialize(void)
{
	/*
	 *  Timer runs long and accurate enough not to require an interrupt.
	 */
	timer_offset = get_cpu_usecs();
}

int benchmark_timer_read(void)
{
	uint64_t total;

	/*
	 *  Read the timer and see how many clicks it has been since we started.
	 */

	total = get_cpu_usecs() - timer_offset;

	if ( benchmark_timer_find_average_overhead == true )
		return total;          /* in one microsecond units */

	if ( total < LEAST_VALID )
		return 0;            /* below timer resolution */

	return total - AVG_OVERHEAD;
}

void benchmark_timer_disable_subtracting_average_overhead(
		bool find_flag
)
{
	benchmark_timer_find_average_overhead = find_flag;
}
