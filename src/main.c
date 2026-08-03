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

    // === Init Homekit === //
    homekit_start();
}

// pass 0 for LED off / relay closed, 1 for LED on / relay open
void setIO(bool state)
{
    gpio_set_level(RELAY_PIN, state);
    gpio_set_level(LED_PIN, !state);    // onboard LED inverted
}

// returns the toggled plug state
int checkBtnWithDebounce(int state)
{
    static int lastRawReading      = 0;        // last raw pin read
    static int debouncedState      = 0;        // confirmed, stable state
    static int64_t lastChangeTime  = 0;        // when raw reading last changed

    int rawReading = gpio_get_level(BTN_PIN);

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
                return state;
            }
        }
    }
    return state;
}

void app_main(void)
{
    init();

    int plugState = 0;
    plug_set(plugState);    // Start with LED off / relay open

    while (1)
    {
        if (checkBtnWithDebounce(plugState) != plugState)
        {
            plugState = !plugState;
            plug_set(plugState);
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}