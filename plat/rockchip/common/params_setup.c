/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arm_gic.h>
#include <assert.h>
#include <bl_common.h>
#include <console.h>
#include <debug.h>
#include <gpio.h>
#include <mmio.h>
#include <platform.h>
#include <plat_params.h>
#include <plat_private.h>
#include <string.h>

static struct gpio_info param_reset;
static struct gpio_info param_poweroff;
static struct apio_info param_apio;
static struct gpio_info *rst_gpio;
static struct gpio_info *poweroff_gpio;
static struct gpio_info suspend_gpio[10];
uint32_t suspend_gpio_cnt;
static struct apio_info *suspend_apio;

struct gpio_info *plat_get_rockchip_gpio_reset(void)
{
	return rst_gpio;
}

struct gpio_info *plat_get_rockchip_gpio_poweroff(void)
{
	return poweroff_gpio;
}

struct gpio_info *plat_get_rockchip_suspend_gpio(uint32_t *count)
{
	*count = suspend_gpio_cnt;

	return &suspend_gpio[0];
}

struct apio_info *plat_get_rockchip_suspend_apio(void)
{
	return suspend_apio;
}

static int rockchip_bl31_plat_param_handler(struct bl31_plat_param *param) {
	struct bl31_apio_param *apio_param;
	struct bl31_gpio_param *gpio_param;

	switch (param->tag) {
	case PARAM_RESET:
		gpio_param = (struct bl31_gpio_param *)param;
		memcpy(&param_reset, &gpio_param->gpio, sizeof(param_reset));
		rst_gpio = &param_reset;
		return 1;
	case PARAM_POWEROFF:
		gpio_param = (struct bl31_gpio_param *)param;
		memcpy(&param_poweroff, &gpio_param->gpio,
		       sizeof(param_poweroff));
		poweroff_gpio = &param_poweroff;
		return 1;
	case PARAM_SUSPEND_GPIO:
		if (suspend_gpio_cnt >= ARRAY_SIZE(suspend_gpio)) {
			ERROR("Supported suspend gpio number exceeded\n");
			return -1;
		}
		gpio_param = (struct bl31_gpio_param *)param;
		memcpy(&suspend_gpio[suspend_gpio_cnt], &gpio_param->gpio,
		       sizeof(suspend_gpio[0]));
		suspend_gpio_cnt++;
		return 1;
	case PARAM_SUSPEND_APIO:
		apio_param = (struct bl31_apio_param *)param;
		memcpy(&param_apio, &apio_param->apio, sizeof(param_apio));
		suspend_apio = &param_apio;
		return 1;
	}

	return 0;
}

void params_early_setup(void *from_bl2)
{
	bl31_plat_params_setup(from_bl2, rockchip_bl31_plat_param_handler);
}
