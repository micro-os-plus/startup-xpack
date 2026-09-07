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
#include <cinttypes>

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

#if defined(__GNUC__)
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif // defined(__GNUC__)

// Redefine this function to initialise the free store.
void
micro_os_plus_startup_initialise_free_store_hook (void* heap_address,
                                                  std::size_t heap_size_bytes)
{
  trace::printf ("Heap: @0x%08" PRIXPTR " (%zu KiB)\n",
                 reinterpret_cast<std::uintptr_t> (heap_address),
                 heap_size_bytes / 1024);
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif // defined(__GNUC__)

// ----------------------------------------------------------------------------

#endif // defined(MICRO_OS_PLUS_STARTUP_ENABLED)

// ----------------------------------------------------------------------------

#endif // !Unix

// ----------------------------------------------------------------------------
