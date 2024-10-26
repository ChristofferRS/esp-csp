#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include "csp/csp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "csp_if_lora.h"
#include "lora.h"

void csp_print_func(const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    esp_log_writev(ESP_LOG_INFO,"csp", fmt, args);
    va_end(args);
}

void csp_reboot_hook(void) {
    esp_restart();
}

void csp_shutdown_hook(void) {
    esp_restart();
}

void router_task(void * param) {
	while(1) {
		csp_route_work();
	}
}


void app_main(){
	csp_conf.version = 2;
	csp_conf.hostname = "ESP32-1";
	csp_conf.model = "1";
	csp_conf.revision = "1";
	csp_conf.conn_dfl_so = CSP_O_NONE;
	csp_conf.dedup = CSP_DEDUP_OFF;

    csp_init();
    csp_lora_init(3);
    csp_print("Interfaces\r\n");
    csp_iflist_print();

	csp_bind_callback(csp_service_handler, CSP_ANY);
    //csp_sendto(uint8_t prio, uint16_t dest, uint8_t dport, uint8_t src_port, uint32_t opts, csp_packet_t * packet) {

	static StackType_t router_stack[5000] __attribute__((section(".noinit")));
	static StaticTask_t router_tcb  __attribute__((section(".noinit")));
	xTaskCreateStatic(router_task, "RTE", 5000, NULL, 2, router_stack, &router_tcb);

    while(1){
        int rep = csp_ping(10, 2000, 10, CSP_O_NONE);
        ESP_LOGI("ping","Ping reply: %d",rep);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

