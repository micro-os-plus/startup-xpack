# Change & release log

Releases in reverse chronological order.

Please check
[GitHub](https://github.com/micro-os-plus/startup-xpack/issues/)
and close existing issues and pull requests.

## 2026-08-13

* v6.1.0
* 26e7993 #12: split the specific startup code

## 2026-08-05 

* 2641a45 #if defined() cosmetics

## 2026-08-04

* 1b33bf6 README update
* 2622be1 6.0.0
* 70e2dd5 prepare v6.0.0
* 9edddca re-generate top commons

## 2026-07-31

* 83329ca CHANGELOG update
* cd769ae re-generate top xcdl
* 481fb25 re-generate top commons
* 707d168 cosmetise includes

## 2026-07-28

* 6bbf017 cosmetise includes
* 8aa3527 cosmetise #if defined()/#endif // defined()
* a8d34b5 startup.cpp fix trace::printf format
* 804b19e update copyright notices

## 2026-07-20

* 00f6ebc prepare 6.0.0
* 66ac557 update for new xcdl macros
* 3c3f9ad update with xcdl-export
* fdfc9d2 update xcdl-package.jsonc
* 3b0832c re-generate top commons

## 2026-06-11

* fce1977 prefer project-config.h

## 2026-06-06

* 96ddc1f use __has_include(<micro-os-plus/config.h>)
* eb580d4 copyright update 2026

## 2025-11-20

* ec98bef rename npm-pack

## 2025-10-07

* 7a971d9 update copyright notices

## 2023-11-28

* 5e6ed62 README updates
* 2d49acf package.json: cosmetise scripts

## 2023-10-19

* aa48828 package.json: cosmetise description
* 714f4fe README updates

## 2023-07-14

* 03d0318 README updates
* 9615bf7 package.json: cosmetise urls
* 782fb27 package.json: minXpm 0.16.2

## 2023-06-04

* 99ed1bd update for @scope/name

## 2023-06-03

* f5a9a8a lower case ci.yml
* 73fc208 package.json min 0.16.0

## 2023-05-08

* b37fe81 .npmignore update
* 9ad993e README updates
* 34b49be README updates
* 9bcc4e9 meson.build cosmetics

## 2022-08-16

* 918c43b package.json min 0.14.0 & defaults

## 2022-08-03

* f6cc591 README updates
* 5bfb7af .vscode/settings.json: cmake.ignoreCMakeListsMissing
* d938287 .vscode/settings.json: makefile.configureOnOpen

## 2022-08-02

* 109c859 READMEs updates

## 2022-07-30

* d8d1d81 5.2.0
* cb13874 prepare v5.2.0
* 0dc5ed1 #11: Display "Built with GCC..." line on trace

## 2022-07-28

* v5.1.0
* 3bad7e6 cosmetics in messages
* 811d1f1 #7: use cast to init __dso_handle
* e8d6c00 #8: add exit.c & _sbrk.c to CMake/meson
* bb3e7d2 #10: make version dependency optional

## 2022-07-25

* 88c5477 startup.cpp: use trace::puts() for goodbye
* c529619 hooks.h: check MICRO_OS_PLUS_HAS_INTERRUPTS_STACK
* 2fbd2c4 add preliminary xpack.json

## 2022-07-24

* ef8040d xpack.json: no more camelCase for option names

## 2022-07-23

* d7c32b8 add experimental xpack.json
* 9233e0b #7: __dso_handle inside startup; no hack
* f28cb35 #9: MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_MULTIPLE_RAM_SECTIONS

## 2022-07-22

* v5.0.0
* 517b806 README: list preprocessor options
* 03ad60d #8: check MICRO_OS_PLUS_INCLUDE_EXIT
* 88fe2c6 #8: move exit() and _sbrk() here
* 0ed8089 #7: Conditionally add local __dso_handle hack
* f92cd60 #6: use MICRO_OS_PLUS_USE_SEMIHOSTING
* 51bec0c #5: MICRO_OS_PLUS_INCLUDE_STARTUP_INITIALIZE_HARDWARE
* 7b448e9 startup.cpp: remove trailing dot in messages

## 2022-06-09

* v4.0.0 released
* 9114a3c use newlib linker script definitions
* 24cd0c9 MICRO_OS_PLUS_INCLUDE_STARTUP

## 2022-06-04

* v3.1.1

## 2022-05-26

* dacf413 simplify trace::puts()
* f20dfa5 simplify to trace::puts
* 7260518 copy .data only if needed

## 2022-02-15

* v3.0.0

## 2022-02-10

* 4d967f4 .c* include <micro-os-plus/config.h>

## 2022-02-09

* d39404a rename MICRO_OS_PLUS_DEBUG

## 2022-02-05

* v2.2.1

## 2022-02-04

* v2.2.0
* 49f6df2 cmake: add xpack_display_target_lists

## 2022-02-02

* fab0c59 add meson support
* 20c0000 startup.cpp: trace Heap

## 2022-01-28

* 3d875d8 rework CMake, top config, no sources

## 2022-01-27

* 7442ca1 add call to initialise_monitor_handles()

## 2022-01-26

* rework CMake

## 2022-01-02

* v2.0.2

## 2021-03-09

* v2.0.1
* fix/silence warnings

## 2021-03-01

* v2.0.0

## 2021-02-28

* rename micro_os_plus

## 2021-02-04

* v1.1.0
* first restructured release

## 2020-09-19

* v1.0.8
* bump diag-trace to 1.0.7

## 2018-04-16

* v1.0.7
* move deps back to npm
* update README developer info
* bump dependencies

## 2018-01-25

* v1.0.6
* update dependencies

## 2018-01-24

* v1.0.5

## 2018-01-21

* [#2] Move os_terminate() to architecture/device.

## 2018-01-15

* Use wfi() in infinite loops

## 2018-01-09

* v1.0.4
* README updates

## 2017-10-04

* v1.0.3
* package.json clean-ups
* add CHANGELOG.md
* use portable nop/brk
* move `trace::initalize()` before `os_startup_initialize_hardware()`
* add notice about undefined behaviour

## 2017-09-18

* v1.0.2
* use for_each to iterate init/fini; on release the code is slightly shorter!

## 2017-09-04

* v1.0.1
* initial version, copied from µOS++.
