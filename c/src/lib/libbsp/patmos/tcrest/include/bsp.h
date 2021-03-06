/*  bsp.h
 *
 *  This include file contains all Patmos hardware definitions.
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

#ifndef _BSP_H
#define _BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Patmos CPU variant: TCREST */
#define TCREST 2

/* Constants */

/*
 *  Information placed in the linkcmds file.
 */

#define RAM_START 0x400
#define RAM_SIZE 1M
#define RAM_END RAM_START + RAM_SIZE

#define PROM_START 0x00000000
#define PROM_SIZE 0K
#define PROM_END PROM_START + PROM_SIZE

#define __PATMOS_CPUINFO_BASE   0xF0000000
#define __PATMOS_UART_BASE      0xF0080000
#define __PATMOS_TIMER_BASE     0xF0020000
#define __PATMOS_EXCUNIT_BASE   0xF0010000

#ifndef ASM

#include <bspopts.h>
#include <rtems.h>
#include <rtems/clockdrv.h>
#include <rtems/console.h>

extern int   CLOCK_SPEED;

extern int   end;        /* last address in the program */

/* miscellaneous stuff assumed to exist */

rtems_isr_entry set_vector(                     /* returns old vector */
    rtems_isr_entry     handler,                /* isr routine        */
    rtems_vector_number vector,                 /* vector number      */
    int                 type                    /* RTEMS or RAW intr  */
);

void BSP_fatal_return( void );

void bsp_spurious_initialize( void );

#endif /* !ASM */

#ifdef __cplusplus
}
#endif

#include <tcrest.h>

#endif


