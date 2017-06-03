/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PLAT_PARAMS_H__
#define __PLAT_PARAMS_H__

/*
 * CAREFUL: This header may be included directly from external code bases (e.g.
 * coreboot). Do not add any further includes unless you can ensure that they're
 * available in all dependent code bases and provide all required declarations.
 */
#include <bl31_plat_params.h>
#include <stdint.h>

#define BL31_GPIO_DIR_OUT		0
#define BL31_GPIO_DIR_IN		1

#define BL31_GPIO_LEVEL_LOW		0
#define BL31_GPIO_LEVEL_HIGH		1

#define BL31_GPIO_PULL_NONE		0
#define BL31_GPIO_PULL_UP		1
#define BL31_GPIO_PULL_DOWN		2

/* Rockchip-specific BL31 platform parameter tags. */
enum {
	PARAM_NONE = BL31_PLAT_PARAM_VENDOR_BASE,
	PARAM_RESET,
	PARAM_POWEROFF,
	PARAM_SUSPEND_GPIO,
	PARAM_SUSPEND_APIO,
};

struct apio_info {
	uint8_t apio1 : 1;
	uint8_t apio2 : 1;
	uint8_t apio3 : 1;
	uint8_t apio4 : 1;
	uint8_t apio5 : 1;
};

struct gpio_info {
	uint8_t polarity;
	uint8_t direction;
	uint8_t pull_mode;
	uint32_t index;
};

struct bl31_gpio_param {
	struct bl31_plat_param h;
	struct gpio_info gpio;
};

struct bl31_apio_param {
	struct bl31_plat_param h;
	struct apio_info apio;
};

#endif /* __PLAT_PARAMS_H__ */
