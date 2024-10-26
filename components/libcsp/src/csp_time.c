

#include <csp/arch/csp_time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

uint32_t csp_get_ms(void) {
	return (uint32_t)(xTaskGetTickCount() * (1000 / configTICK_RATE_HZ));
}

uint32_t csp_get_ms_isr(void) {
	return (uint32_t)(xTaskGetTickCountFromISR() * (1000 / configTICK_RATE_HZ));
}

uint32_t csp_get_s(void) {
	return (uint32_t)(xTaskGetTickCount() / configTICK_RATE_HZ);
}

uint32_t csp_get_s_isr(void) {
	return (uint32_t)(xTaskGetTickCountFromISR() / configTICK_RATE_HZ);
}
