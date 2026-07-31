idf_component_register(
    SRCS "main.c" "homekit.c"
    INCLUDE_DIRS "."
    REQUIRES esp_hap_core app_wifi app_hap_setup_payload
)