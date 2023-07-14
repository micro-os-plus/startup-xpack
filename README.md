[![GitHub package.json version](https://img.shields.io/github/package-json/v/micro-os-plus/startup-xpack)](https://github.com/micro-os-plus/startup-xpack/blob/xpack/package.json)
[![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/micro-os-plus/startup-xpack)](https://github.com/micro-os-plus/startup-xpack/tags/)
[![npm (scoped)](https://img.shields.io/npm/v/@micro-os-plus/startup.svg?color=blue)](https://www.npmjs.com/package/@micro-os-plus/startup/)
[![license](https://img.shields.io/github/license/micro-os-plus/startup-xpack)](https://github.com/micro-os-plus/startup-xpack/blob/xpack/LICENSE)
[![CI on Push](https://github.com/micro-os-plus/startup-xpack/actions/workflows/ci.yml/badge.svg)](https://github.com/micro-os-plus/startup-xpack/actions/workflows/ci.yml)

## A source library xPack with the µOS++ portable startup code for bare-metal platforms

This project provides the **startup** source library as an xPack
dependency and includes a portable startup code for bare-metal platforms.

The myth that startup code must be written in assembly because
'the C environment is not ready' is not exactly right, and,
with some care to avoid undefined behaviour, can be
overcome.

The 'C environment' is mainly the stack (and maybe other architecture
ABI registers, like GP on RISC-V), and this can be set in a short
assembly entry code and then the standard `_start()` function can
be safely called.

The project is hosted on GitHub as
[micro-os-plus/startup-xpack](https://github.com/micro-os-plus/startup-xpack).

## Maintainer info

This page is addressed to developers who plan to include this source
library into their own projects.

For maintainer info, please see the
[README-MAINTAINER](README-MAINTAINER.md) file.

## Install

As a source library xPack, the easiest way to add it to a project is via
**xpm**, but it can also be used as any Git project, for example as a submodule.

### Prerequisites

A recent [xpm](https://xpack.github.io/xpm/),
which is a portable [Node.js](https://nodejs.org/) command line application.

It is recommended to update to the latest version with:

```sh
npm install --global xpm@latest
```

For details please follow the instructions in the
[xPack install](https://xpack.github.io/install/) page.

### xpm

Note: the package will be available from npmjs.com at a later date.

For now, it can be installed from GitHub:

```sh
cd my-project
xpm init # Unless a package.json is already present

xpm install github:micro-os-plus/startup-xpack
```

When ready, this package will be available as
[`@micro-os-plus/startup`](https://www.npmjs.com/package/@micro-os-plus/startup)
from the `npmjs.com` registry:

```sh
cd my-project
xpm init # Unless a package.json is already present

xpm install @micro-os-plus/startup@latest

ls -l xpacks/@micro-os-plus/startup
```

### Git submodule

If, for any reason, **xpm** is not available, the next recommended
solution is to link it as a Git submodule below an `xpacks` folder.

```sh
cd my-project
git init # Unless already a Git project
mkdir -p xpacks

git submodule add https://github.com/micro-os-plus/startup-xpack.git \
  xpacks/@micro-os-plus/startup
```

## Branches

Apart from the unused `master` branch, there are two active branches:

- `xpack`, with the latest stable version (default)
- `xpack-develop`, with the current development version

All development is done in the `xpack-develop` branch, and contributions via
Pull Requests should be directed to this branch.

When new releases are published, the `xpack-develop` branch is merged
into `xpack`.

## Developer info

### Overview

This source xPack provides the `_startup()` routine, as a replacement for
the newlib code in crt0.o or GCC code in crtbegin.o.

It is responsible for clearing the BSS, copying the DATA section,
and running the static constructors.

Specific for bare-metal applications, the code also perform
hardware initialisations, by calling custom hooks.

### Status

The **startup** source library is fully functional.

### C++ API

- none

### C API

This library defines a C function that can be called from the
reset handler.

```c
void __attribute__ ((noreturn, weak))
_start (void);
```

Optionally it can define:

```c
#if defined(MICRO_OS_PLUS_INCLUDE_EXIT)

void __attribute__ ((weak, noreturn))
abort (void);

void __attribute__ ((noreturn))
exit (int code);

void __attribute__ ((weak, noreturn))
_Exit (int code);

void __attribute__ ((weak, noreturn, alias ("_Exit")))
_exit (int status);

#endif // MICRO_OS_PLUS_INCLUDE_EXIT
```

and also:

```c
#if defined(MICRO_OS_PLUS_INCLUDE_SBRK)

void*
_sbrk (ptrdiff_t incr);

#endif // defined(MICRO_OS_PLUS_INCLUDE_SBRK)
```

### Hooks

The

```c
#if defined(MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE_EARLY)

void
micro_os_plus_startup_initialize_hardware_early (void);

#endif // MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE_EARLY

#if defined(MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE)

void
micro_os_plus_startup_initialize_hardware (void);

#endif // MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE

// A weak definition is provided here.
void
micro_os_plus_startup_initialize_free_store (void* heap_address,
                                              size_t heap_size_bytes);

// A weak definition is provided here. The RTOS redefines it.
// Called from _Exit().
void
micro_os_plus_terminate_goodbye (void);

// Must be defined by the device package.
void __attribute__ ((noreturn))
micro_os_plus_terminate (int code);
```

### Build & integration info

The project is written in C++, and it is expected
to be used in C and C++ projects.

The source code was compiled with GCC 11, clang 12, clang 13
arm-none-eabi-gcc 11, riscv-none-elf-gcc 12, and should be warning free.

To ease the integration of this package into user projects, there
are already made CMake and meson configuration files (see below).

For other build systems, consider the following details:

#### Include folders

The following folders should be passed to the compiler during the build:

- `include`

The header files to be included in user projects are:

```c++
#include <micro-os-plus/startup/hooks.h>
```

#### Source files

The source files to be added to the build are:

- `src/startup.cpp`

#### Preprocessor definitions

- `MICRO_OS_PLUS_INCLUDE_CONFIG_H` - to include `<micro-os-plus/config.h>`
- `MICRO_OS_PLUS_INCLUDE_STARTUP` - to include the startup code
- `MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE_EARLY`
- `MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE`
- `MICRO_OS_PLUS_INCLUDE_STARTUP_INIT_MULTIPLE_RAM_SECTIONS`
- `MICRO_OS_PLUS_INCLUDE_SBRK`
- `MICRO_OS_PLUS_INCLUDE_EXIT`
- `MICRO_OS_PLUS_BOOL_STARTUP_GUARD_CHECKS=false`

#### Compiler options

- `-std=c++20` or higher for C++ sources
- `-std=c11` for C sources

#### C++ Namespaces

- none

#### C++ Classes

- none

#### Dependencies

- the `@micro-os-plus/diag/trace` package, for the tracing features

#### CMake

To integrate the startup source library into a CMake application,
add this folder to the build:

```cmake
add_subdirectory("xpacks/@micro-os-plus/startup")`
```

The result is an interface library that can be added as an application
dependency with:

```cmake
target_link_libraries(your-target PRIVATE

  micro-os-plus::startup
)
```

#### meson

To integrate the startup source library into a meson application,
add this folder to the build:

```meson
subdir('xpacks/@micro-os-plus/startup')
```

The result is a dependency object that can be added
to an application with:

```meson
exe = executable(
  your-target,
  link_with: [
    # Nothing, not static.
  ],
  dependencies: [
    micro_os_plus_startup_dependency,
  ]
)
```

### Examples

TBD

### Known problems

- none

### Tests

TBD

## Change log - incompatible changes

According to [semver](https://semver.org) rules:

> Major version X (X.y.z | X > 0) MUST be incremented if any
backwards incompatible changes are introduced to the public API.

The incompatible changes, in reverse chronological order,
are:

- v4.x: use newlib linker script definitions
- v3.x: rename MICRO_OS_PLUS_DEBUG
- v2.x: rename namespaces, use `MICRO_OS_PLUS_` prefix
- v1.x: initial release

## License

The original content is released under the
[MIT License](https://opensource.org/licenses/MIT/),
with all rights reserved to
[Liviu Ionescu](https://github.com/ilg-ul).
