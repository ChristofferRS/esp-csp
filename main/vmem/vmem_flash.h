#pragma once
#include "vmem/vmem.h"
#include "esp_partition.h"
//TODO: Implement "page roaming" for partition erase_size
#define FLASH_BUFFER_SIZE 4096
typedef struct {
    esp_partition_t* partition;
    uint32_t     addr;
	void * physaddr;
} vmem_flash_driver_t;

void vmem_flash_init(vmem_t * vmem);
void vmem_flash_read(vmem_t * vmem, uint64_t addr, void * dataout, uint32_t len);
void vmem_flash_write(vmem_t * vmem, uint64_t addr, const void * datain, uint32_t len);

//TODO: .ack_with_pull = 1 needed on macro=
#define VMEM_DEFINE_FLASH(name_in, strname, size_in, _vaddr) \
	uint8_t vmem_##name_in##_buf[size_in] = {}; \
	static vmem_flash_driver_t vmem_##name_in##_driver = { \
		.physaddr = vmem_##name_in##_buf, \
	}; \
	__attribute__((section("vmem"))) \
	__attribute__((aligned(1))) \
	__attribute__((used)) \
	vmem_t vmem_##name_in = { \
		.type = VMEM_TYPE_FLASH, \
		.name = strname, \
		.size = size_in, \
		.read = vmem_flash_read, \
		.write = vmem_flash_write, \
		.driver = &vmem_##name_in##_driver, \
		.vaddr =  _vaddr,\
	};


