/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <coreboot/cbmem_console.h>
#include <xlat_tables_v2.h>

/*
 * The console is initially validated by coreboot, so we can trust it. But at
 * runtime the non-secure OS could manipulate the size field to trick us into
 * making bad memory accesses. Therefore we need to store the known-good size
 * value at boot time. This way, even if the non-secure OS corrupts other
 * console contents later, it can not trick our putc code into writing outside
 * of the original buffer.
 */
struct cbmem_console {
	uint32_t size_dont_access_after_boot;
	uint32_t cursor;
	uint8_t body[0];
};
#define CURSOR_MASK ((1 << 28) - 1)
#define OVERFLOW (1 << 31)

static uint8_t preinit_buffer[256];
static const char overflow_warning[] = "*** Pre-CBMEM BL31 console overflowed, "
	"log truncated! ***\n";
struct cbmem_console *coreboot_cbmc = (struct cbmem_console *)&preinit_buffer;
uint32_t coreboot_cbmc_size = sizeof(preinit_buffer) -
			      sizeof(struct cbmem_console);

static void expand_and_mmap(uintptr_t baseaddr, size_t size)
{
	uintptr_t pageaddr = round_down(baseaddr, PAGE_SIZE);
	size_t expanded = round_up(baseaddr - pageaddr + size, PAGE_SIZE);
	mmap_add_region(pageaddr, pageaddr, expanded,
			MT_MEMORY | MT_RW | MT_NS | MT_EXECUTE_NEVER);
}

void coreboot_cbmc_init(uintptr_t baseaddr) {
	struct cbmem_console *preinit_console = coreboot_cbmc;
	uint32_t preinit_console_size = coreboot_cbmc_size;
	uint32_t i;

	coreboot_cbmc = (struct cbmem_console *)baseaddr;
	coreboot_cbmc_size = coreboot_cbmc->size_dont_access_after_boot;

	if (preinit_console->cursor & OVERFLOW) {
		for (i = 0; i < sizeof(overflow_warning) - 1; i++)
			coreboot_cbmc_putc(overflow_warning[i]);
		for (i = preinit_console->cursor & CURSOR_MASK;
		     i < preinit_console_size; i++)
			coreboot_cbmc_putc(preinit_console->body[i]);
	}
	for (i = 0; i < (preinit_console->cursor & CURSOR_MASK); i++)
		coreboot_cbmc_putc(preinit_console->body[i]);

	expand_and_mmap(baseaddr, coreboot_cbmc_size);
}
