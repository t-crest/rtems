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
