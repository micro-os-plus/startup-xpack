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

#include <cstdint>
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

extern std::uint32_t __heap_begin__;
extern std::uint32_t __heap_end__;

// extern uint32_t __heap_begin__;
// extern uint32_t __HeapLimit;
// extern uint32_t end;
// extern uint32_t __HeapLimit;

using function_ptr_t = void (*) (void);

// These magic symbols are provided by the linker. newlib standard.
extern function_ptr_t __preinit_array_start [[gnu::weak]][];
extern function_ptr_t __preinit_array_end [[gnu::weak]][];

extern function_ptr_t __init_array_start [[gnu::weak]][];
extern function_ptr_t __init_array_end [[gnu::weak]][];

extern function_ptr_t __postinit_array_start [[gnu::weak]][];
extern function_ptr_t __postinit_array_end [[gnu::weak]][];

extern function_ptr_t __fini_array_start [[gnu::weak]][];
extern function_ptr_t __fini_array_end [[gnu::weak]][];

// Note: Strictly speaking, according to the recent C/C++ standards,
// using symbols defined in the linker scripts rely on undefined
// behaviour, since comparing pointers that do not point to elements
// of the same area or members of the same object is undefined.
// The danger is that compilers that perform very aggressive
// optimisations may completely remove such code.
// If this happens, the workaround is to disable the specific
// optimisation that caused it, or reduce the optimisation level
// for this file only.

extern "C"
{
  static void
  micro_os_plus_run_init_array (void) noexcept;

  // Not static since it is called from exit()
  void
  micro_os_plus_run_fini_array (void) noexcept;

  /**
   * @brief Initialise the semihosting monitor file handles.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   *
   * @details
   * Specific to newlib libgloss; provided by the semihosting library,
   * not defined in this package.
   */
  void
  initialise_monitor_handles (void);

  /**
   * @brief The application entry point.
   * @param [in] argc Number of command line arguments.
   * @param [in] argv Array of pointers to the command line argument
   *  strings; `argv[argc]` is a null pointer.
   * @return The exit code, passed on to `exit()`.
   *
   * @details
   * Implemented by the application; not defined in this package.
   */
  int
  main (int argc, char* argv[]);
}

#if defined(__GNUC__)
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Waggregate-return"
#endif // defined(__GNUC__)

/**
 * @brief Run the preinit/init array.
 * @par Parameters
 *  None.
 * @par Returns
 *  Nothing.
 *
 * @details
 * Iterates over the preinit array, then the init array (mainly C++
 * static constructors), and calls each entry in order. Each array is
 * skipped entirely if empty (start and end coincide).
 */
[[gnu::always_inline]]
inline void
micro_os_plus_run_init_array (void) noexcept
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

  if (&__postinit_array_start != &__postinit_array_end)
    {
      std::for_each (__postinit_array_start, __postinit_array_end,
                     [] (const function_ptr_t pf) { pf (); } //
      );
    }
}

/**
 * @brief Run the fini array.
 * @par Parameters
 *  None.
 * @par Returns
 *  Nothing.
 *
 * @details
 * Iterates over the fini array (mainly C++ static destructors) and
 * calls each entry in order. Skipped entirely if empty. Not `static`,
 * since it is also called from `exit()` (`exit.c`).
 */
void
micro_os_plus_run_fini_array (void) noexcept
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

/**
 * @brief Initialise and run `main()`.
 * @par Parameters
 *  None.
 * @par Returns
 *  Nothing.
 *
 * @details
 * Initialises tracing, runs the preinit/init arrays (C++ static
 * constructors), fetches `argc`/`argv`, calls `main()`, and finally
 * calls `exit()` with the value `main()` returned. Never returns.
 */
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
#endif // defined(MICRO_OS_PLUS_VERSION_ENABLED)

#if defined(__clang__)
  trace::printf ("Built with clang " __VERSION__);
#elif defined(__GNUC__)
  trace::printf ("Built with GCC " __VERSION__);
#else
#error "Built with an unknown compiler"
#endif // defined(__clang__)
#if !(defined(__APPLE__) || defined(__linux__) || defined(__unix__) \
      || defined(WIN32))
// This is relevant only on bare-metal.
#if defined(__ARM_PCS_VFP) || defined(__ARM_FP)
  trace::printf (", with FP");
#else
  trace::printf (", no FP");
#endif // defined(__ARM_PCS_VFP) || defined(__ARM_FP)
#endif // !Unix
#if defined(__EXCEPTIONS)
  trace::printf (", with exceptions");
#else
  trace::printf (", no exceptions");
#endif // defined(__EXCEPTIONS)
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
#endif /* defined(MICRO_OS_PLUS_DEBUG_ENABLED)
           || defined(MICRO_OS_PLUS_DIAG_TRACE_ENABLED) */

  int code = 0;

#if defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED)

  // Hook to continue the initialisations. Usually compute and store the
  // clock frequency in a global variable, cleared above.
  code = micro_os_plus_startup_initialise_hardware_hook ();
  if (code != 0)
    {
      goto fail;
    }

  trace::puts ();
  trace::puts ("Hardware initialised");

#endif // defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED)

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

  // Call the standard library initialisation (mandatory for C++ to
  // execute the static objects constructors).
  micro_os_plus_run_init_array ();

#if defined(MICRO_OS_PLUS_STARTUP_POST_INIT_ARRAY_ENABLED)

  // Hook to continue the initialisations. Usually compute and store the
  // clock frequency in a global variable, cleared above.
  code = micro_os_plus_startup_post_init_array_hook ();
  if (code != 0)
    {
      goto fail;
    }

#endif // defined(MICRO_OS_PLUS_STARTUP_POST_INIT_ARRAY_ENABLED)

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

#if defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED) \
    || defined(MICRO_OS_PLUS_STARTUP_POST_INIT_ARRAY_ENABLED)
fail:
#endif /* defined(MICRO_OS_PLUS_STARTUP_INITIALISE_HARDWARE_ENABLED) 
       || defined(MICRO_OS_PLUS_STARTUP_POST_INIT_ARRAY_ENABLED) */

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

/**
 * @brief Display statistics and say goodbye before terminating.
 * @par Parameters
 *  None.
 * @par Returns
 *  Nothing.
 *
 * @details
 * The default (weak) implementation only prints a farewell message.
 * An RTOS or application may redefine this function to display memory
 * allocator reports or other statistics before the application
 * terminates.
 */
[[gnu::weak]]
void
micro_os_plus_startup_exit_goodbye_hook (void)
{
  trace::puts ("\nHasta la vista!");
}

// ----------------------------------------------------------------------------

#endif // defined(MICRO_OS_PLUS_STARTUP_ENABLED)

// ----------------------------------------------------------------------------

#endif // !Unix

// ----------------------------------------------------------------------------
