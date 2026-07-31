#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define RELAY_PIN   0
#define LED_PIN     1
#define BTN_PIN     2
#define DEBOUNCE_US 20000

static const char *TAG = "smartplug";

void init()
{
    // Relay
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);

    // LED
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Button
    gpio_set_direction(BTN_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_PIN, GPIO_PULLUP_ONLY);
}

// pass 0 for LED off
// pass 1 for LED on
void setLED(int led)
{
    gpio_set_level(LED_PIN, led);
}

// pass 0 for relay open
// pass 1 for relay closed
void setRelay(int relay)
{
    gpio_set_level(RELAY_PIN, relay);
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
    init();

    int state = 0;
    setLED(state);   // Start with LED off
    setRelay(state); // Start with relay open

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
                    state = !state;
                    setLED(state);      // LED toggle
                    setRelay(state);    // Relay toggle
                    ESP_LOGI(TAG, "toggled, state=%d", state);
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}