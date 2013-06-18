/*
 *  Idle Thread Body
 *
 *  This routine puts PASIM in power-down mode.
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
 * 
 */



#include <rtems/asm.h>

/* PASIM specific power-down function */

        .align 4
        PUBLIC(bsp_idle_thread)
SYM(bsp_idle_thread):
pwdloop:  mov   %g0, %asr19
          ba    pwdloop
          nop
          retl
          nop

