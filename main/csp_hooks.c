#include "csp/csp.h"
#include "esp_log.h"

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
