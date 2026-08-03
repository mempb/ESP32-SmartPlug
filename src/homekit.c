#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include <iot_button.h>
#include <app_wifi.h>
#include <app_hap_setup_payload.h>
#include <homekit.h>

static const char *TAG = "ESP32-SmartPlug";

#define PLUG_TASK_PRIORITY  1
#define PLUG_TASK_STACKSIZE 4 * 1024
#define PLUG_TASK_NAME      "hap_plug"

#define RESET_NETWORK_BUTTON_TIMEOUT        3
#define RESET_TO_FACTORY_BUTTON_TIMEOUT     10
#define RESET_GPIO  GPIO_NUM_10

static hap_char_t *on_char;

static void reset_network_handler(void* arg)
{
    hap_reset_network();
}

static void reset_to_factory_handler(void* arg)
{
    hap_reset_to_factory();
}

static void reset_key_init(uint32_t key_gpio_pin)
{
    button_handle_t handle = iot_button_create(key_gpio_pin, BUTTON_ACTIVE_LOW);
    iot_button_add_on_release_cb(handle, RESET_NETWORK_BUTTON_TIMEOUT, reset_network_handler, NULL);
    iot_button_add_on_press_cb(handle, RESET_TO_FACTORY_BUTTON_TIMEOUT, reset_to_factory_handler, NULL);
}

static int plug_identify(hap_acc_t *ha)
{
    ESP_LOGI(TAG, "Accessory identified");
    return HAP_SUCCESS;
}

static int plug_write(hap_write_data_t write_data[], int count, void *serv_priv, void *write_priv)
{
    int i, ret = HAP_SUCCESS;
    hap_write_data_t *write;
    for (i = 0; i < count; i++) {
        write = &write_data[i];
        *(write->status) = HAP_STATUS_VAL_INVALID;
        if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
            ESP_LOGI(TAG, "Received Write for Plug %s", write->val.b ? "On" : "Off");
            if (plug_set(write->val.b) == 0) {
                *(write->status) = HAP_STATUS_SUCCESS;
            }
        } else {
            *(write->status) = HAP_STATUS_RES_ABSENT;
        }
        if (*(write->status) == HAP_STATUS_SUCCESS) {
            hap_char_update_val(write->hc, &(write->val));
        } else {
            ret = HAP_FAIL;
        }
    }
    return ret;
}

static void plug_thread_entry(void *arg)
{
    hap_acc_t *accessory;
    hap_serv_t *service;

    hap_init(HAP_TRANSPORT_WIFI);

    hap_acc_cfg_t cfg = {
        .name = "ESP32-SmartPlug",
        .manufacturer = "mempb",
        .model = "EspPlg01",
        .serial_num = "EspPlg01",
        .fw_rev = "0.9.0",
        .hw_rev = "1.0",
        .pv = "1.1.0",
        .identify_routine = plug_identify,
        .cid = HAP_CID_LIGHTING,
    };

    accessory = hap_acc_create(&cfg);
    if (!accessory) {
        ESP_LOGE(TAG, "Failed to create accessory");
        goto plug_err;
    }

    uint8_t product_data[] = {'E','S','P','3','2','H','A','P'};
    hap_acc_add_product_data(accessory, product_data, sizeof(product_data));
    hap_acc_add_wifi_transport_service(accessory, 0);
    service = hap_serv_outlet_create(false, false);
    if (!service) {
        ESP_LOGE(TAG, "Failed to create plug Service");
        goto plug_err;
    }

    on_char = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ON);

    int ret = hap_serv_add_char(service, hap_char_name_create("My Plug"));
    
    if (ret != HAP_SUCCESS) {
        ESP_LOGE(TAG, "Failed to add optional characteristics to Plug");
        goto plug_err;
    }

    hap_serv_set_write_cb(service, plug_write);
    hap_acc_add_serv(accessory, service);
    hap_add_accessory(accessory);
    ESP_LOGI(TAG, "reached point add");
    reset_key_init(RESET_GPIO);
    hap_set_setup_code("111-22-333");
    hap_set_setup_id("ES32");
    app_hap_setup_payload("111-22-333", "ES32", false, cfg.cid);
    app_wifi_init();
    hap_start();
    app_wifi_start(portMAX_DELAY);

    vTaskDelete(NULL);

plug_err:
    hap_acc_delete(accessory);
    vTaskDelete(NULL);
    ESP_LOGI(TAG, "reached point plug_err");
}

int plug_set(bool value)
{
    setIO(value);      // state toggle
    ESP_LOGI(TAG, "toggled, state=%d", value);

    // Update HomeKit state
    if (on_char) {
        hap_val_t new_val = { .b = value };
        hap_char_update_val(on_char, &new_val);
    }
    return 0;
}

void homekit_start()
{
    xTaskCreate(plug_thread_entry, PLUG_TASK_NAME, PLUG_TASK_STACKSIZE, NULL, PLUG_TASK_PRIORITY, NULL);
}