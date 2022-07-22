/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus/)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#if (!(defined(__APPLE__) || defined(__linux__) || defined(__unix__))) \
    || defined(__DOXYGEN__)

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_INCLUDE_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_INCLUDE_CONFIG_H

#if defined(MICRO_OS_PLUS_INCLUDE_SBRK)

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>

// ----------------------------------------------------------------------------

void*
_sbrk (ptrdiff_t incr);

// ----------------------------------------------------------------------------

// The definitions used here should be kept in sync with the
// stack definitions in the linker script.

extern uint32_t __heap_begin__; // Defined by the linker.
extern uint32_t __heap_end__; // Defined by the linker.

#if defined(MICRO_OS_PLUS_USE_SEMIHOSTING)
// Heap limit returned from SYS_HEAPINFO semihosting call.
char* __heap_limit = (char*)0xCAFEDEAD;
#endif // MICRO_OS_PLUS_USE_SEMIHOSTING

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
#if defined(MICRO_OS_PLUS_USE_SEMIHOSTING)
      // Honour heap limit if it's valid.
      || (__heap_limit != (char*)0xCAFEDEAD
          && current_heap_end + incr > __heap_limit)
#endif // MICRO_OS_PLUS_USE_SEMIHOSTING
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

#endif // defined(MICRO_OS_PLUS_INCLUDE_SBRK)

// ----------------------------------------------------------------------------

#endif // !Unix

// ----------------------------------------------------------------------------
