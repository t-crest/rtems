/*
 *  Cache Management Support Routines for the PATMOS
 *
 *  $Id: cache.c,v 1.00 2013/06/14 15:17:12 André Rocha $
 */

#include <rtems.h>
#include "cache_.h"


#if defined(HAS_INSTRUCTION_CACHE)

void _CPU_cache_invalidate_entire_instruction ( void )
{
  asm volatile ("flush");
}

/* XXX these need to be addressed */
void _CPU_cache_freeze_instruction ( void )
{
}

void _CPU_cache_unfreeze_instruction ( void )
{
}

void _CPU_cache_enable_instruction ( void )
{
}

void _CPU_cache_disable_instruction (   void )
{
}

#endif
/* end of file */
