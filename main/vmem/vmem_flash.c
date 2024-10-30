/*
 * vmem flash storage implementation
 * based on the file implementation
 */

#include "vmem/vmem.h"
#include "vmem_flash.h"
#include "esp_log.h"
#include "esp_partition.h"
#include <string.h>

static esp_partition_t * part = NULL;

void vmem_flash_init(vmem_t* vmem){
    part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
    if(part==NULL){
        ESP_LOGI("ERR","NO PARTITION FOUND!!");
    }else{
        ESP_LOGI("OK","!!!PARTITION FOUND %s!!",part->label);
    }
    ((vmem_flash_driver_t *) vmem->driver)->partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
}
void vmem_flash_read(vmem_t * vmem, uint64_t addr, void * dataout, uint32_t len) {
    //esp_partition_t* part = ((vmem_flash_driver_t *) vmem->driver)->partition;
    ESP_LOGI("vmem", "READ FROM FLASH %s  len  %lu  addr 0x%016llx\n!",part->label,len, addr);
    ESP_ERROR_CHECK(esp_partition_read(part, addr, dataout, len));
}

void vmem_flash_write(vmem_t * vmem, uint64_t addr, const void * datain, uint32_t len) {
    //esp_partition_t* part = ((vmem_flash_driver_t *) vmem->driver)->partition;
    ESP_LOGI("vmem", "WRITE TO FLASH %s  len  %lu  addr 0x%016llx\n (%lu)!",part->label,len, addr, part->erase_size);
	memcpy(((vmem_flash_driver_t *) vmem->driver)->physaddr + addr, datain, len);

    //WRITE UPDATED BUFFER TO PARTITION
    ESP_ERROR_CHECK(esp_partition_erase_range(part, 0x0, FLASH_BUFFER_SIZE));
    ESP_ERROR_CHECK(esp_partition_write(part, 0x0, ((vmem_flash_driver_t *) vmem->driver)->physaddr, FLASH_BUFFER_SIZE));
}




