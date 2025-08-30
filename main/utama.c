/* Blink Example - Modified for NTP OLED Clock

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "led_strip.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"


// --- Tambahan Include untuk OLED ---
#include "ssd1306.h"
#include "font8x8_basic.h"

// --- Include dari proyek Anda ---
// #include "proj_wifi.h"
#include "proj_sntp.h"
#include "proj_mqtt.h"
#include "proj_dht.h"

static const char *TAG = "OLED_CLOCK";

// --- Variabel dan fungsi untuk LED Blink ---
#define BLINK_GPIO CONFIG_BLINK_GPIO
static uint8_t s_led_state = 0;
static led_strip_handle_t led_strip;

static void blink_led(void)
{
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
    // --- Inisialisasi Sistem (HANYA DILAKUKAN SEKALI DI SINI) ---
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());


    // Inisialisasi LED
    configure_led();

    // 1. Inisialisasi Layar OLED
    SSD1306_t dev;
    // Pastikan pin I2C sesuai dengan konfigurasi di menuconfig
    i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);
    
    // Tampilkan pesan status awal di OLED
    ssd1306_display_text(&dev, 3, "Connecting...", 13, false);

    // 2. Inisialisasi WiFi, SNTP, dan DHT
    // wifi_init_sta();
    ESP_ERROR_CHECK(example_connect());
    time_t t = init_sntp();
    ESP_LOGI(TAG,"SNTP initialized, time: %lld", t);
    init_dht();

    // 3. Atur Zona Waktu (PENTING!)
    setenv("TZ", CONFIG_TIMEZONE, 1);
    tzset();

    // 4. Inisialisasi MQTT
    ESP_LOGI(TAG, "MQTT Initializing...");
    init_mqtt();
    
    ESP_LOGI(TAG, "Setup complete. Starting display loop.");

    // --- Pindahkan deklarasi buffer ke luar loop ---
    char time_str[20];
    char date_str[16];
    char sensor_str[40];
    char json_payload[200];
    // char iso_time_str[30]; // Tidak diperlukan lagi untuk JSON

    // 5. Loop Utama
    while (1) {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        // Format string waktu dan tanggal (statis)
        strftime(time_str, sizeof(time_str), "%H:%M:%S KST", &timeinfo);
        strftime(date_str, sizeof(date_str), "%d-%m-%Y", &timeinfo);

        // Format string sensor (untuk di-scroll)
        // Tambahkan spasi di akhir agar ada jeda saat teks berulang
        float temperature = get_temperature();
        float humidity = get_humidity();
        sprintf(sensor_str, "Suhu: %.1f C   Lembap: %.1f %%   ", temperature, humidity);

        // Bersihkan layar sekali di awal loop
        ssd1306_clear_screen(&dev, false);

        // Tampilkan teks statis
        ssd1306_display_text(&dev, 0, "Jam Digital", 11, false);
        ssd1306_display_text(&dev, 2, time_str, strlen(time_str), false);
        ssd1306_display_text(&dev, 4, date_str, strlen(date_str), false);

        // Buat payload JSON untuk MQTT dengan Unix Timestamp
        // Perhatikan: %lld untuk time_t dan tidak ada tanda kutip di sekitar nilainya
        sprintf(json_payload, 
                "{\"time\":%lld,\"suhu\":%.1f,\"humid\":%.1f}",
                (long long)now, temperature, humidity);
        
        // Kirim data ke MQTT
        proj_mqtt_publish(CONFIG_MQTT_TOPIC, json_payload, strlen(json_payload), 0, 0);

        // Tampilkan teks sensor yang berjalan di baris paling bawah (page 7)
        // Fungsi ini bersifat blocking dan akan menjadi delay utama untuk loop ini
        ssd1306_display_text_box2(&dev, 7, 0, sensor_str, 16, strlen(sensor_str), false, 5);

        // Kedipkan LED sekali setiap siklus scroll selesai
        blink_led();
        s_led_state = !s_led_state;

        // Tidak perlu vTaskDelay lagi di sini, karena delay sudah ditangani
        // oleh fungsi ssd1306_display_text_box2()
    }
}
