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

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_STARTUP_ENABLED)

// ----------------------------------------------------------------------------

#if !defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)

// Semihosting uses a more elaborate version of
// micro_os_plus_startup_initialise_args_hook() to parse arguments received from
// host.

#if defined(__GNUC__)
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif // defined(__GNUC__)

// This is the standard default implementation for the routine to
// process arguments. It returns a single empty arg.
//
// For semihosting applications, this is redefined to get the real
// arguments from the debugger.
//
// The application can redefine it to fetch some arguments from a
// non-volatile memory.

[[gnu::weak]]
void
micro_os_plus_startup_initialise_args_hook (int* p_argc, char*** p_argv)
{
  // By the time we reach this, the data and bss should have been initialized.

  // The strings pointed to by the argv array shall be modifiable by the
  // program, and retain their last-stored values between program startup
  // and program termination. (static, no const)
  static char name[] = "";

  // The string pointed to by argv[0] represents the program name;
  // argv[0][0] shall be the null character if the program name is not
  // available from the host environment. argv[argc] shall be a null pointer.
  // (static, no const)
  static char* argv[2] = { name, NULL };

  *p_argc = 1;
  *p_argv = &argv[0];

  return;
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__)

#endif // !defined(MICRO_OS_PLUS_SEMIHOSTING_ENABLED)

// ----------------------------------------------------------------------------

#endif // defined(MICRO_OS_PLUS_STARTUP_ENABLED)

// ----------------------------------------------------------------------------

#endif // !Unix

// ----------------------------------------------------------------------------
