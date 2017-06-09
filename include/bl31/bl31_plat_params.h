/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BL31_PLAT_PARAMS_H
#define __BL31_PLAT_PARAMS_H

/*
 * CAREFUL: This header may be included directly from external code bases (e.g.
 * coreboot). Do not add any further includes unless you can ensure that they're
 * available in all dependent code bases and provide all required declarations.
 */
#include <stdint.h>

/*
 * This header describes a common BL31 platform parameter format that may be
 * used by individual platform implementations to receive parameters from BL2.
 * Parameters are contained in a linked list of variable-length structures. All
 * structures share a common 16-byte header with a tag and a pointer to the next
 * parameter. For the last parameter in the list, this points to NULL. The tag
 * identifies the format of the 0 or more bytes of tag-specific parameter data
 * that follows. Below is an illustration of a two-parameter list:
 *
 * plat_params_from_bl2 ---> +----------------+ --+
 *                           |      tag       |   |
 *                           +----------------+   |--> struct bl31_plat_param
 *                      +----|      next      |   |
 *                      |    +----------------+ --+
 *                      |    | parameter data |
 *                      |    +----------------+
 *                      |
 *                      +--> +----------------+ --+
 *                           |      tag       |   |
 *                           +----------------+   |--> struct bl31_plat_param
 *                  NULL <---|      next      |   |
 *                           +----------------+ --+
 *                           | parameter data |
 *                           +----------------+
 *
 * Note: This list is parsed before the MMU is enabled (i.e. as strongly
 * ordered memory), so we cannot make unaligned accesses. BL2 must ensure that
 * all structures are naturally aligned.
 */

/*
 * Tags 0x0 - 0xbfffffff are free for platform-specific use. Tags 0xc0000000 to
 * 0xffffffff are reserved for use by common code.
 */
enum bl31_plat_param_tag {
	BL31_PLAT_PARAM_VENDOR_BASE = 0,
	/* ... */
	BL31_PLAT_PARAM_COMMON_BASE = 0xc0000000,
	BL31_PLAT_PARAM_COREBOOT_TABLE,
	BL31_PLAT_PARAM_MAX = 0xffffffff
};

struct bl31_plat_param {
	uint32_t tag;
	uint32_t _reserved;	/* for future expansion (maybe 'flags'?) */
	void *next;
};

/* @return 1: recognized, handled | 0: not recognized | -1: recognized, error */
typedef int (*bl31_plat_param_handler_t)(struct bl31_plat_param *param);

/*
 * This function should be called from early platform initialization code,
 * passing in the bl31_plat_param argument that was passed from BL2 to BL31
 * in X1. It can also be passed a platform-specific parameter handler function
 * that will be called individually for each parameter that is not recognized by
 * common code. It needs to be called before the MMU is set up since structures
 * passed from BL2 may reside in memory areas that are not accessible from
 * normal BL31 page tables. All parameter contents that will be needed later
 * must be copied out of the parameter structure.
 */
void bl31_plat_params_setup(void *from_bl2, bl31_plat_param_handler_t handler);

#endif /* __BL31_PLAT_PARAMS_H */
