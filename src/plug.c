#include <stdio.h>
#include <stdbool.h>
#include "esp_log.h"

static const char *TAG = "plug";

/**
 * @brief initialize the plug lowlevel module
 */
void plug_init(void)
{
    ESP_LOGI(TAG, "Dummy Driver Init.");
}

/**
 * @brief turn on/off the lowlevel plug
 */
int plug_set_on(bool value)
{
    ESP_LOGI(TAG, "plug_set_on : %s", value == true ? "true" : "false");
    return 0;
}