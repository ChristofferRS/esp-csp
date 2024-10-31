/*
 * vmem flash storage implementation
 * based on the file implementation
 */

#include "vmem/vmem.h"
#include "vmem_flash.h"
#include "esp_log.h"
#include "esp_partition.h"
#include <string.h>


void vmem_flash_init(vmem_t* vmem){
    ((vmem_flash_driver_t *) vmem->driver)->partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, "libparam");
}
void vmem_flash_read(vmem_t * vmem, uint64_t addr, void * dataout, uint32_t len) {
    esp_partition_t* part = ((vmem_flash_driver_t *) vmem->driver)->partition;
    esp_partition_read(part, addr, dataout, len);
}

void vmem_flash_write(vmem_t * vmem, uint64_t addr, const void * datain, uint32_t len) {
    esp_partition_t* part = ((vmem_flash_driver_t *) vmem->driver)->partition;
	memcpy(((vmem_flash_driver_t *) vmem->driver)->physaddr + addr, datain, len);
    esp_partition_erase_range(part, 0x0, FLASH_BUFFER_SIZE);
    esp_partition_write(part, 0x0, ((vmem_flash_driver_t *) vmem->driver)->physaddr, FLASH_BUFFER_SIZE);
}




