/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <asm_macros.S>

	.globl	coreboot_cbmc_putc
	.globl	coreboot_cbmc_flush

/*
 * Write a character to the CBMEM console, including overflow handling. Since
 * this may be called from crash handling code (right before
 * plat_console_crash_putc), we have to conform to that function's ABI which
 * means only X1 and X2 are available as scratch registers. X0 holds the
 * character to be printed, but we have to preserve it for the actual crash
 * console. Since a character is only 8 bits, we commandeer some of the top
 * 16 bits of W0 as temporary scratch space.
 */
func coreboot_cbmc_putc
	ldr	x2, coreboot_cbmc
	ldr	w1, [x2, #4]		/* load cursor */
	ror	w0, w0, #16		/* rotate character to high 16 bits */
	bfxil	w0, w1, #28, #4		/* extract flags into low 16 bits */
	and	w1, w1, #0x0fffffff	/* mask out flags in cursor */
	ldr	w2, coreboot_cbmc_size	/* load size */
	cmp	w1, w2			/* if cursor >= size... (illegal!) */
	b.hs	2f			/* ...return (must be malicious) */
	ror	w0, w0, #16		/* rotate character back in place */
	ldr	x2, coreboot_cbmc
	add	x2, x2, #8		/* point X2 to &cbmc->body */
	strb	w0, [x2, w1, uxtw]	/* write character */
	add	w1, w1, #1		/* cursor++ */
	ror	w0, w0, #16		/* rotate W0 back to flags */
	ldr	w2, coreboot_cbmc_size	/* load size */
	cmp	w1, w2			/* if cursor < size... */
	bfi	w1, w0, #28, #4		/* [but first reinsert flags field] */
	b.lt	1f			/* ...then skip overflow handling */
	and	w1, w1, #0xf0000000	/* set cursor to zero (flags remain) */
	orr	w1, w1, #(1 << 31)	/* set overflow flag */
1:	ldr	x2, coreboot_cbmc
	str	w1, [x2, #4]		/* write back cursor to memory */
2:	ror	w0, w0, #16		/* rotate character back in place */
	and	w0, w0, #0x000000ff	/* zero scratch space we used in W0 */
	ret
endfunc coreboot_cbmc_putc

/*
 * Flush the memory area used for the CBMEM console from the cache. Similar
 * register restrictions as above (can only use X0 through X3).
 */
func coreboot_cbmc_flush
	ldr	x0, coreboot_cbmc	/* load pointer to structure */
	ldr	x1, coreboot_cbmc_size	/* load size of console body */
	add	x1, x1, #8		/* add sizeof(struct cbmem_console) */
	b	clean_dcache_range	/* (clobbers x2 and x3) */
endfunc coreboot_cbmc_flush
