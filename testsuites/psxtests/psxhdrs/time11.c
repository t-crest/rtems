/*
 *  This test file is used to verify that the header files associated with
 *  the callout are correct.
 *
 *  COPYRIGHT (c) 1989-1997. 1995, 1996.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may in
 *  the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id$
 */

#include <time.h>
 
void test( void )
{
  char     *buffer_pointer;
  char      buffer[ 80 ];
  time_t    time;
  
  buffer_pointer = ctime_r( &time, buffer );
}
