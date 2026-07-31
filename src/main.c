#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "homekit.h"

#define RELAY_PIN   0
#define BTN_PIN     2
#define LED_PIN     8       // ESP32-C3 onboard LED
#define DEBOUNCE_US 20000

static const char *TAG = "ESP32-SmartPlug";

void init()
{
    // === Init GPIOs === //
    // Relay
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    // LED
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    // Button
    gpio_set_direction(BTN_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_PIN, GPIO_PULLUP_ONLY);
    ESP_LOGI(TAG, "GPIOs initialized");

    // === Init Homekit === //
    homekit_start();
    ESP_LOGI(TAG, "Homekit initialized");
}

// pass 0 for LED off / relay closed
// pass 1 for LED on / relay open
void setState(bool state)
{
    gpio_set_level(RELAY_PIN, state);
    gpio_set_level(LED_PIN, !state);    // onboard LED inverted
}

// return 0 if button not pressed
// return 1 if button pressed
int checkBtn()
{
    int btnState = gpio_get_level(BTN_PIN);

    if (!btnState)
    {
        ESP_LOGI(TAG, "button pressed");
        return 1;
    }
    else
    {
        return 0;
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Enter app_main");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    init();

    int plugState = 0;
    setState(plugState);         // Start with LED off / relay open

    int lastRawReading = 0;      // last raw pin read
    int debouncedState = 0;      // confirmed, stable state
    int64_t lastChangeTime = 0;  // when raw reading last changed

    while (1)
    {
        int rawReading = checkBtn();  // read ONCE per loop
        if (rawReading != lastRawReading)
        {
            lastChangeTime = esp_timer_get_time();
            lastRawReading = rawReading;
        }
        if ((esp_timer_get_time() - lastChangeTime) > DEBOUNCE_US)
        {
            if (rawReading != debouncedState)
            {
                debouncedState = rawReading;

                // Only toggle on the press edge (0 -> 1), not release
                if (debouncedState == 1)
                {
                    plugState = !plugState;
                    plug_set_on(plugState);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}