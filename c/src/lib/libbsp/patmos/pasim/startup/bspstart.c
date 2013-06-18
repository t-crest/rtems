/*
 *  This set of routines starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before any of these are invoked.
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
#include <rtems/bspIo.h>

/*
 * Tells us if data cache snooping is available
 */
int CPU_SPARC_HAS_SNOOPING;

/*
 * set_snooping
 *
 * Read the data cache configuration register to determine if
 * bus snooping is available. This is needed for some drivers so
 * that they can select the most efficient copy routines.
 *
 */

static inline int set_snooping(void)
{
  int tmp;
  asm(" lda [%1] 2, %0 "
      : "=r"(tmp)
      : "r"(0xC)
  );
  return (tmp >> 27) & 1;
}

/*
 *  bsp_start
 *
 *  This routine does the bulk of the system initialization.
 */
void bsp_start( void )
{  
}
