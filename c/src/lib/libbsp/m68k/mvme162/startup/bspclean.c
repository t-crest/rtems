/*
 *  This routine returns control to 162Bug.
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may in
 *  the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *
 *  Modifications of respective RTEMS file: COPYRIGHT (c) 1994.
 *  EISCAT Scientific Association. M.Savitski
 *
 *  This material is a part of the MVME162 Board Support Package
 *  for the RTEMS executive. Its licensing policies are those of the
 *  RTEMS above.
 *
 *  $Id$
 */

#include <rtems.h>
#include <bsp.h>
#include <z8036.h>
#include <page_table.h>

void bsp_return_to_monitor_trap()
{
  extern void start( void  );

  page_table_teardown();

  lcsr->intr_ena = 0;               /* disable interrupts */
  m68k_set_vbr(0xFFE00000);         /* restore 162Bug vectors */
  asm volatile( "trap   #15"  );    /* trap to 162Bug */
  asm volatile( ".short 0x63" );    /* return to 162Bug (.RETURN) */
                                    /* restart program */
  /*
   *  This does not work on the 162....
   */
#if 0
  {  register volatile void *start_addr;

     start_addr = start;

     asm volatile ( "jmp %0@" : "=a" (start_addr) : "0" (start_addr) );
  }
#endif
}

void bsp_cleanup( void )
{
   M68Kvec[ 45 ] = bsp_return_to_monitor_trap;   /* install handler */
   asm volatile( "trap #13" );  /* insures SUPV mode */
}
