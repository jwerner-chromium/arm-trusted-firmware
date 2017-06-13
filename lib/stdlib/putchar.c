/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <console.h>
#include <coreboot/cbmem_console.h>

/* Putchar() should either return the character printed or EOF in case of error.
 * Our current console_putc() function assumes success and returns the
 * character. Write all other printing functions in terms of putchar(), if
 * possible, so they all benefit when this is improved.
 */
int putchar(int c)
{
	int res;
	if (COREBOOT)
		coreboot_cbmc_putc(c);
	if (console_putc((unsigned char)c) >= 0)
		res = c;
	else
		res = EOF;

	return res;
}
