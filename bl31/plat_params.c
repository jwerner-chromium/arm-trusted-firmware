/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <bl31_plat_params.h>
#include <coreboot/coreboot_table.h>
#include <debug.h>
#include <stddef.h>

void bl31_plat_params_setup(void *from_bl2, bl31_plat_param_handler_t handler) {
	struct bl31_plat_param *param;

	for (param = from_bl2; param != NULL; param = param->next) {
		/* Always try handler first, may need to override common code */
		if (handler && handler(param))
			continue;

		switch (param->tag) {
		case BL31_PLAT_PARAM_COREBOOT_TABLE:
			if (COREBOOT)
				coreboot_table_setup(param);
			else
				ERROR("Got coreboot table parameter, but "
				      "coreboot support wasn't compiled in!\n");
			break;
		default:
			ERROR("Unrecognized BL31 platform parameter tag: %d\n",
			      param->tag);
			break;
		}
	}
}
