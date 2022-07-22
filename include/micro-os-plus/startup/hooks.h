/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus/)
 * Copyright (c) 2017 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#ifndef MICRO_OS_PLUS_STARTUP_HOOKS_H_
#define MICRO_OS_PLUS_STARTUP_HOOKS_H_

// ----------------------------------------------------------------------------

#include <stddef.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  /**
   * @addtogroup micro-os-plus-app-hooks
   * @{
   */

  /**
   * @name Startup Routines
   * @{
   */

#if defined(MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE_EARLY) \
    || defined(__DOXYGEN__)

  /**
   * @brief Initialise hardware early.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_initialize_hardware_early (void);

#endif // MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE_EARLY

#if defined(MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE) \
    || defined(__DOXYGEN__)

  /**
   * @brief Initialise hardware.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_initialize_hardware (void);

#endif // MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE

  /**
   * @brief Initialise the free store.
   * @param heap_address The first unallocated RAM address (after the BSS).
   * @param heap_size_bytes The free store size.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_initialize_free_store (void* heap_address,
                                               size_t heap_size_bytes);

  /**
   * @brief Initialise the interrupts stack.
   * @param stack_begin_address The stack bottom address.
   * @param stack_size_bytes The stack size.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_initialize_interrupts_stack (void* stack_begin_address,
                                                     size_t stack_size_bytes);

  /**
   * @brief Initialise arguments.
   * @param [out] p_argc Pointer to argc.
   * @param [out] p_argv Pointer to argv.
   */
  void
  micro_os_plus_startup_initialize_args (int* p_argc, char*** p_argv);

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
  void __attribute__ ((noreturn)) micro_os_plus_terminate (int code);

  /**
   * @}
   */

  /**
   * @}
   */

#if defined(__cplusplus)
}
#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_STARTUP_HOOKS_H_

// ----------------------------------------------------------------------------
