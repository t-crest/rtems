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

#include <pthread.h>
 
#ifndef _POSIX_THREADS
#error "rtems is supposed to have pthread_condattr_setpshared"
#endif
#ifndef _POSIX_THREAD_PROCESS_SHARED
#error "rtems is supposed to have pthread_condattr_setpshared"
#endif

void test( void )
{
  pthread_condattr_t attribute;
  int                pshared;
  int                result;

  pshared = PTHREAD_PROCESS_SHARED;
  pshared = PTHREAD_PROCESS_PRIVATE;

  result = pthread_condattr_setpshared( &attribute, pshared );
}
