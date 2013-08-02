/*
 *  Thread Handler
 *
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

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/apiext.h>
#include <rtems/score/context.h>
#include <rtems/score/interr.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/score/priority.h>
#include <rtems/score/states.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>

/*PAGE
 *
 *  _Thread_Load_environment
 *
 *  Load starting environment for another thread from its start area in the
 *  thread.  Only called from t_restart and t_start.
 *
 *  Input parameters:
 *    the_thread - thread control block pointer
 *
 *  Output parameters:  NONE
 */

void _Thread_Load_environment(
  Thread_Control *the_thread
)
{
  bool is_fp;

#if ( CPU_HARDWARE_FP == TRUE ) || ( CPU_SOFTWARE_FP == TRUE )
  if ( the_thread->Start.fp_context ) {
    the_thread->fp_context = the_thread->Start.fp_context;
    _Context_Initialize_fp( &the_thread->fp_context );
    is_fp = true;
  } else
#endif
    is_fp = false;


  the_thread->do_post_task_switch_extension = false;
  the_thread->is_preemptible   = the_thread->Start.is_preemptible;
  the_thread->budget_algorithm = the_thread->Start.budget_algorithm;
  the_thread->budget_callout   = the_thread->Start.budget_callout;

  #ifdef _RTEMS_SCORE_PATMOS_H
	
  /* initialize the thread CPU context with:
   *   the thread CPU registers
   *   the thread initial stack area
   *   the thread initial stack size
   *   the thread isr level
   *   the thread entry point (is the common entry point to all threads)
   *   the thread initial shadow stack area */
  _Context_Initialize(
	&the_thread->Registers ,
	the_thread->Start.Initial_stack.area ,
	the_thread->Start.Initial_stack.size ,
	the_thread->Start.isr_level ,
	_Thread_Handler ,
	the_thread->shadow_stack_base);
						
  #else
  
  _Context_Initialize(
    &the_thread->Registers,
    the_thread->Start.Initial_stack.area,
    the_thread->Start.Initial_stack.size,
    the_thread->Start.isr_level,
    _Thread_Handler,
    is_fp
  );
  
  #endif /* _RTEMS_SCORE_PATMOS_H */

}
