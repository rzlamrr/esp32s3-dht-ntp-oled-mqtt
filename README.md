# ESP32-S3 IoT Weather Station

This project transforms an ESP32-S3 into a comprehensive IoT environmental monitoring station. It captures real-time temperature and humidity data, displays it locally on an OLED screen along with the current time, and transmits the data to an MQTT broker for remote monitoring and data logging.

This project is built upon the foundational work from [HankB/ESP32-ESP-IDF-CLI-start](https://github.com/HankB/ESP32-ESP-IDF-CLI-start) and has been significantly expanded to include new features and a modular architecture. The final version of the code is available in this repository.

> **Note:** This project was developed for academic and learning purposes. It serves as a practical demonstration of integrating various IoT technologies on the ESP32 platform.

-----

## ✨ Key Features

  - **Real-time Clock:** Synchronizes with an NTP server to display the accurate date and time.
  - **Environmental Sensing:** Reads temperature and humidity from a DHT22 sensor.
  - **Local Display:** Presents data on an SSD1306 OLED screen with a clean user interface, including static info and a dynamic scrolling text area for sensor data.
  - **IoT Connectivity:** Publishes sensor data and a Unix timestamp in JSON format to an MQTT broker for easy integration with other services like Node-RED, Home Assistant, or custom dashboards.
  - **Status Indicator:** Utilizes an addressable LED for visual feedback on the device's operational status.
  - **Highly Configurable:** All major parameters (WiFi, MQTT, GPIO pins) can be easily configured via the ESP-IDF `menuconfig` utility.

-----

## 🛠️ Hardware Requirements

  - ESP32-S3 Development Board
  - SSD1306 I2C OLED Display (128x64)
  - DHT22 (or AM2301) Temperature and Humidity Sensor
  - Addressable LED (e.g., WS2812B)
  - Breadboard and jumper wires

-----

## 💾 Software & Dependencies

This project is built using the **ESP-IDF v5.5** framework. The necessary components are managed via the `idf_component.yml` file and include:

  - **`protocol_examples_common`**: ESP-IDF helper for network connectivity.
  - **`nopnop2002/ssd1306`**: Driver for the SSD1306 OLED display.
  - **`espressif/led_strip`**: Official driver for addressable LED strips.
  - **`esp-idf-lib/dht`**: Library for the DHT series of sensors.

-----

## ⚙️ Configuration

This project is designed to be highly modular and configurable without changing the source code. Use the ESP-IDF configuration tool by running `idf.py menuconfig` in your project's root directory.

Key configuration menus are:

  - **`SSD1306 Configuration`**: Set up your OLED display's I2C pins (SDA, SCL) and other display-related settings.
  - **`Example Connection Configuration`**: Configure your Wi-Fi network credentials (SSID and Password).
  - **`Main Configuration`**: Set other application-specific parameters, including:
      - GPIO pin for the DHT22 sensor.
      - MQTT broker URL and topic.
      - SNTP server address.
      - Timezone string.

-----

## 🚀 How to Build and Flash

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/rzlamrr/esp32s3-dht-ntp-oled-mqtt
    cd esp32s3-dht-ntp-oled-mqtt
    ```

2.  **Set your ESP-IDF target:**

    ```bash
    idf.py set-target esp32s3
    ```

3.  **Configure the project:**

    ```bash
    idf.py menuconfig
    ```

    (Set up your WiFi and GPIO pins as described in the Configuration section above).

4.  **Build, Flash, and Monitor:**

    ```bash
    idf.py build flash monitor
    ```

-----

## 📊 Functionality in Detail

### OLED Display

The local display provides immediate visual feedback.

  - **Static Area:** The top portion of the screen shows the title ("Jam Digital"), the current time, and the date. This information is updated with every cycle.
  - **Scrolling Area:** The bottom-most line is a dynamic ticker that scrolls a string containing the current temperature and humidity.

### MQTT Data Transmission

The device acts as a reliable IoT node by publishing data in a structured JSON format to the `/weather/data` topic (or as configured).

**Payload Example:**

```json
{
  "time": 1756569692,
  "suhu": 27.6,
  "humid": 58.7
}