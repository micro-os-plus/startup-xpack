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

#if defined(MICRO_OS_PLUS_INCLUDE_EXIT)

#include <micro-os-plus/startup/hooks.h>
#include <micro-os-plus/architecture.h>
#include <micro-os-plus/diag/trace.h>

#include <stdlib.h>
#include <stdbool.h>

extern void
micro_os_plus_run_fini_array (void);

extern void
__call_exitprocs (int, void*);

// ----------------------------------------------------------------------------

void __attribute__ ((weak, noreturn)) abort (void)
{
  trace_puts ("abort(), exiting...");

  _Exit (1);
  /* NOTREACHED */
}

// ----------------------------------------------------------------------------

/**
 * @details
 * `exit()` does several cleanups before ending the application:
 *
 * - calls all application-defined cleanup functions enrolled with `atexit()`;
 * - files and streams are
 * cleaned up: any pending output is delivered to the host system, each
 * open file or stream is closed, and files created by `tmpfile()` are
 * deleted (wishful thinking, not implemented);
 * - call the static destructors (in reverse order of constructors)
 *
 * When all cleanups are done, `_Exit()` is called to perform
 * the actual termination.
 */
void __attribute__ ((noreturn)) exit (int code)
{
  trace_printf ("\n%s(%d)\n", __func__, code);

  // Call the cleanup functions enrolled with atexit().
  __call_exitprocs (code, NULL);

  // Run the C++ static destructors.
  micro_os_plus_run_fini_array ();

  // This should normally be the end of it.
  _Exit (code);

  // Reset again, in case _Exit() did not kill it.
  // This normally should not happen, but since it can be
  // overloaded by the application, better safe than sorry.
  micro_os_plus_terminate (code);

#if defined(MICRO_OS_PLUS_DEBUG)
  micro_os_plus_architecture_brk ();
#endif // defined(MICRO_OS_PLUS_DEBUG)
  // If it does not want to die, loop.
  while (true)
    {
      micro_os_plus_architecture_wfi ();
    }
  /* NOTREACHED */
}

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-parameter"
// arm-none-eabi/include/stdlib.h:202:6: note: '_exit' target declared here
//  202 | void _Exit (int __status) _ATTRIBUTE ((__noreturn__));
#pragma GCC diagnostic ignored "-Wmissing-attributes"

// On Release, call the hardware reset procedure.
// On Debug, use a breakpoint to notify the debugger.
//
// It can be redefined by the application, if more functionality
// is required. For example, when semihosting is used, this
// function sends the return code to the host.

void __attribute__ ((weak, noreturn)) _Exit (int code)
{
  trace_printf ("%s(%d)\n", __func__, code);

  // Print some statistics about memory use.
  micro_os_plus_terminate_goodbye ();

  // Gracefully terminate the trace session.
  trace_flush ();

  // Reset hardware or terminate the semihosting session.
  micro_os_plus_terminate (code);

#if defined(MICRO_OS_PLUS_DEBUG)
  micro_os_plus_architecture_brk ();
#endif // defined(MICRO_OS_PLUS_DEBUG)
  while (true)
    {
      micro_os_plus_architecture_wfi ();
    }
  /* NOTREACHED */
}

void __attribute__ ((weak, noreturn, alias ("_Exit"))) _exit (int status);

#pragma GCC diagnostic pop

#endif // MICRO_OS_PLUS_INCLUDE_EXIT

// ----------------------------------------------------------------------------

#endif // !Unix

// ----------------------------------------------------------------------------
