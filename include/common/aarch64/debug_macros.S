/*
 * Copyright (c) 2014-2016, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __DEBUG_MACROS_S__
#define __DEBUG_MACROS_S__

	/*
	 * Theses macros are wrappers around plat_crash_console_putc and
	 * plat_crash_console_flush that serve as a single place to implement
	 * hooks for other crash console backends that might be compiled in.
	 */

	 /* In: x0 = character to be printed. Clobber: x30, x1, x2 */
	 .macro crash_console_putc
#if COREBOOT
	bl	coreboot_cbmc_putc
#endif
	bl	plat_crash_console_putc
	.endm

	 /* Clobber: x30, x0, x1, x2, x3 */
	 .macro crash_console_flush
#if COREBOOT
	bl	coreboot_cbmc_flush
#endif
	bl	plat_crash_console_flush
	.endm


#endif /* __DEBUG_MACROS_S__ */
