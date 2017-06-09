/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __COREBOOT_TABLE_H
#define __COREBOOT_TABLE_H

#include <coreboot/params.h>

void coreboot_console_init(void);
void coreboot_table_setup(struct bl31_plat_param *param);

#endif /* __COREBOOT_TABLES_H */
