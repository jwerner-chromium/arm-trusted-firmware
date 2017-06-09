/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <console.h>
#include <coreboot/coreboot_table.h>
#include <debug.h>
#include <mmio.h>
#include <string.h>

/*
 * Structures describing coreboot's in-memory descriptor tables. See
 * <coreboot>/src/commonlib/include/commonlib/coreboot_tables.h for
 * canonical implementation.
 */

struct cb_header {
	char signature[4];
	uint32_t header_bytes;
	uint32_t header_checksum;
	uint32_t table_bytes;
	uint32_t table_checksum;
	uint32_t table_entries;
};

enum cb_tag {
	CB_TAG_SERIAL = 0xf,
};

struct cb_serial {
	uint32_t type;			/* always 2 (memory-mapped) on ARM */
	uint32_t baseaddr;
	uint32_t baud;
	uint32_t regwidth;		/* in bytes, i.e. usually 4 */
	uint32_t input_hertz;
	uint32_t uart_pci_addr;		/* unused on current ARM systems */
};
static struct cb_serial serial;

struct cb_entry {
	uint32_t tag;
	uint32_t size;
	union {
		struct cb_serial serial;
	};
};

/*
 * The coreboot table is parsed before the MMU is enabled (i.e. with strongly
 * ordered memory), so we cannot make unaligned accesses. The table entries
 * immediately follow one another without padding, so nothing after the header
 * is guaranteed to be naturally aligned. Therefore, we need to define safety
 * functions that can read an unaligned integers.
 */
static uint32_t read_le32(uint32_t *p)
{
	uintptr_t addr = (uintptr_t)p;
	return mmio_read_8(addr)		|
	       mmio_read_8(addr + 1) << 8	|
	       mmio_read_8(addr + 2) << 16	|
	       mmio_read_8(addr + 3) << 24;
}

void coreboot_table_setup(struct bl31_plat_param *param)
{
	struct bl31_coreboot_table_param *cb_param = (void *)param;
	struct cb_header *header = (struct cb_header *)cb_param->baseaddr;
	void *ptr;
	int i;

	if (strncmp(header->signature, "LBIO", 4)) {
		ERROR("coreboot table signature corrupt!\n");
		return;
	}

	ptr = (void *)header + header->header_bytes;
	for (i = 0; i < header->table_entries; i++) {
		struct cb_entry *entry = ptr;

		if (ptr - (void *)header >= header->header_bytes +
					    header->table_bytes) {
			ERROR("coreboot table exceeds its bounds!\n");
			break;
		}

		switch (read_le32(&entry->tag)) {
		case CB_TAG_SERIAL:
			memcpy(&serial, &entry->serial, sizeof(serial));
			break;
		default:
			/* There are many tags TF doesn't need to care about. */
			break;
		}

		ptr += read_le32(&entry->size);
	}
}

void coreboot_console_init()
{
	if (serial.type)
		console_init(serial.baseaddr, serial.input_hertz, serial.baud);
}
