/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __COREBOOT_PARAMS_H
#define __COREBOOT_PARAMS_H

/*
 * CAREFUL: This header is included directly from coreboot sources. Do not add
 * any further includes unless you make sure that they're available in coreboot
 * and provide all required declarations.
 */
#include <bl31_plat_params.h>

struct bl31_coreboot_table_param {
	struct bl31_plat_param h;
	void *baseaddr;
};

#endif /* __COREBOOT_PARAMS_H */
