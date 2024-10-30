#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include "csp/csp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lora/csp_if_lora.h"
#include "wifi/wifi.h"
#include "chip_init.h"
#include "param/param.h"
#include "udp/csp_if_udp.h"
#include "param/param_server.h"
#include "vmem/vmem_server.h"
#include "vmem/vmem_ram.h"

uint8_t static _test_param;
PARAM_DEFINE_STATIC_RAM(22, test_param, PARAM_TYPE_UINT8, -1, 0, PM_CONF, NULL, NULL, &_test_param, "Test Parameter");

void vmem_server_task(void * param) {
	vmem_server_loop(param);
}

void router_task(void * param) {
	while(1) {
		csp_route_work();
	}
}

static double data_receieved = 0;
void data_callback(csp_packet_t * packet){
    data_receieved+= 8.0 * packet->length;
	csp_buffer_free(packet);
}

void app_main(){
    //csp_dbg_packet_print = 1;
    chip_init();
    //wifi_init_sta();

	csp_conf.version = 2;
	csp_conf.hostname = "ESP32-2";
	csp_conf.model = "1";
	csp_conf.revision = "1";
	csp_conf.conn_dfl_so = CSP_O_NONE;
	csp_conf.dedup = CSP_DEDUP_ALL;






    csp_init();

	csp_bind_callback(csp_service_handler, CSP_ANY);
    csp_bind_callback(param_serve, PARAM_PORT_SERVER);
    csp_bind_callback(data_callback, 8);

	static StaticTask_t vmem_server_tcb __attribute__((section(".noinit")));
	static StackType_t vmem_server_stack[5000] __attribute__((section(".noinit")));
	xTaskCreateStatic(vmem_server_task, "VMEMSRV", 5000, NULL, 1, vmem_server_stack, &vmem_server_tcb);

	static StackType_t router_stack[5000] __attribute__((section(".noinit")));
	static StaticTask_t router_tcb  __attribute__((section(".noinit")));
	xTaskCreateStatic(router_task, "RTE", 5000, NULL, 2, router_stack, &router_tcb);

    //csp_iface_t udp_iface;
    //csp_if_udp_conf_t udp_conf;
    //udp_conf.host = "192.168.1.40";
    //udp_conf.lport = 52002;
    //udp_conf.rport = 52001;
    //udp_conf.sockfd = 0;
    //udp_iface.addr = 11;
    //udp_iface.netmask = 11;
    //csp_if_udp_init(&udp_iface, &udp_conf);
    //csp_rtable_set(11, 11, &udp_iface, CSP_NO_VIA_ADDRESS);

    csp_iface_t lora_iface;
    lora_iface.addr = 19;
    lora_iface.is_default = 1;
    lora_iface.netmask = 11;
    csp_lora_init(&lora_iface);
    csp_rtable_set(19, 11, &lora_iface, CSP_NO_VIA_ADDRESS);

    csp_print("Interfaces\r\n");
    csp_iflist_print();

    csp_packet_t * packet = csp_buffer_get(0);
    while(1){
        //int rep = csp_ping(18, 200, 10, CSP_O_CRC32);
        //packet->length = 200;
        //for (int i = 0; i < 200; i++){
        //    packet->data[i] = i;
        //}
        //csp_sendto(CSP_PRIO_NORM,19, 8, 22, CSP_O_CRC32, packet);

        ESP_LOGI("Throughput","Received: %lf Mbit \t %lf Kbps",data_receieved, (data_receieved/1000.0)/2 );
        data_receieved = 0;
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

}

