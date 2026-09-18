# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

@.github/copilot-instructions.md

## Project overview

The µOS++ **startup** source library (an xpm package): portable C/C++20 startup
code for bare-metal embedded targets (Cortex-M, RISC-V), replacing newlib's
`crt0.o`/GCC's `crtbegin.o`. It clears BSS, copies DATA from flash to RAM,
runs the preinit/init arrays (C++ static constructors), calls `main()`, and
implements `exit()`/`abort()`/`_sbrk()`. There is no build output: this is a
source-only library meant to be compiled directly into a consuming firmware
project, via xpm, CMake `add_subdirectory()`, meson `subdir()`, or a Git
submodule under `xpacks/@micro-os-plus/startup`.

## Build files are generated — do not hand-edit

`CMakeLists.txt` and `meson.build` are both generated from
`xcdl-package.jsonc` (the single source of truth for components, source
files and dependencies) via the Liquid templates in `scripts/templates/`.
After editing `xcdl-package.jsonc`, regenerate both with:

```sh
xpm run xcdl-export
```

Never hand-edit `CMakeLists.txt` or `meson.build`; changes are overwritten on
the next export. `.github/workflows/test-ci.yml` and `.vscode/settings.json`
are likewise generated from an upstream `npm-packages-helper` template (see
each file's header comment) and should be changed at the template source,
not locally.

## Common commands

- `xpm run xcdl-export` — regenerate `CMakeLists.txt`/`meson.build` from
  `xcdl-package.jsonc`
- `xpm run clang-format` — reformat C/C++ sources (`scripts/clang-format.mjs`)
- `xpm run cmake-format` — reformat CMake files (`scripts/cmake-format.mjs`)
- `xpm run jsonc-format` — reformat JSON/JSONC files
  (`scripts/jsonc-format.mjs`)
- `npm run git-log` — short, dated one-line git log

The test commands from the Testing section above (`xpm run test -C tests`,
`test-native-clang`, `test-qemu-cortex-m7f-gcc`) depend on `tests/top`, a
linked/generated test project that is not part of this repository (it is
listed in `.gitignore`, and `tests/CMakeLists.txt` here is currently a
stub). Populate it first with `xpm run install-all`, then
`xpm run test-all`, as described in `README-MAINTAINER.md`.

## Architecture

The startup sequence only compiles for bare-metal targets; every source file
under `src/` is `#if`-guarded out on macOS/Linux/Unix builds.

1. `src/_start.cpp` — `_start()`, entered from the reset handler (Cortex-M's
   `Reset_Handler`, or a small RISC-V asm stub). Copies `.data` from flash to
   RAM and zero-fills `.bss`, using symbols defined by
   `linker-scripts/*.ld`. Supports both a single RAM region and multiple
   RAM regions
   (`MICRO_OS_PLUS_STARTUP_INITIALISE_MULTIPLE_RAM_SECTIONS_ENABLED`), plus
   optional guard-value corruption checks in debug builds. Ends by calling
   `micro_os_plus_startup_run_main()`.
2. `src/run-main.cpp` — `micro_os_plus_startup_run_main()`: initialises
   tracing, runs the preinit/init arrays (C++ static constructors), fetches
   `argc`/`argv` via `initialise-args.cpp`, calls `main()`, then `exit()`.
3. `src/exit.c` — `exit()`, `abort()`, `_Exit()`/`_exit()`: runs `atexit()`
   handlers and the fini array (static destructors), then calls
   `micro_os_plus_startup_exit()`, which in turn calls the weak
   `micro_os_plus_startup_exit_terminate_hook()`.
4. `src/_sbrk.c`, `src/initialise-free-store.cpp` — heap/free-store setup,
   coordinated with the linker scripts.

Every optional stage is a `[[gnu::weak]]` function gated by a
`MICRO_OS_PLUS_STARTUP_*_ENABLED` macro (see
`include/micro-os-plus/startup.h`, and the `cdlOptions`/nested
`cdlComponents` in `xcdl-package.jsonc`), so a consuming application
overrides only the hooks it needs (for example
`micro_os_plus_startup_initialise_hardware_hook()`,
`micro_os_plus_startup_exit_terminate_hook()`) while the rest keep their
default, often trace-only, behaviour. This library depends on
`micro-os-plus::architecture`, `micro-os-plus::diag-trace` and
`micro-os-plus::semihosting` (sibling xPacks resolved under
`xpacks/@micro-os-plus/`); when a hook or `#include` crosses that boundary,
read it alongside the dependency's source.

`README.md`'s API description lags recent additions (for example the
`finalise-hardware` hook and `call-real-main` option); treat
`include/micro-os-plus/startup.h` and `xcdl-package.jsonc` as authoritative
for current hook names and macros.
