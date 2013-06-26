/**
 * @file rtems/score/types.h
 */

/*
 *  This include file contains type definitions pertaining to the
 *  Patmos processor family.
 *
 *  Project: T-CREST - Time-Predictable Multi-Core Architecture for Embedded Systems
 *
 *  Copyright (C) GMVIS Skysoft S.A., 2013
 *  @author André Rocha
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_TYPES_H
#define _RTEMS_SCORE_TYPES_H

#ifndef ASM

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  This section defines the basic types for this processor.
 */

typedef uint16_t         Priority_Bit_map_control;

#ifdef RTEMS_DEPRECATED_TYPES
typedef bool	boolean;              /* Boolean value   */
typedef float	single_precision;     /* single precision float */
typedef double	double_precision;     /* double precision float */
#endif

#ifdef __cplusplus
}
#endif

#endif  /* !ASM */

#endif
