/*
 *  This file contains the TTY driver for the serial ports on Patmos.
 *
 *  This driver uses the termios pseudo driver.
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
#include <rtems/libio.h>
#include <stdlib.h>
#include <assert.h>

/*
 *  console_outbyte_polled
 *
 *  This routine transmits a character using polling.
 */
void console_outbyte_polled( 
  int           port,
  unsigned char ch
)
{
	int s;

	/*
	 * Wait for the UART to be ready for transmission
	 */
	do
	{
		__PATMOS_UART_STATUS(s);
	} while((s & __PATMOS_UART_TRE) == 0);

	/*
	 * Write data to the UART
	 */
	__PATMOS_UART_WR_DATA(ch);
}

/*
 *  console_inbyte_nonblocking
 *
 *  This routine polls for a character.
 */
int console_inbyte_nonblocking( int port )
{
	int s, c;

	/* wait for data to be available from the UART */
	do
	{
		__PATMOS_UART_STATUS(s);
	} while((s & (__PATMOS_UART_DAV | __PATMOS_UART_PAE)) == 0);

	/* read the data from the UART */
	__PATMOS_UART_RD_DATA(c);

	return c;
}

/* putchar/getchar for printk */
static void bsp_out_char(char c)
{
  console_outbyte_polled(0, c);
}

/*
 *  To support printk
 */

#include <rtems/bspIo.h>

BSP_output_char_function_type BSP_output_char = bsp_out_char;

static int bsp_in_char(void)
{
  int tmp;

  while ((tmp = console_inbyte_nonblocking(0)) < 0);
  return tmp;
}

BSP_polling_getchar_function_type BSP_poll_char = bsp_in_char;
