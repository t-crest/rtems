/**
 * @file rtems/score/cpu.h
 */

/*
 *  This include file contains information pertaining to the port of
 *  the executive to the PATMOS processor.
 *
 *  Project: T-CREST - Time-Predictable Multi-Core Architecture for Embedded Systems
 *
 *  Copyright (C) GMVIS Skysoft S.A., 2013
 *  @author Andre Rocha
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 */

#ifndef _RTEMS_SCORE_CPU_H
#define _RTEMS_SCORE_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/score/patmos.h>               /* pick up machine definitions */
#ifndef ASM
#include <rtems/score/types.h>
#endif

/* conditional compilation parameters */

/*
 *  Should the calls to _Thread_Enable_dispatch be inlined?
 *
 *  If TRUE, then they are inlined.
 *  If FALSE, then a subroutine call is made.
 */

#define CPU_INLINE_ENABLE_DISPATCH       TRUE

/*
 *  Should the body of the search loops in _Thread_queue_Enqueue_priority
 *  be unrolled one time?  In unrolled each iteration of the loop examines
 *  two "nodes" on the chain being searched.  Otherwise, only one node
 *  is examined per iteration.
 *
 *  If TRUE, then the loops are unrolled.
 *  If FALSE, then the loops are not unrolled.
 *
 */

#define CPU_UNROLL_ENQUEUE_PRIORITY      TRUE

/*
 *  Does the executive manage a dedicated interrupt stack in software?
 *
 *  If TRUE, then a stack is allocated in _ISR_Handler_initialization.
 *  If FALSE, nothing is done.
 * 
 */

#define CPU_HAS_SOFTWARE_INTERRUPT_STACK   TRUE

/*
 *  Does the CPU follow the simple vectored interrupt model?
 *
 *  If TRUE, then RTEMS allocates the vector table it internally manages.
 *  If FALSE, then the BSP is assumed to allocate and manage the vector
 *  table
 *
 */
#define CPU_SIMPLE_VECTORED_INTERRUPTS TRUE

/*
 *  Does this CPU have hardware support for a dedicated interrupt stack?
 *
 *  If TRUE, then it must be installed during initialization.
 *  If FALSE, then no installation is performed.
 *
 *  Patmos does not have a dedicated HW interrupt stack.
 */

#define CPU_HAS_HARDWARE_INTERRUPT_STACK  FALSE

/*
 *  Do we allocate a dedicated interrupt stack in the Interrupt Manager?
 *
 *  If TRUE, then the memory is allocated during initialization.
 *  If FALSE, then the memory is allocated during initialization.
 */

#define CPU_ALLOCATE_INTERRUPT_STACK      TRUE

/*
 *  Does the RTEMS invoke the user's ISR with the vector number and
 *  a pointer to the saved interrupt frame (1) or just the vector
 *  number (0)?
 */

#define CPU_ISR_PASSES_FRAME_POINTER 0

/*
 *  Does the CPU have hardware floating point?
 *
 *  If TRUE, then the FLOATING_POINT task attribute is supported.
 *  If FALSE, then the FLOATING_POINT task attribute is ignored.
 */

#if ( PATMOS_HAS_FPU == 1 )
#define CPU_HARDWARE_FP     TRUE
#else
#define CPU_HARDWARE_FP     FALSE
#endif
#define CPU_SOFTWARE_FP     FALSE

/*
 *  Are all tasks FLOATING_POINT tasks implicitly?
 *
 *  If TRUE, then the FLOATING_POINT task attribute is assumed.
 *  If FALSE, then the FLOATING_POINT task attribute is followed.
 */

#define CPU_ALL_TASKS_ARE_FP     FALSE

/*
 *  Should the IDLE task have a floating point context?
 *
 *  If TRUE, then the IDLE task is created as a FLOATING_POINT task
 *  and it has a floating point context which is switched in and out.
 *  If FALSE, then the IDLE task does not have a floating point context.
 */

#define CPU_IDLE_TASK_IS_FP      FALSE

/*
 *  Should the saving of the floating point registers be deferred
 *  until a context switch is made to another different floating point
 *  task?
 *
 *  If TRUE, then the floating point context will not be stored until
 *  necessary.  It will remain in the floating point registers and not
 *  disturned until another floating point task is switched to.
 *
 *  If FALSE, then the floating point context is saved when a floating
 *  point task is switched out and restored when the next floating point
 *  task is restored.  The state of the floating point registers between
 *  those two operations is not specified.
 */

#define CPU_USE_DEFERRED_FP_SWITCH       TRUE

/*
 *  Does this port provide a CPU dependent IDLE task implementation?
 *
 *  If TRUE, then the routine _CPU_Thread_Idle_body
 *  must be provided and is the default IDLE thread body instead of
 *  _CPU_Thread_Idle_body.
 *
 *  If FALSE, then use the generic IDLE thread body if the BSP does
 *  not provide one.
 */

#define CPU_PROVIDES_IDLE_THREAD_BODY    FALSE

/*
 *  Does the stack grow up (toward higher addresses) or down
 *  (toward lower addresses)?
 *
 *  If TRUE, then the grows upward.
 *  If FALSE, then the grows toward smaller addresses.
 *
 *  The stack grows to lower addresses on Patmos.
 */

#define CPU_STACK_GROWS_UP               FALSE

/*
 *  The following is the variable attribute used to force alignment
 *  of critical data structures.  On some processors it may make
 *  sense to have these aligned on tighter boundaries than
 *  the minimum requirements of the compiler in order to have as
 *  much of the critical data area as possible in a cache line. 
 */

#define CPU_STRUCTURE_ALIGNMENT          __attribute__ ((aligned (32)))

/*
 *  Define what is required to specify how the network to host conversion
 *  routines are handled.
 */

#define CPU_BIG_ENDIAN                           TRUE
#define CPU_LITTLE_ENDIAN                        FALSE

/*
 *  The following defines the number of bits actually used in the
 *  interrupt field of the task mode.  How those bits map to the
 *  CPU interrupt levels is defined by the routine _CPU_ISR_Set_level().
 *
 */

#define CPU_MODES_INTERRUPT_MASK   0x00000001

/*
 * Contexts
 *
 *  Generally there is 1 type of context to save.
 *     1. Interrupt registers to save 
 *
 *  This means we have the following 1 context item:
 *     1. task level context stuff::  Context_Control  
 */

#ifndef ASM

typedef struct {	  
	  uint32_t r1;
	  uint32_t r2;
	  uint32_t r3;
	  uint32_t r4;
	  uint32_t r5;
	  uint32_t r6;
	  uint32_t r7;
      
      uint32_t r8;
	  uint32_t r9;
	  uint32_t r10;
	  uint32_t r11;
	  uint32_t r12;
	  uint32_t r13;
	  uint32_t r14;
	  uint32_t r15;
	  
	  uint32_t r16;
	  uint32_t r17;
	  uint32_t r18;
	  uint32_t r19;
	  uint32_t r20;
	  uint32_t r21;
	  uint32_t r22;
	  uint32_t r23;
	  
	  uint32_t r24;
	  uint32_t r25;
	  uint32_t r26;
	  uint32_t r27;
	  uint32_t r28;
	  uint32_t r29;
	  uint32_t r30;
	  uint32_t r31;

      uint32_t s0;
	  uint32_t s1;
	  uint32_t s2;
	  uint32_t s3;
	  uint32_t s4;
	  uint32_t s5;
	  uint32_t s6;
	  uint32_t s7;
      
      uint32_t s8;
	  uint32_t s9;
	  uint32_t s10;
	  uint32_t s11;
	  uint32_t s12;
	  uint32_t s13;
	  uint32_t s14;
	  uint32_t s15;

	  uint32_t ssize;
	  uint32_t exc_status;

   } Context_Control;

#endif /* ASM */

/*
 *  Offsets of fields with Context_Control for assembly routines.
 */

#define r1_OFFSET    0
#define r2_OFFSET    1
#define r3_OFFSET    2
#define r4_OFFSET    3
#define r5_OFFSET    4
#define r6_OFFSET    5
#define r7_OFFSET    6

#define r8_OFFSET    7
#define r9_OFFSET    8
#define r10_OFFSET   9
#define r11_OFFSET   10
#define r12_OFFSET   11
#define r13_OFFSET   12
#define r14_OFFSET   13
#define r15_OFFSET   14

#define r16_OFFSET   15
#define r17_OFFSET   16
#define r18_OFFSET   17
#define r19_OFFSET   18
#define r20_OFFSET   19
#define r21_OFFSET   20
#define r22_OFFSET   21
#define r23_OFFSET   22

#define r24_OFFSET   23
#define r25_OFFSET   24
#define r26_OFFSET   25
#define r27_OFFSET   26
#define r28_OFFSET   27
#define r29_OFFSET   28
#define r30_OFFSET   29
#define r31_OFFSET   30

#define s0_OFFSET    31
#define s1_OFFSET    32
#define s2_OFFSET    33
#define s3_OFFSET    34
#define s4_OFFSET    35
#define s5_OFFSET    36
#define s6_OFFSET    37
#define s7_OFFSET    38

#define s8_OFFSET    39
#define s9_OFFSET    40
#define s10_OFFSET   41
#define s11_OFFSET   42
#define s12_OFFSET   43
#define s13_OFFSET   44
#define s14_OFFSET   45
#define s15_OFFSET   46

#define ssize_OFFSET 47
#define exc_OFFSET   48

/*
 *  context control size (in number of bytes)
 */

#define CONTEXT_CONTROL_SIZE 196

/**
*
*  This macro returns the stack pointer associated with _context.
*
*  In: _context is the thread context area to access
*
*  Out: This method returns the stack pointer.
*/
#define _CPU_Context_Get_SP( _context ) \
 (_context)->s6

#define MAX_STACK_CACHE_SIZE 0x3FFFF

#ifndef ASM

/*
 *  Context saved on stack for an interrupt.
 *
 */

typedef struct {
  Context_Control  		   Stack_frame;
} CPU_Interrupt_frame;

#endif /* ASM */

/*
 *  Offsets of fields with CPU_Interrupt_frame for assembly routines.
 */

#define ISR_r1_OFFSET	0
#define ISR_r2_OFFSET   1
#define ISR_r3_OFFSET   2
#define ISR_r4_OFFSET   3
#define ISR_r5_OFFSET   4
#define ISR_r6_OFFSET   5
#define ISR_r7_OFFSET   6
#define ISR_r8_OFFSET   7

#define ISR_r9_OFFSET   8
#define ISR_r10_OFFSET   9
#define ISR_r11_OFFSET   10
#define ISR_r12_OFFSET   11
#define ISR_r13_OFFSET   12
#define ISR_r14_OFFSET   13
#define ISR_r15_OFFSET   14
#define ISR_r16_OFFSET   15

#define ISR_r17_OFFSET   16
#define ISR_r18_OFFSET   17
#define ISR_r19_OFFSET   18
#define ISR_r20_OFFSET   19
#define ISR_r21_OFFSET   20
#define ISR_r22_OFFSET   21
#define ISR_r23_OFFSET   22
#define ISR_r24_OFFSET   23

#define ISR_r25_OFFSET   24
#define ISR_r26_OFFSET   25
#define ISR_r27_OFFSET   26
#define ISR_r28_OFFSET   27
#define ISR_r29_OFFSET   28
#define ISR_r30_OFFSET   29
#define ISR_s0_OFFSET    30
#define ISR_s1_OFFSET    31

#define ISR_s2_OFFSET    32
#define ISR_s3_OFFSET    33
#define ISR_s4_OFFSET    34
#define ISR_s6_OFFSET    35
#define ISR_s7_OFFSET    36
#define ISR_s8_OFFSET    37
#define ISR_s9_OFFSET    38
#define ISR_s10_OFFSET   39

#define ISR_s11_OFFSET   40
#define ISR_s12_OFFSET   41
#define ISR_s13_OFFSET   42
#define ISR_s14_OFFSET   43
#define ISR_s15_OFFSET   44
#define ISR_ssize_OFFSET 45
#define ISR_exc_OFFSET   46

#define ISR_CONTEXT_CONTROL_SIZE 188

#ifndef ASM
/*
 *  This stack is allocated by the Interrupt Manager and the switch
 *  is performed in _ISR_Handler.  These variables contain pointers
 *  to the lowest and highest addresses in the chunk of memory allocated
 *  for the interrupt stack.  Since it is unknown whether the stack
 *  grows up or down (in general), this give the CPU dependent
 *  code the option of picking the version it wants to use.  Thus
 *  both must be present if either is.
 *
 */

SCORE_EXTERN void *_CPU_Interrupt_stack_low; /* This variable points to the lowest physical address of the interrupt stack */
SCORE_EXTERN void *_CPU_Interrupt_stack_high; /* This variable points to the highest physical address of the interrupt stack */

/*
 *  The size of the floating point context area.
 */

#define CPU_CONTEXT_FP_SIZE 0

#endif /* ASM */

/*
 *  Amount of extra stack (above minimum stack size) required by
 *  MPCI receive server thread.  Remember that in a multiprocessor
 *  system this thread must exist and be able to process all directives.
 */

#define CPU_MPCI_RECEIVE_SERVER_EXTRA_STACK 1024

/*
 *  This defines the number of entries in the ISR_Vector_table managed
 *  by the executive.
 */

#define CPU_INTERRUPT_NUMBER_OF_VECTORS     0
#define CPU_INTERRUPT_MAXIMUM_VECTOR_NUMBER 0 /* This defines the highest interrupt vector number for this port. */

/*
 *  This is defined if the port has a special way to report the ISR nesting
 *  level.  Most ports maintain the variable _ISR_Nest_level.
 */

#define CPU_PROVIDES_ISR_IS_IN_PROGRESS FALSE

/*
 *  Should be large enough to run all tests.  This ensures
 *  that a "reasonable" small application should not have any problems.
 */

#define CPU_STACK_MINIMUM_SIZE  (1024*4)

/*
 *  CPU's worst alignment requirement for data types on a byte boundary.  This
 *  alignment does not take into account the requirements for the stack.
 */

#define CPU_ALIGNMENT      8

/*
 *  This number corresponds to the byte alignment requirement for the
 *  heap handler.  This alignment requirement may be stricter than that
 *  for the data types alignment specified by CPU_ALIGNMENT.  It is
 *  common for the heap to follow the same alignment requirement as
 *  CPU_ALIGNMENT.  If the CPU_ALIGNMENT is strict enough for the heap,
 *  then this should be set to CPU_ALIGNMENT.
 *
 *  NOTE:  This does not have to be a power of 2.  It does have to
 *         be greater or equal to than CPU_ALIGNMENT.
 */

#define CPU_HEAP_ALIGNMENT         CPU_ALIGNMENT

/*
 *  This number corresponds to the byte alignment requirement for memory
 *  buffers allocated by the partition manager.  This alignment requirement
 *  may be stricter than that for the data types alignment specified by
 *  CPU_ALIGNMENT.  It is common for the partition to follow the same
 *  alignment requirement as CPU_ALIGNMENT.  If the CPU_ALIGNMENT is strict
 *  enough for the partition, then this should be set to CPU_ALIGNMENT.
 *
 *  NOTE:  This does not have to be a power of 2.  It does have to
 *         be greater or equal to than CPU_ALIGNMENT.
 */

#define CPU_PARTITION_ALIGNMENT    CPU_ALIGNMENT

/*
 *  This number corresponds to the byte alignment requirement for the
 *  stack.  This alignment requirement may be stricter than that for the
 *  data types alignment specified by CPU_ALIGNMENT.  If the CPU_ALIGNMENT
 *  is strict enough for the stack, then this should be set to 0.
 *
 *  NOTE:  This must be a power of 2 either 0 or greater than CPU_ALIGNMENT.
 */

#define CPU_STACK_ALIGNMENT        16

#ifndef ASM

/*
 *  ISR handler macros
 */

/*
 *  Support routine to initialize the RTEMS vector table after it is allocated.
 */

#define _CPU_Initialize_vectors()

/*
 *  Disable all interrupts for a critical section.  The previous
 *  level is returned in _level.
 */

#define _CPU_ISR_Disable( _level ) \
  (_level) = patmos_disable_interrupts()

/*
 *  Enable interrupts to the previous level (returned by _CPU_ISR_Disable).
 *  This indicates the end of a critical section.  The parameter
 *  _level is not modified.
 */

#define _CPU_ISR_Enable( _level ) \
  patmos_enable_interrupts( _level )

/*
 *  This temporarily restores the interrupt to _level before immediately
 *  disabling them again.  This is used to divide long critical
 *  sections into two or more parts.  The parameter _level is not
 *  modified.
 */

#define _CPU_ISR_Flash( _level ) \
  patmos_flash_interrupts( _level )

/*
 *  Map interrupt level in task mode onto the hardware that the CPU
 *  actually provides.  Currently, interrupt levels which do not
 *  map onto the CPU in a straight fashion are undefined. Someday,
 *  it would be nice if these were "mapped" by the application
 *  via a callout.  For example, m68k has 8 levels 0 - 7, levels
 *  8 - 255 would be available for bsp/application specific meaning.
 *  This could be used to manage a programmable interrupt controller
 *  via the rtems_task_mode directive.
 */

#define _CPU_ISR_Set_level( _newlevel ) \
   patmos_enable_interrupts( _newlevel ^ CPU_MODES_INTERRUPT_MASK)

/*
 *  Return the current interrupt disable level for this task in
 *  the format used by the interrupt level portion of the task mode.
 */

uint32_t   _CPU_ISR_Get_level( void );

/* end of ISR handler macros */

/* Context handler macros */

/*
 *  Initialize the context to a state suitable for starting a
 *  task after a context restore operation.  Generally, this
 *  involves:
 *
 *     - setting a starting address
 *     - preparing the stack and shadow stack
 *     - preparing the stack and frame pointers for both the stack and the shadow stack
 *     - setting the proper interrupt level in the context 
 *
 *  NOTE:  Implemented as a subroutine for Patmos port.
 */

void _CPU_Context_Initialize(
Context_Control *the_context ,
uint32_t *stack_base ,
uint32_t size ,
uint32_t new_level ,
void *entry_point ,
uint32_t *shadow_stack_base
);

/*
 *  This routine is responsible for somehow restarting the currently
 *  executing task.
 */

#define _CPU_Context_Restart_self( _the_context ) \
   _CPU_Context_restore( (_the_context) );

/* end of Context handler macros */

/* Fatal Error manager macros */

/*
 *  This routine copies _error into a known place -- typically a stack
 *  location or a register, optionally disables interrupts, and
 *  halts/stops the CPU.
 */

#define _CPU_Fatal_halt( _error ) \
  do { \
	  asm volatile ("call __exit \n\t nop \n\t nop \n\t nop \n\t" : :);	\
  } while (1)

/* end of Fatal Error manager macros */

/* Bitfield handler macros */

/*
 *  The Patmos port uses the generic C algorithm for bitfield scan if the
 *  CPU model does not have a scan instruction.
 */

#if ( PATMOS_HAS_BITSCAN == 0 )
#define CPU_USE_GENERIC_BITFIELD_CODE TRUE
#define CPU_USE_GENERIC_BITFIELD_DATA TRUE
#else
#error "scan instruction not currently supported by RTEMS!!"
#endif

/* end of Bitfield handler macros */

/* Priority handler handler macros */

/*
 *  The Patmos port uses the generic C algorithm for bitfield scan if the
 *  CPU model does not have a scan instruction.
 */

#if ( PATMOS_HAS_BITSCAN == 1 )
#error "scan instruction not currently supported by RTEMS!!"
#endif

/* end of Priority handler macros */

/* functions */

/*
 *  _CPU_Initialize
 *
 *  This routine performs CPU dependent initialization.
 */

void _CPU_Initialize(void);

/*
 *  _CPU_ISR_install_raw_handler
 *
 *  This routine installs new_handler to be directly called from the trap
 *  table.
 */

void _CPU_ISR_install_raw_handler(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
);

/*
 *  _CPU_ISR_install_vector
 *
 *  This routine installs an interrupt vector.
 */

void _CPU_ISR_install_vector(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
);

#if (CPU_PROVIDES_IDLE_THREAD_BODY == TRUE)

/*
 *  _CPU_Thread_Idle_body
 *
 *  This routine is the CPU dependent IDLE thread body. 
 */

void *_CPU_Thread_Idle_body( uintptr_t ignored );

#endif /* CPU_PROVIDES_IDLE_THREAD_BODY */

/*
 *  _CPU_Context_switch
 *
 *  This routine switches from the run context to the heir context.
 */

void _CPU_Context_switch(
  Context_Control  *run,
  Context_Control  *heir
) __attribute__((naked,noinline));

/*
 *  _CPU_Context_restore
 *
 *  This routine is generally used only to restart self in an
 *  efficient manner.
 */

void _CPU_Context_restore(
  Context_Control *new_context
) __attribute__((naked,noinline));

/*
 *  CPU_swap_u32
 *
 *  The following routine swaps the endian format of an unsigned int.
 *  It must be static because it is referenced indirectly.
 *
 *  This version will work on any processor, but if you come across a better
 *  way for the Patmos PLEASE use it.  The most common way to swap a 32-bit
 *  entity as shown below is not any more efficient on Patmos.
 *
 *     swap least significant two bytes with 16-bit rotate
 *     swap upper and lower 16-bits
 *     swap most significant two bytes with 16-bit rotate
 *
 */

static inline uint32_t CPU_swap_u32(
  uint32_t value
)
{
  uint32_t   byte1, byte2, byte3, byte4, swapped;

  byte4 = (value >> 24) & 0xff;
  byte3 = (value >> 16) & 0xff;
  byte2 = (value >> 8)  & 0xff;
  byte1 =  value        & 0xff;

  swapped = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
  return( swapped );
}

/*
 *  CPU_swap_u16
 *
 *  This routine swaps a 16 bit quantity.
 */

#define CPU_swap_u16( value ) \
  (((value&0xff) << 8) | ((value >> 8)&0xff))

#endif /* ASM */

#ifdef __cplusplus
}
#endif

#endif
