/*  mpci.h
 *
 *  This include file contains all the renaming necessary to
 *  have an application use the Shared Memory Driver as its
 *  sole mechanism for MPCI.
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

#ifndef __SHM_MPCI_h
#define __SHM_MPCI_h

#ifdef __cplusplus
extern "C" {
#endif

#include <shm.h>

#define MPCI_Initialization( _configuration ) \
  Shm_Initialization( _configuration )

#define MPCI_Get_packet( _the_packet ) \
  Shm_Get_packet( _the_packet )

#define MPCI_Return_packet( _the_packet ) \
  Shm_Return_packet( _the_packet )

#define MPCI_Receive_packet( _the_packet ) \
  Shm_Receive_packet( _the_packet )

#define MPCI_Send_packet( _destination, _the_packet ) \
  Shm_Send_packet( _destination, _the_packet )

/* Unnecessary... mapped in shm.h
#define MPCI_Fatal( _the_error ) \
  Shm_Fatal( _the_error )
*/

#define MPCI_Enable_statistics()

#define MPCI_Print_statistics() \
  Shm_Print_statistics()

/* no need to rename the MPCI_Table either */

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
