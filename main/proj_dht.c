#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "dht.h"

#include "proj_dht.h"

static const char *TAG = "DHT";

// --- Atur pin GPIO untuk DHT22 di menuconfig ---
#define SENSOR_GPIO CONFIG_DHT_GPIO

// Variabel untuk menyimpan data terakhir yang berhasil dibaca
static float last_temperature = 0;
static float last_humidity = 0;

void dht_reader_task(void *pvParameters)
{
    // Sensor DHT22 tidak bisa dibaca terlalu cepat (minimal interval 2 detik)
    // Jadi kita buat task terpisah yang membacanya setiap 5 detik
    while(1) {
        float temperature, humidity;

        if (dht_read_float_data(DHT_TYPE_AM2301, SENSOR_GPIO, &humidity, &temperature) == ESP_OK) {
            ESP_LOGI(TAG, "Humidity: %.1f%% Temp: %.1fC", humidity, temperature);
            last_humidity = humidity;
            last_temperature = temperature;
        } else {
            ESP_LOGE(TAG, "Could not read data from sensor");
        }
        
        vTaskDelay(5000 / portTICK_PERIOD_MS); // Tunggu 5 detik
    }
}

void init_dht(void)
{
    // Buat task yang akan berjalan di background untuk membaca sensor
    xTaskCreate(dht_reader_task, "dht_reader_task", 2048, NULL, 5, NULL);
}

float get_temperature(void)
{
    return last_temperature;
}

float get_humidity(void)
{
    return last_humidity;
}
