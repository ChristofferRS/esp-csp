idf_component_register(SRCS
                        "main.c" "chip_init.c" "csp_hooks.c"
                        "lora/csp_if_lora.c" "lora/lora.c"
                        "wifi/wifi.c"
                        "udp/csp_if_udp.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES driver libcsp esp_wifi nvs_flash libparam
)



