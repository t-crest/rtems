/*
 *  PASIM Spurious Trap Handler
 *
 *  This is just enough of a trap handler to let us know what
 *  the likely source of the trap was.
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

#include <bsp.h>

#include <rtems/bspIo.h>

/*
 *  bsp_spurious_handler
 *
 *  Print a message on the debug console and then die
 */

rtems_isr bsp_spurious_handler(
   rtems_vector_number trap,
   CPU_Interrupt_frame *isf
)
{  
  asm volatile( "mov 1, %g1; ta 0x0" );
}

/*
 *  bsp_spurious_initialize
 *
 *  Install the spurious handler for most traps. Note that set_vector()
 *  will unmask the corresponding asynchronous interrupt, so the initial
 *  interrupt mask is restored after the handlers are installed.
 */

void bsp_spurious_initialize()
{  
}
