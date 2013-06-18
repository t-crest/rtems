/**
 * @file rtems/score/patmos.h
 */

/*
 *  This include file contains information pertaining to the PATMOS
 *  processor family.
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

#ifndef _RTEMS_SCORE_PATMOS_H
#define _RTEMS_SCORE_PATMOS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  This file contains the information required to build
 *  RTEMS for a particular member of the "patmos" family.  It does
 *  this by setting variables to indicate which implementation
 *  dependent features are present in a particular member
 *  of the family.
 *
 *  Currently recognized feature flags:
 *
 *    + PATMOS_HAS_FPU
 *        0 - no HW FPU
 *        1 - has HW FPU (assumed to be compatible w/90C602)
 *
 *    + PATMOS_HAS_BITSCAN
 *        0 - does not have scan instructions
 *        1 - has scan instruction  (not currently implemented)
 *
 */

/*
 */

#define PATMOS_HAS_BITSCAN                0

/*
 * determine if PATMOS has hardware FPU
 */

#define PATMOS_HAS_FPU 0

#if PATMOS_HAS_FPU
#define CPU_MODEL_NAME "w/FPU"
#else
#define CPU_MODEL_NAME "w/soft-float"
#endif

/*
 *  Define the name of the CPU family.
 */

#define CPU_NAME "PATMOS"

/*
 *  Miscellaneous constants
 */

/*
 *  Standard nop
 */

#define nop() \
  do { \
    asm volatile ( "nop" ); \
  } while ( 0 )

#ifdef __cplusplus
}
#endif

#endif /* _RTEMS_SCORE_PATMOS_H */
