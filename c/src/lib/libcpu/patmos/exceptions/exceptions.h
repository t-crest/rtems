/*
 *  Patmos Exception Management
 *
 *  Project: T-CREST - Time-Predictable Multi-Core Architecture for Embedded Systems
 *
 *  Copyright (C) GMVIS Skysoft S.A., 2014
 *  @author Andre Rocha
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#ifndef __PATMOS_EXCEPTIONS_h
#define __PATMOS_EXCEPTIONS_h

/*
 * The exception handler type
 */
typedef void (*exc_handler_t)(void);

/*
 * The exception vector array
 */
#define EXC_VEC(I) (((_IODEV exc_handler_t volatile * const)(&_excunit_base+0x80))[I])

/*
 * Various named exception vector entry numbers
 */
#define EXC_ILLEGAL_OP       0
#define EXC_ILLEGAL_ADDRESS  1
#define EXC_INTR_CLOCK       16
#define EXC_INTR_USEC        17

void patmos_enable_interrupts(uint32_t level);

uint32_t patmos_disable_interrupts(void);

void set_exc_handler(unsigned n, exc_handler_t fun);

void intr_clear_all_pending(void);

void intr_unmask(unsigned n);

#endif

/* end of include file */
