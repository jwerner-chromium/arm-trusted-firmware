#
# Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

BL31_SOURCES	+=	$(addprefix lib/coreboot/,	\
			${ARCH}/cbmem_console_asm.S	\
			cbmem_console.c			\
			coreboot_table.c)
