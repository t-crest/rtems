/*
 *  Exceptions
 *
 *  This module implements functions to handle exceptions in Patmos
 *
 *
 *  Project: T-CREST - Time-Predictable Multi-Core Architecture for Embedded Systems
 *
 *  Copyright (C) GMVIS Skysoft S.A., 2014
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

#include "exceptions.h"

/*
 * Enable interrupts
 */
void patmos_enable_interrupts(void){
  EXC_STATUS |= 1;
}

/*
 * Disable interrupts
 */
void patmos_disable_interrupts(void){
  EXC_STATUS &= ~1;
}

/*
 * Register a function as exception handler.
 *   n - The exception to register the function for
 *   fun - The exception handler
 */
void set_exc_handler(unsigned n, exc_handler_t fun){
  EXC_VEC(n) = (unsigned)fun;
}

/*
 * Clear the pending flag of all interrupts
 */
void intr_clear_all_pending(void){
  EXC_PEND = 0;
}

/*
 * Unmask a particular interrupts
 * 	 n - The interrupt to be unmasked
 */
void intr_unmask(unsigned n){
  EXC_MASK |= (1 << n);
}
