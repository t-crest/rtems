/*  Shm_Envelope_control *Shm_Locked_queue_Get( lq_cb )
 *
 *  This routine returns an envelope control block from a shared
 *  memory queue.
 *
 *  Input parameters:
 *    lq_cb - pointer to a locked queue control block
 *
 *  Output parameters:
 *    returns - pointer to an envelope control block
 *            - NULL if no envelopes on specified queue
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may in
 *  the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id$
 */

#include <rtems.h>
#include <shm.h>

Shm_Envelope_control *Shm_Locked_queue_Get(
  Shm_Locked_queue_Control *lq_cb
)
{
  Shm_Envelope_control *tmp_ecb;
  rtems_unsigned32 tmpfront;

  tmp_ecb = NULL;
  Shm_Lock( lq_cb );

    tmpfront = Shm_Convert(lq_cb->front);
    if ( tmpfront != Shm_Locked_queue_End_of_list ) {
      tmp_ecb = &Shm_Envelopes[ tmpfront ];
      lq_cb->front = tmp_ecb->next;
      if ( tmp_ecb->next == Shm_Locked_queue_End_of_list )
        lq_cb->rear = Shm_Locked_queue_End_of_list;
      tmp_ecb->next = Shm_Locked_queue_Not_on_list;
    }

  Shm_Unlock( lq_cb );
  return( tmp_ecb );
}
