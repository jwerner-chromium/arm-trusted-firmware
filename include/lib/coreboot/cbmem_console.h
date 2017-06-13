/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CBMEM_CONSOLE_H
#define __CBMEM_CONSOLE_H

#include <types.h>

void coreboot_cbmc_init(uintptr_t baseaddr);
void coreboot_cbmc_putc(int c);
void coreboot_cbmc_flush(void);

#endif /* __CBMEM_CONSOLE_H */
