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
 * `__vectors_align` can be overridden from outside this script,
 * without editing it, by passing it on the command line. For example,
 * `gcc -Wl,--defsym=__vectors_align=256 ...` is required for Pico,
 * which has a 256-byte bootstrap that the interrupt vectors must
 * immediately follow.
 */
__vectors_align = DEFINED(__vectors_align) ? __vectors_align : 1024;

SECTIONS
{
  /* ----------------------------------------------------------------------- */

  /*
   * For Cortex-M devices, the beginning of the startup code is stored in
   * the .interrupt_vectors section, which is placed in FLASH.
   *
   * The minimum alignment is 128, but the exact value is (16+N)*4 rounded
   * up to the next power of two. The default of 1024 is enough for up to
   * 240 interrupts; for larger devices, increase `__vectors_align`
   * accordingly.
   */
  .interrupt_vectors : ALIGN(__vectors_align)
  {
    FILL(0xFF)

    __vectors_start = ABSOLUTE(.) ;
    __vectors_start__ = ABSOLUTE(.) ; /* STM-specific definition */

    /* 
     * Interrupt vectors, pointers to exceptions and interrupt handlers.
     * For Cortex-M, the first two words hold the reset handler and the
     * initial stack.
     */
    KEEP(*(.interrupt_vectors .interrupt_vectors.*))

    /* Freescale specific configuration words. */
    KEEP(*(.cfmconfig .cfmconfig.*))

    /*
     * Some devices require additional boot metadata immediately after the
     * interrupt vectors, within the first few KB of FLASH. For example, the
     * RP2350 boot ROM looks for a PICOBIN metadata block here to identify
     * and locate the image (see
     * `platforms/raspberry-pi-pico-2-arm/device/src/embedded-block-rp2350.c`
     * in the µTest++ tests); this matches how the Pico SDK's own linker
     * script places its `.embedded_block` in the same output section as
     * the vector table. On devices that define no `.embedded_block` input
     * section, this contributes nothing.
     */
    KEEP(*(.embedded_block .embedded_block.*))
  } >FLASH

  /* ----------------------------------------------------------------------- */
}
