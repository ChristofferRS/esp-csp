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



PARAM_DEFINE_STATIC_RAM(22, test_param, PARAM_TYPE_UINT8, -1, 0, PM_CONF, NULL, NULL, 0x00, "Test Parameter");

void vmem_server_task(void * param) {
	vmem_server_loop(param);
}

void router_task(void * param) {
	while(1) {
		csp_route_work();
	}
}


void app_main(){
    chip_init();
    wifi_init_sta();

	csp_conf.version = 2;
	csp_conf.hostname = "ESP32-1";
	csp_conf.model = "1";
	csp_conf.revision = "1";
	csp_conf.conn_dfl_so = CSP_O_NONE;
	csp_conf.dedup = CSP_DEDUP_OFF;

    csp_init();

    csp_iface_t udp_iface;
    csp_if_udp_conf_t udp_conf;
    udp_conf.host = "192.168.1.40";
    udp_conf.lport = 52002;
    udp_conf.rport = 52001;
    udp_conf.sockfd = 0;
    udp_iface.addr = 4;
    udp_iface.netmask = 1;
    csp_if_udp_init(&udp_iface, &udp_conf);

    csp_print("Interfaces\r\n");
    csp_iflist_print();

	csp_bind_callback(csp_service_handler, CSP_ANY);
    csp_print("BINDING PARAM SERV TO %d\r\n", PARAM_PORT_SERVER);
    csp_bind_callback(param_serve, PARAM_PORT_SERVER);

	static StackType_t router_stack[5000] __attribute__((section(".noinit")));
	static StaticTask_t router_tcb  __attribute__((section(".noinit")));
	xTaskCreateStatic(router_task, "RTE", 5000, NULL, 2, router_stack, &router_tcb);

	static StaticTask_t vmem_server_tcb __attribute__((section(".noinit")));
	static StackType_t vmem_server_stack[1000] __attribute__((section(".noinit")));
	xTaskCreateStatic(vmem_server_task, "VMEMSRV", 1000, NULL, 1, vmem_server_stack, &vmem_server_tcb);

    while(1){
        //int rep = csp_ping(10, 2000, 10, CSP_O_NONE);
        //ESP_LOGI("ping","Ping reply: %d",rep);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    //   csp_lora_init(3);

    //   //csp_sendto(uint8_t prio, uint16_t dest, uint8_t dport, uint8_t src_port, uint32_t opts, csp_packet_t * packet) {


    //   while(1){
    //       //int rep = csp_ping(10, 2000, 10, CSP_O_NONE);
    //       //ESP_LOGI("ping","Ping reply: %d",rep);
    //       vTaskDelay(2000 / portTICK_PERIOD_MS);
    //   }

}

