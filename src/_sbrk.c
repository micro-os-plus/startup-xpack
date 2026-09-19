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

#include "micro-os-plus/startup.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdint.h>

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_STARTUP_ENABLED) \
    && defined(MICRO_OS_PLUS_STARTUP_SBRK_ENABLED)

// ----------------------------------------------------------------------------

void*
_sbrk (ptrdiff_t incr);

// ----------------------------------------------------------------------------

// The definitions used here should be kept in sync with the
// stack definitions in the linker script.
//
// Note: this grows a heap over [__heap_begin__, __heap_end__) to back
// newlib's malloc()/new. This is independent of, and not coordinated
// with, micro_os_plus_startup_initialise_free_store_hook() (see
// startup.h), which some applications override to install a custom
// allocator over the same region. Do not enable both over the same
// region unless that overlap is intended.

extern uint32_t __heap_begin__; // Defined by the linker script.
extern uint32_t __heap_end__; // Defined by the linker script.

#if defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)
// Heap limit returned from SYS_HEAPINFO semihosting call.
char* __heap_limit = (char*)0xCAFEDEAD;
#endif // defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)

/**
 * @brief Extend or shrink the heap (newlib `sbrk()` syscall stub).
 * @param [in] incr Number of bytes to add to the current heap break;
 *  may be negative to shrink it.
 * @return A pointer to the start of the newly allocated block, or
 *  `(caddr_t) -1` with `errno` set to `ENOMEM` if the request cannot
 *  be satisfied.
 *
 * @details
 * Rounds `incr` up to a multiple of `alignof(max_align_t)`, then
 * grows or shrinks a heap tracked in a function-local `static`
 * pointer, seeded from `__heap_begin__` on first call and bounded by
 * `__heap_end__` (and, when semihosting is enabled, by `__heap_limit`
 * if it has been set to a valid value). Backs newlib's `malloc()`/
 * `new`; see the note above for its relationship with
 * `micro_os_plus_startup_initialise_free_store_hook()`.
 */
[[gnu::weak]]
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

  // Need to align heap to the strictest fundamental alignment, for
  // efficiency reasons and to possibly avoid hardware faults.
  // So we assume that the heap starts properly aligned,
  // hence make sure we always add a multiple of that alignment to it.
#define STARTUP_SBRK_ALIGN_ ((ptrdiff_t)alignof (max_align_t))
  if (incr > PTRDIFF_MAX - (STARTUP_SBRK_ALIGN_ - 1))
    {
      // Reject an `incr` that would overflow `ptrdiff_t` once rounded
      // up for alignment, rather than relying on it never happening.
      errno = ENOMEM;
      return (caddr_t)-1;
    }
  incr = (incr + (STARTUP_SBRK_ALIGN_ - 1)) & ~(STARTUP_SBRK_ALIGN_ - 1);
#undef STARTUP_SBRK_ALIGN_

  if ((current_heap_end + incr > (char*)&__heap_end__)
      // Also reject a negative `incr` that would move the break below
      // the start of the heap.
      || (current_heap_end + incr < (char*)&__heap_begin__)
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

#endif /* defined(MICRO_OS_PLUS_STARTUP_ENABLED)
           && defined(MICRO_OS_PLUS_STARTUP_SBRK_ENABLED) */

// ----------------------------------------------------------------------------

#endif // !Unix

// ----------------------------------------------------------------------------
