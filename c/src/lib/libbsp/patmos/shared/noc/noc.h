/*
   Copyright 2014 Technical University of Denmark, DTU Compute. 
   All rights reserved.
   
   This file is part of the time-predictable VLIW processor Patmos.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

      1. Redistributions of source code must retain the above copyright notice,
         this list of conditions and the following disclaimer.

      2. Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER ``AS IS'' AND ANY EXPRESS
   OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
   NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   The views and conclusions contained in the software and documentation are
   those of the authors and should not be interpreted as representing official
   policies, either expressed or implied, of the copyright holder.
 */

/**
 * \file noc.h Definitions for libnoc.
 * 
 * \author Wolfgang Puffitsch <wpuffitsch@gmail.com>
 *
 */

/*
 *  This file has been modified by GMVIS Skysoft S.A.
 */

#ifndef _NOC_H_
#define _NOC_H_

#include <bsp.h>

////////////////////////////////////////////////////////////////////////////
// Definitions used for initialization of network interface
////////////////////////////////////////////////////////////////////////////

/// The CPU ID
#define CORE_ID *((_iodev_ptr_t) __PATMOS_CPU_ID_ADDR)

/// \brief The number of cores on the platform.
///
/// Defined by the application
extern const int NOC_CORES;

/// \brief The number of tables for Noc configuration.
///
/// Defined by the application
extern const int NOC_TABLES;

/// \brief The number of timeslots in the schedule.
///
/// Defined by the application
extern const int NOC_TIMESLOTS;

/// \brief The number of DMAs in the configuration.
///
/// Defined by the application
extern const int NOC_DMAS;

/// \brief The array for initialization data.
///
/// Defined by the application
extern const int noc_init_array [];

/// \brief The master core, which governs booting and startup synchronization.
///
/// Defined by the application
extern const int NOC_MASTER;

/// \brief Configure network interface according to initialization
/// information in #noc_init_array.
void noc_configure(void);

/// \brief Configure network-on-chip and synchronize all cores.
///
/// #noc_init is a static constructor and not intended to be called directly.
void noc_init(void) __attribute__((constructor,used));

////////////////////////////////////////////////////////////////////////////
// Functions for transmitting data
////////////////////////////////////////////////////////////////////////////

/// \brief Start a NoC transfer.
///
/// The addresses and the size are in double-words and relative to the
/// communication SPM base #NOC_SPM_BASE.
/// \param rcv_id The core id of the receiver.
/// \param write_ptr The address in the receiver's communication SPM,
/// in double-words, relative to #NOC_SPM_BASE.
/// \param read_ptr The address in the sender's communication SPM, in
/// double-words, relative to #NOC_SPM_BASE.
/// \param size The size of data to be transferred, in double-words.
/// \returns 1 if sending was successful, 0 otherwise.
int noc_dma(unsigned rcv_id, unsigned short write_ptr,
            unsigned short read_ptr, unsigned short size);

/// \brief Transfer data via the NoC.
///
/// The addresses and the size are absolute and in bytes.
/// \param rcv_id The core id of the receiver.
/// \param dst A pointer to the destination of the transfer.
/// \param src A pointer to the source of the transfer.
/// \param size The size of data to be transferred, in bytes.
void noc_send(int rcv_id, volatile void _SPM *dst,
              volatile void _SPM *src, size_t size);


////////////////////////////////////////////////////////////////////////////
// Definitions for setting up a transfer
////////////////////////////////////////////////////////////////////////////

/// The flag to mark a DMA entry as valid
#define NOC_VALID_BIT 0x08000
/// The flag to mark a DMA entry as done
#define NOC_DONE_BIT 0x04000

////////////////////////////////////////////////////////////////////////////
// Boot data definitions
////////////////////////////////////////////////////////////////////////////

#define MAX_CORES 64

#define STATUS_NULL     0
#define STATUS_BOOT     1
#define STATUS_INIT     2
#define STATUS_INITDONE 3

struct master_info_t {
  volatile entrypoint_t entrypoint;
  volatile int status;
};

struct slave_info_t {
  volatile int status;
};

struct boot_info_t {
  struct master_info_t master;
  struct slave_info_t slave[MAX_CORES];
};

/* Place boot info at the beginning of the memory. Nothing else may be
   placed there. */
#define boot_info ((_UNCACHED struct boot_info_t *)0x00000010)

#endif /* _NOC_H_ */
