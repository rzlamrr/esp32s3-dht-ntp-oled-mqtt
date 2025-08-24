/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "esp_wifi.h"
#include <time.h>

#include "proj_wifi.h"
#include "proj_sntp.h"
#include "proj_mqtt.h"

static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

static uint8_t s_led_state = 0;

static led_strip_handle_t led_strip;

static void blink_led(void)
{
    /* If the addressable LED is enabled */
    if (s_led_state) {
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        led_strip_set_pixel(led_strip, 0, 16, 16, 16);
        /* Refresh the strip to send data */
        led_strip_refresh(led_strip);
    } else {
        /* Set all LED off to clear all pixels */
        led_strip_clear(led_strip);
    }
}


static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1, // at least one LED on board
    };
    led_strip_spi_config_t spi_config = {
        .spi_bus = SPI2_HOST,
        .flags.with_dma = true,
    };
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));

    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}


void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    configure_led();
    wifi_init_sta();
    time_t t = init_sntp();
    ESP_LOGI(TAG,"init_sntp(): %lld", t);
    init_mqtt();
    esp_log_level_set("*", ESP_LOG_WARN); 
    // ref: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/log.html#dynamic-log-level-control
     
#define payload_len 64
    multi_heap_info_t   heap_info;
    wifi_ap_record_t    ap_info;
    char payload_buf[payload_len];

    while (1) {
        ESP_LOGI(TAG, "Turning the LED %s at %lld!", s_led_state == true ? "ON" : "OFF", time(0));
        heap_caps_get_info(&heap_info, MALLOC_CAP_8BIT);
        esp_wifi_sta_get_ap_info(&ap_info);
        int len=snprintf(payload_buf, payload_len, "heap total:%u, used:%u, rssi:%d",
            heap_info.total_free_bytes, heap_info.total_allocated_bytes,
            ap_info.rssi);

        proj_mqtt_publish("/topic/repeating/C3", payload_buf, len, 0, 0);
        blink_led();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
