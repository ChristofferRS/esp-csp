idf_component_register(SRCS "main.c" "lora.c" "csp_if_lora.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver libcsp
)
