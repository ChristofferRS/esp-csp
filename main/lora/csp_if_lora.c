#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include "csp/csp.h"
#include "csp/csp_id.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "lora.h"

//Struct provides locking of comm with lora chip since the chip can only
//be in oen state at the time it is not thread safe to have tx/rx going
static struct lora_driver_s{
    csp_iface_t *iface;
    SemaphoreHandle_t lock;
    StaticSemaphore_t lock_buf;
} lora_driver = {
    .lock = NULL,
};



int lora_tx(csp_iface_t *iface, uint16_t via, csp_packet_t *packet, int from_me){
    ESP_LOGI(pcTaskGetName(NULL), "Called lora tx");
    struct lora_driver_s *driver = iface->driver_data;
	csp_id_prepend(packet);
    //Lock -> Transmit -> unlock
    while(xSemaphoreTake(driver->lock, 10) == pdFALSE);
    lora_send_packet(packet->frame_begin, packet->frame_length);
    lora_receive(); // put into receive mode
    xSemaphoreGive(driver->lock);

    return CSP_ERR_NONE;
}


int work_lora_rx(struct lora_driver_s *drv){

    if(lora_received()) {
        csp_packet_t * packet = csp_buffer_get(0);
        if (packet == NULL) {
            return CSP_ERR_NOMEM;
        }
        int header_size = csp_id_setup_rx(packet);
        int received_len = lora_receive_packet((uint8_t *)packet->frame_begin, sizeof(packet->data) + header_size);
        if (received_len <= 4) {
            csp_buffer_free(packet);
            return CSP_ERR_NOMEM;
        }
        packet->frame_length = received_len;
        if (csp_id_strip(packet) != 0) {
            csp_buffer_free(packet);
            return CSP_ERR_INVAL;
        }
        csp_qfifo_write(packet, drv->iface, NULL);
    }
    return CSP_ERR_NONE;
}


void lora_rx_task(void* param){
    struct lora_driver_s *drv = param;
    while(1){
        if(xSemaphoreTake(drv->lock, 10) == pdTRUE){
            //ESP_LOGI(pcTaskGetName(NULL), "WORKING RX!!");
            work_lora_rx(drv);
            xSemaphoreGive(drv->lock);
        }
        //vTaskDelay(10 / portTICK_PERIOD_MS);
        vTaskDelay(1);
    }
}


int csp_lora_init(csp_iface_t *iface){
    iface->name = "LoRa";
    iface->nexthop = lora_tx;
    iface->driver_data = &lora_driver;
    lora_driver.iface = iface;
    lora_driver.lock = xSemaphoreCreateMutexStatic(&lora_driver.lock_buf);

    if (lora_init() == 0) {
        ESP_LOGE(pcTaskGetName(NULL), "Does not recognize the module");
        return 1;
    }

    lora_set_frequency(433e6); // 433MHz
    ESP_LOGI(pcTaskGetName(NULL), "Frequency is 433MHz");

    lora_enable_crc();

    int cr = 1;
    int bw = 7;
    int sf = 7;
    lora_set_coding_rate(cr);
    lora_set_bandwidth(bw);
    lora_set_spreading_factor(sf);
    lora_receive(); // put into receive mode


    ESP_LOGI(pcTaskGetName(NULL), "coding_rate=%d", cr);
    ESP_LOGI(pcTaskGetName(NULL), "bandwidth=%d", bw);
    ESP_LOGI(pcTaskGetName(NULL), "spreading_factor=%d", sf);

	static StackType_t lora_rx_stack[10000] __attribute__((section(".noinit")));
	static StaticTask_t lora_rx_tcb  __attribute__((section(".noinit")));
	xTaskCreateStatic(lora_rx_task, "LoRaRX", 10000, &lora_driver, 3, lora_rx_stack, &lora_rx_tcb);

	csp_iflist_add(lora_driver.iface);
    return CSP_ERR_NONE;
}
