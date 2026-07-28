/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2015-2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

#if (!(defined(__APPLE__) || defined(__linux__) || defined(__unix__))) \
    || defined(__DOXYGEN__)

// ----------------------------------------------------------------------------

#include <micro-os-plus/startup.h>

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_STARTUP_ENABLED) \
    && defined(MICRO_OS_PLUS_STARTUP_SBRK_ENABLED)

// ----------------------------------------------------------------------------

void*
_sbrk (ptrdiff_t incr);

// ----------------------------------------------------------------------------

// The definitions used here should be kept in sync with the
// stack definitions in the linker script.

extern uint32_t __heap_begin__; // Defined by the linker script.
extern uint32_t __heap_end__; // Defined by the linker script.

#if defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)
// Heap limit returned from SYS_HEAPINFO semihosting call.
char* __heap_limit = (char*)0xCAFEDEAD;
#endif // defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)

void*
_sbrk (ptrdiff_t incr)
{
  static char* current_heap_end; // STATIC! Zero after BSS init.
  char* current_block_address;

  if (current_heap_end == 0)
    {
      current_heap_end = (char*)&__heap_begin__;
    }

  current_block_address = current_heap_end;

  // Need to align heap to word boundary, for efficiency reasons and
  // to possibly avoid hardware faults.
  // So we assume that the heap starts on word boundary,
  // hence make sure we always add a multiple of 4 to it.
  incr = (incr + 3) & (~3); // align value to 4
  if ((current_heap_end + incr > (char*)&__heap_end__)
#if defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)
      // Honour heap limit if it's valid.
      || (__heap_limit != (char*)0xCAFEDEAD
          && current_heap_end + incr > __heap_limit)
#endif // defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)
  )
    {
      // Some of the libstdc++-v3 tests rely upon detecting 'out of memory'
      // errors, so DO NOT abort here, but return error.

      errno = ENOMEM; // Heap has overflowed.
      return (caddr_t)-1;
    }

  current_heap_end += incr;

  return (caddr_t)current_block_address;
}

#endif // defined(MICRO_OS_PLUS_STARTUP_ENABLED) &&
       // defined(MICRO_OS_PLUS_STARTUP_SBRK_ENABLED)

// ----------------------------------------------------------------------------

#endif // !Unix

// ----------------------------------------------------------------------------
