/*  pasim.h
 *
 *  PASIM BSP data types and macros.
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

#ifndef _INCLUDE_PASIM_h
#define _INCLUDE_PASIM_h

#include <rtems/score/patmos.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TODO: replace these by linker symbols
 */

#define _shadow_stack_base 0x4000000
#define _stack_cache_base 0x3000000

#define __PATMOS_CPUINFO_BASE  0xF0000000
#define __PATMOS_UART_BASE     0xF0080000
#define __PATMOS_TIMER_BASE    0xF0020000
#define __PATMOS_EXCUNIT_BASE  0xF0010000

#ifndef ASM

#define _IODEV __attribute__((address_space(1)))

typedef _IODEV unsigned int volatile * const _iodev_ptr_t;


/*
 * CPU Info Management
 */

/* Address to access the CPU id */
#define __PATMOS_CPU_ID_ADDR (__PATMOS_CPUINFO_BASE + 0x00)

/* Address to access the CPU frequency */
#define __PATMOS_CPU_FREQ_ADDR (__PATMOS_CPUINFO_BASE + 0x04)

/* Macro to read the CPU id */
#define __PATMOS_CPU_RD_ID(res) res = *((_iodev_ptr_t)__PATMOS_CPU_ID_ADDR);

/* Macro to read the CPU frequency */
#define __PATMOS_CPU_RD_FREQ(res) res = *((_iodev_ptr_t)__PATMOS_CPU_FREQ_ADDR);

/*
 * End of CPU Info Management
 */


/*
 * UART Management
 */

/*
 *  The following defines the bits in the PASIM UART Status Registers.
 */

#define __PATMOS_UART_TRE 1 /* Bit mask for the transmit-ready bit (TRE) */
#define __PATMOS_UART_DAV 2 /* Bit mask for the data-available bit (DAV) */
#define __PATMOS_UART_PAE 4 /* Bit mask for the parity-error bit (PAE) */
#define __PATMOS_UART_TFL 8 /* Bit mask for the transmit-flush bit (TFL) */

/*
 * Address to access the status register of the UART coming with Patmos
 */
#define __PATMOS_UART_STATUS_ADDR (__PATMOS_UART_BASE + 0x00)

/*
 * Address to access the data register of the UART coming with Patmos
 */
#define __PATMOS_UART_DATA_ADDR (__PATMOS_UART_BASE + 0x04)


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

/*
 * End of UART Management
 */


/*
 * RTC Management
 */

extern uint64_t get_cpu_usecs (void);

/* Address to access the cycle counter low register of the RTC */
#define __PATMOS_RTC_CYCLE_LOW_ADDR (__PATMOS_TIMER_BASE + 0x04)

/* Address to access the cycle counter up register of the RTC */
#define __PATMOS_RTC_CYCLE_UP_ADDR (__PATMOS_TIMER_BASE + 0x00)

/* Address to access the time in microseconds low register of the RTC */
#define __PATMOS_RTC_TIME_LOW_ADDR (__PATMOS_TIMER_BASE + 0x0C)

/* Address to access the time in microseconds up register of the RTC */
#define __PATMOS_RTC_TIME_UP_ADDR (__PATMOS_TIMER_BASE + 0x08)

/* Address to access the ISR address register of the RTC cycle timer */
#define __PATMOS_RTC_ISR_ADDR (__PATMOS_EXCUNIT_BASE + 0xc0)

/* Macro to read the RTC's cycle counter low register of the RTC */
#define __PATMOS_RTC_RD_CYCLE_LOW(res) res = *((_iodev_ptr_t)__PATMOS_RTC_CYCLE_LOW_ADDR);

/* Macro to read the RTC's cycle counter up register of the RTC */
#define __PATMOS_RTC_RD_CYCLE_UP(res) res = *((_iodev_ptr_t)__PATMOS_RTC_CYCLE_UP_ADDR);

/* Macro to read the RTC's time in microseconds low register of the RTC */
#define __PATMOS_RTC_RD_TIME_LOW(res) res = *((_iodev_ptr_t)__PATMOS_RTC_TIME_LOW_ADDR);

/* Macro to read the RTC's time in microseconds up register of the RTC */
#define __PATMOS_RTC_RD_TIME_UP(res) res = *((_iodev_ptr_t)__PATMOS_RTC_TIME_UP_ADDR);

/* Macro to write the RTC's cycle counter low register */
#define __PATMOS_RTC_WR_CYCLE_LOW(val) *((_iodev_ptr_t)__PATMOS_RTC_CYCLE_LOW_ADDR) = val;

/* Macro to write the RTC's cycle counter up register */
#define __PATMOS_RTC_WR_CYCLE_UP(val) *((_iodev_ptr_t)__PATMOS_RTC_CYCLE_UP_ADDR) = val;

/* Macro to write the RTC's ISR address register */
#define __PATMOS_RTC_WR_ISR(address) *((_iodev_ptr_t)__PATMOS_RTC_ISR_ADDR) = address;

/*
 * End of RTC Management
 */


/*
 * Exception Management
 */

/* The status register of the exception unit */
#define EXC_STATUS (*((_iodev_ptr_t)(__PATMOS_EXCUNIT_BASE+0x00)))

/* The interrupt mask register */
#define EXC_MASK   (*((_iodev_ptr_t)(__PATMOS_EXCUNIT_BASE+0x04)))

/* The pending interrupts register */
#define EXC_PEND   (*((_iodev_ptr_t)(__PATMOS_EXCUNIT_BASE+0x08)))

/* The exception source register */
#define EXC_SOURCE (*((_iodev_ptr_t)(__PATMOS_EXCUNIT_BASE+0x0c)))

/*
 * End of Exception Management
 */

#endif /* !ASM */

#ifdef __cplusplus
}
#endif

#endif /* !_INCLUDE_PASIM_h */
/* end of include file */

