/*  pasim.h
 *
 *  PASIM BSP data types and macros.
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

#ifndef _INCLUDE_PASIM_h
#define _INCLUDE_PASIM_h

#include <rtems/score/patmos.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
#define PASIM_SHADOW_STACK_BASE 0x4000
#define PASIM_STACK_CACHE_BASE 0x3000
*/
#define PASIM_SHADOW_STACK_BASE 0x4000000
#define PASIM_STACK_CACHE_BASE 0x3000000

#ifndef ASM

extern char _iomap_base; /* linker symbol giving the base address of the IO map address range */

#define _IODEV __attribute__((address_space(1)))

typedef _IODEV unsigned int volatile * const _iodev_ptr_t;

extern char _uart_status_base; /* linker symbol giving the address of the UART status register */

extern char _uart_data_base; /* linker symbol giving the address of the UART data register */

#endif

/*
 *  The following defines the bits in the PASIM UART Status Registers.
 */

#define __PATMOS_UART_TRE 1 /* Bit mask for the transmit-ready bit (TRE) */
#define __PATMOS_UART_DAV 2 /* Bit mask for the data-available bit (DAV) */
#define __PATMOS_UART_PAE 4 /* Bit mask for the parity-error bit (PAE) */
#define __PATMOS_UART_TFL 8 /* Bit mask for the transmit-flush bit (TFL) */

#define __PATMOS_UART_STATUS_ADDR (&_uart_status_base) /* Address to access the status register of the UART coming with Patmos */
#define __PATMOS_UART_DATA_ADDR (&_uart_data_base) /* Address to access the data register of the UART coming with Patmos */

#ifndef ASM

/*
 *  Macros to handle read and write to the UART's status, control and data registers 
 */
 
/* Macro to read the UART's status register */
#define __PATMOS_UART_STATUS(res) res = *((_iodev_ptr_t)__PATMOS_UART_STATUS_ADDR);

/* Macro to read the UART's data register */
#define __PATMOS_UART_RD_DATA(res) res = *((_iodev_ptr_t)__PATMOS_UART_DATA_ADDR);

/* Macro to write the UART's control register */
#define __PATMOS_UART_WR_CTRL(data) *((_iodev_ptr_t)__PATMOS_UART_STATUS_ADDR) = data;
 
/* Macro to write the UART's data register */
#define __PATMOS_UART_WR_DATA(data) *((_iodev_ptr_t)__PATMOS_UART_DATA_ADDR) = data;

#endif /* !ASM */

#ifdef __cplusplus
}
#endif

#endif /* !_INCLUDE_PASIM_h */
/* end of include file */

