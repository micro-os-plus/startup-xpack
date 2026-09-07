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
  void
  micro_os_plus_startup_run_main (void);

#if defined(MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALISE_HARDWARE_EARLY)

  /**
   * @brief Initialise hardware early hook.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_initialise_hardware_early_hook (void);

#endif // defined(MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALISE_HARDWARE_EARLY)

#if defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED)

  /**
   * @brief Initialise hardware hook.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_initialise_hardware_hook (void);

#endif // defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED)

  /**
   * @brief Initialise the free store hook.
   * @param heap_address The first unallocated RAM address (after the BSS).
   * @param heap_size_bytes The free store size.
   * @par Returns
   *  Nothing.
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
   * @brief Display statistics and say goodbye before terminating.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_terminate_goodbye (void);

  /**
   * @brief Terminate the application. There is no more life after this.
   * @param [in] code Exit code, 0 for success, non 0 for failure.
   * @par Returns
   *  Nothing.
   */
  void __attribute__ ((noreturn))
  micro_os_plus_terminate (int code);

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
