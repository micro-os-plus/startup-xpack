/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2021-2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

#ifndef MICRO_OS_PLUS_STARTUP_H_
#define MICRO_OS_PLUS_STARTUP_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
#if !(__cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L))
#error "C++20 or higher is required"
#endif // !(__cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L))
#endif // defined(__cplusplus)

#if __has_include("micro-os-plus/project-config.h")
#include "micro-os-plus/project-config.h"
#endif // __has_include("micro-os-plus/project-config.h")

#if __has_include("micro-os-plus/startup-defines.h")
#include "micro-os-plus/startup-defines.h"
#endif // __has_include("micro-os-plus/startup-defines.h")

// ----------------------------------------------------------------------------

#include <stddef.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_STARTUP_ENABLED)

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

  /**
   * @addtogroup micro-os-plus-app-hooks
   * @{
   */

  /**
   * @name Startup Routines
   * @{
   */

  /**
   * @brief Initialise and run `main()`.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  [[noreturn]]
  void
  micro_os_plus_startup_run_main (void);

#if defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_EARLY_ENABLED)

  /**
   * @brief Initialise hardware early hook.
   * @par Parameters
   *  None.
   * @returns 0 for success, non 0 for failure.
   */
  int
  micro_os_plus_startup_initialise_hardware_early_hook (void);

#endif // defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_EARLY_ENABLED)

#if defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED)

  /**
   * @brief Initialise hardware hook.
   * @par Parameters
   *  None.
   * @returns 0 for success, non 0 for failure.
   */
  int
  micro_os_plus_startup_initialise_hardware_hook (void);

#endif // defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED)

#if defined(MICRO_OS_PLUS_STARTUP_FINALISE_HARDWARE_ENABLED)

  /**
   * @brief Finalise hardware hook.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_finalise_hardware_hook (void);

#endif // defined(MICRO_OS_PLUS_STARTUP_FINALISE_HARDWARE_ENABLED)

  /**
   * @brief Initialise the free store hook.
   * @param heap_address The first unallocated RAM address (after the BSS).
   * @param heap_size_bytes The free store size.
   * @par Returns
   *  Nothing.
   *
   * @details
   * The default (weak) implementation only traces the heap region; it
   * does not reserve or allocate anything.
   *
   * If overridden to install a custom allocator over
   * `[heap_address, heap_address + heap_size_bytes)`, note that this
   * region is the same one `_sbrk()` (`src/_sbrk.c`, in the separate
   * `sbrk` component) independently grows a heap over, via the
   * `__heap_begin__`/`__heap_end__` linker symbols, to back newlib's
   * `malloc()`/`new`. The two are not coordinated; do not enable both
   * over the same region unless that overlap is intended.
   */
  void
  micro_os_plus_startup_initialise_free_store_hook (void* heap_address,
                                                    size_t heap_size_bytes);

#if defined(MICRO_OS_PLUS_HAS_INTERRUPTS_STACK)

  /**
   * @brief Initialise the interrupts stack hook.
   * @param stack_begin_address The stack bottom address.
   * @param stack_size_bytes The stack size.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_initialise_interrupts_stack_hook (
      void* stack_begin_address, size_t stack_size_bytes);

#endif // defined(MICRO_OS_PLUS_HAS_INTERRUPTS_STACK)

  /**
   * @brief Initialise arguments hook.
   * @param [out] p_argc Pointer to argc.
   * @param [out] p_argv Pointer to argv.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_initialise_args_hook (int* p_argc, char*** p_argv);

  /**
   * @}
   */

  /**
   * @name Termination Routines
   * @{
   */

  /**
   * @brief Exit.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  [[noreturn]]
  void
  micro_os_plus_startup_exit (int code);

  /**
   * @brief Display statistics and say goodbye before terminating.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_exit_goodbye_hook (void);

  /**
   * @brief Terminate the application. There is no more life after this.
   * @param [in] code Exit code, 0 for success, non 0 for failure.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_exit_terminate_hook (int code);

  /**
   * @}
   */

  /**
   * @}
   */

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

// ----------------------------------------------------------------------------

#endif // defined(MICRO_OS_PLUS_STARTUP_ENABLED)

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_STARTUP_H_

// ----------------------------------------------------------------------------
