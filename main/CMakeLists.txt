idf_component_register(SRCS "main.c" "chip_init.c" "csp_hooks.c" "csp_if_lora.c" "lora.c" "wifi.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver libcsp esp_wifi nvs_flash
)



