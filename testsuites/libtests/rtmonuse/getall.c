/*  Get_all_counters
 *
 *  This routine allows TA5 to atomically obtain the iteration counters.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may in
 *  the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  getall.c,v 1.3 1995/12/19 20:21:07 joel Exp
 */

#include "system.h"

void Get_all_counters()
{
  rtems_mode        previous_mode;
  rtems_status_code status;

  status = rtems_task_mode(
    RTEMS_NO_PREEMPT,
    RTEMS_PREEMPT_MASK,
    &previous_mode
  );
  directive_failed( status, "rtems_task_mode to RTEMS_NO_PREEMPT" );

  Temporary_count = Count;
  Count.count[ 1 ] = 0;
  Count.count[ 2 ] = 0;
  Count.count[ 3 ] = 0;
  Count.count[ 4 ] = 0;
  Count.count[ 5 ] = 0;

  status = rtems_task_mode( RTEMS_PREEMPT, RTEMS_PREEMPT_MASK, &previous_mode );
  directive_failed( status, "rtems_task_mode to RTEMS_PREEMPT" );
}
