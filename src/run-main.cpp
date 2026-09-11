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

#include "micro-os-plus/architecture.h"

#include "micro-os-plus/diag/trace.h"
#include "micro-os-plus/startup.h"
#include "micro-os-plus/semihosting.h"

#include <cstdlib>
#include <algorithm>

// TODO: further refine the versioning mechanism.
#if __has_include("micro-os-plus/version.h")
#include "micro-os-plus/version.h"
#else
#define MICRO_OS_PLUS_QUICK_VERSION_STRING "7.x"
#define MICRO_OS_PLUS_QUICK_YEAR_INTEGER "2026"
#endif // __has_include("micro-os-plus/version.h")

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_STARTUP_ENABLED)

// ----------------------------------------------------------------------------

using namespace micro_os_plus;

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_STARTUP_CALL_REAL_MAIN_ENABLED)

// When the `main()` function is wrapped, the user `main()` becomes
// `__real_main()`.

// Redefine `main` to `__real_main` that the startup will call the user
// function.
#define main __real_main

#endif // defined(MICRO_OS_PLUS_STARTUP_CALL_REAL_MAIN_ENABLED)

// ----------------------------------------------------------------------------

extern uint32_t __heap_begin__;
extern uint32_t __heap_end__;

// extern uint32_t __heap_begin__;
// extern uint32_t __HeapLimit;
// extern uint32_t end;
// extern uint32_t __HeapLimit;

typedef void (*function_ptr_t) (void);

// These magic symbols are provided by the linker. newlib standard.
extern function_ptr_t __preinit_array_start [[gnu::weak]][];
extern function_ptr_t __preinit_array_end [[gnu::weak]][];

extern function_ptr_t __init_array_start [[gnu::weak]][];
extern function_ptr_t __init_array_end [[gnu::weak]][];

extern function_ptr_t __fini_array_start [[gnu::weak]][];
extern function_ptr_t __fini_array_end [[gnu::weak]][];

extern "C"
{
  static void
  micro_os_plus_run_init_array (void);

  // Not static since it is called from exit()
  void
  micro_os_plus_run_fini_array (void);

  // Specific to newlib libgloss.
  void
  initialise_monitor_handles (void);

  int
  main (int argc, char* argv[]);
}

#if defined(__GNUC__)
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Waggregate-return"
#endif // defined(__GNUC__)

// Iterate over all the preinit/init routines (mainly static constructors).
[[gnu::always_inline]]
inline void
micro_os_plus_run_init_array (void)
{
  trace::printf ("%s()\n", __func__);

  if (&__preinit_array_start != &__preinit_array_end)
    {
      std::for_each (__preinit_array_start, __preinit_array_end,
                     [] (const function_ptr_t pf) { pf (); } //
      );
    }

  // If the application needs to run the code in the .init section,
  // please use the startup files, since this requires the code in
  // crti.o and crtn.o to add the function prologue/epilogue.
  //_init(); // DO NOT ENABLE THIS!

  if (&__init_array_start != &__init_array_end)
    {
      std::for_each (__init_array_start, __init_array_end,
                     [] (const function_ptr_t pf) { pf (); } //
      );
    }
}

// Run all the cleanup routines (mainly the static destructors).
void
micro_os_plus_run_fini_array (void)
{
  trace::printf ("%s()\n", __func__);

  if (&__fini_array_start != &__fini_array_end)
    {
      std::for_each (__fini_array_start, __fini_array_end,
                     [] (const function_ptr_t pf) { pf (); } //
      );
    }

  // If the application needs to run the code in the .fini section,
  // please use the startup files, since this requires the code in
  // crti.o and crtn.o to add the function prologue/epilogue.
  //_fini(); // DO NOT ENABLE THIS!
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__)

[[noreturn, gnu::weak]]
void
micro_os_plus_startup_run_main (void)
{
  // Initialize the trace output device. From this moment on,
  // trace::printf() calls are available (including in static
  // constructors).
  trace::initialise ();

#if defined(MICRO_OS_PLUS_VERSION_ENABLED)
  // For an accurate version, include the `@micro-os-plus/version` package.
  trace::puts (
      "\nµOS++ IIIe version " MICRO_OS_PLUS_STRING_MICRO_OS_PLUS_VERSION);
  trace::puts ("Copyright (c) 2007-" MICRO_OS_PLUS_STRING_MICRO_OS_PLUS_YEAR
               " Liviu Ionescu");
#else
  trace::puts ("\nµOS++ IIIe "
               "version " MICRO_OS_PLUS_QUICK_VERSION_STRING);
  trace::puts ("Copyright (c) 2007-" MICRO_OS_PLUS_QUICK_YEAR_INTEGER
               " Liviu Ionescu");
#endif

#if defined(__clang__)
  trace::printf ("Built with clang " __VERSION__);
#elif defined(__GNUC__)
  trace::printf ("Built with GCC " __VERSION__);
#else
#error "Built with an unknown compiler"
#endif
#if !(defined(__APPLE__) || defined(__linux__) || defined(__unix__) \
      || defined(WIN32))
// This is relevant only on bare-metal.
#if defined(__ARM_PCS_VFP) || defined(__ARM_FP)
  trace::printf (", with FP");
#else
  trace::printf (", no FP");
#endif
#endif
#if defined(__EXCEPTIONS)
  trace::printf (", with exceptions");
#else
  trace::printf (", no exceptions");
#endif
#if defined(MICRO_OS_PLUS_DEBUG_ENABLED)
  trace::printf (", with MICRO_OS_PLUS_DEBUG_ENABLED");
#endif // defined(MICRO_OS_PLUS_DEBUG_ENABLED)
#if defined(DEBUG)
  trace::printf (", with DEBUG");
#endif // defined(DEBUG)
  trace::puts ();

#if defined(MICRO_OS_PLUS_DEBUG_ENABLED) \
    || defined(MICRO_OS_PLUS_DIAG_TRACE_ENABLED)
  micro_os_plus_architecture_show_cpuid ();
#endif // defined(MICRO_OS_PLUS_DEBUG_ENABLED) || defined(MICRO_OS_PLUS_DIAG_TRACE_ENABLED)

  trace::puts ();

  int code = 0;

#if defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED)

  // Hook to continue the initializations. Usually compute and store the
  // clock frequency in a global variable, cleared above.
  code = micro_os_plus_startup_initialise_hardware_hook ();
  if (code != 0)
    {
      goto fail;
    }

  trace::puts ("Hardware initialized");

#endif // defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED

#if defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)
  initialise_monitor_handles ();
#endif // defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)

#if defined(MICRO_OS_PLUS_STARTUP_INITIALISE_FREE_STORE_ENABLED)

  // Must be done before `micro_os_plus_run_init_array()`, in case
  // dynamic memory is needed in constructors.
  micro_os_plus_startup_initialise_free_store_hook (
      &__heap_begin__, static_cast<std::size_t> (
                           (reinterpret_cast<char*> ((&__heap_end__))
                            - reinterpret_cast<char*> ((&__heap_begin__)))));

#endif // defined(MICRO_OS_PLUS_STARTUP_INITIALISE_FREE_STORE_ENABLED)

  // Warning: `malloc()` may need `errno` which may depend on knowing
  // the current thread.

  // Call the standard library initialization (mandatory for C++ to
  // execute the static objects constructors).
  micro_os_plus_run_init_array ();

  // Get the argc/argv (useful in semihosting configurations).

  // The application can redefine this function to fetch some arguments
  // from a non-volatile memory.

  int argc;
  char** argv;
  micro_os_plus_startup_initialise_args_hook (&argc, &argv);

  trace::dump_args (argc, argv);
  trace::puts ();

#if defined(__GNUC__)
#pragma GCC diagnostic push

// ISO C++ forbids taking address of function '::main' [-Wpedantic]
#pragma GCC diagnostic ignored "-Wpedantic"
#endif // defined(__GNUC__)

  // Call the main entry point, and save the exit code.
  code = main (argc, argv);

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__)

#if defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED)
fail:
#endif // defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED)

  // Standard program termination;
  // `atexit()` and C++ static destructors are executed.
  exit (code);

  // Oops, should not get here.
#if defined(MICRO_OS_PLUS_DEBUG_ENABLED)
  architecture::brk ();
#endif // defined(MICRO_OS_PLUS_DEBUG_ENABLED)
  while (true)
    {
      architecture::wfi ();
    }
  /* NOTREACHED */
}

// ----------------------------------------------------------------------------

// The RTOS redefines this function to display memory allocator reports or
// other statistics.
[[gnu::weak]]
void
micro_os_plus_terminate_goodbye (void)
{
  trace::puts ("\nHasta la vista!");
}

// ----------------------------------------------------------------------------

#endif // defined(MICRO_OS_PLUS_STARTUP_ENABLED)

// ----------------------------------------------------------------------------

#endif // !Unix

// ----------------------------------------------------------------------------
