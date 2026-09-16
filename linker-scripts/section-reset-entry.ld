/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2022-2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

/*
 * Replace this file with a custom one if needed.
 *
 * `__reset_entry_align` can be overridden from outside this script,
 * without editing it, by passing it on the command line. For example,
 * `gcc -Wl,--defsym=__reset_entry_align=32 ...`.
 */
__reset_entry_align = DEFINED(__reset_entry_align) ? __reset_entry_align : 16;

SECTIONS
{
  /* ----------------------------------------------------------------------- */

  .reset_entry : ALIGN(__reset_entry_align)
  {
    FILL(0xFF)

    __reset_entry_start = ABSOLUTE(.) ;

    /*
     * On RISC-V devices, the reset address is a plain code address rather
     * than a hardware vector table, so this must be the very first section
     * in the image. Apart from the assembly code used to initialise the
     * stack, do not place anything else in this section.
     */
    KEEP(*(SORT_NONE(.reset_entry) SORT_NONE(.reset_entry.*)))
  } >FLASH

  /* ----------------------------------------------------------------------- */
}
