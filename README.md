# ESP32 using ESP-IDF from the coommand line.

*The starting point for this project is the `blink` example and the original README can be found at <https://github.com/espressif/esp-idf/blob/master/examples/get-started/blink/README.md>. Everything in this README is mine.

## Motivation

Using the ESP-IDF SDK provided by Espressif is the most direct way to leverage the packages they provide. In particular the DS18B20 drivers were near trivial with ESP-IDF and ESP-IDF on PlatformIO took me down rabbit holes. The ESP-IDF plugin for VS Code didn't configure because of decisions Debian has made about installing Python packages using `pip` (and which I agree with.) I do use VS Code for editing files and build, flash and monitor using CLI tools.

## Status

* 2025-05-05 Blue flashing LED.

## Plans

* Add WiFi
* Add NTP
* Add MQTT publisher

## 2025-05-05 Setup

* <https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html> Instructions for installing the tool chain and SDK. (I'm installing on Linux - Debian Bookworm.) With everything installed, I start the terminal session and copy the `blink` project files to this directory by:

```text
. ~/esp/esp-idf/export.sh
# see the original README for suggested targets or run "idf.py set-target"
idf.py set-target esp32

# set GPIO for LED in "examples" to '2' - the builtin blue LED.
idf.py menuconfig 

idf.py build
idf.py flash monitor
```

Great success! (Blue flashing LED on my NodeMCU.)

Result looks like

```text
hbarta@olive:~/Programming/ESP32/ESP32-ESP-IDF-CLI-start$ idf.py monitor
Executing action: monitor
Serial port /dev/ttyUSB0
Connecting.....
Detecting chip type... Unsupported detection protocol, switching and trying again...
Connecting..........
Detecting chip type... ESP32
Running idf_monitor in directory /home/hbarta/Programming/ESP32/ESP32-ESP-IDF-CLI-start
Executing "/home/hbarta/.espressif/python_env/idf5.4_py3.11_env/bin/python /home/hbarta/esp/esp-idf/tools/idf_monitor.py -p /dev/ttyUSB0 -b 115200 --toolchain-prefix xtensa-esp32-elf- --target esp32 --revision 0 /home/hbarta/Programming/ESP32/ESP32-ESP-IDF-CLI-start/build/blink.elf -m '/home/hbarta/.espressif/python_env/idf5.4_py3.11_env/bin/python' '/home/hbarta/esp/esp-idf/tools/idf.py'"...
--- esp-idf-monitor 1.6.2 on /dev/ttyUSB0 115200
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
ets Jun  8 2016 00:22:57

rst:0x1 (POWERON_RESET),boot:0x17 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0030,len:6276
load:0x40078000,len:15748
load:0x40080400,len:4
--- 0x40080400: _init at ??:?

ho 8 tail 4 room 4
load:0x40080404,len:3860
entry 0x4008063c
I (31) boot: ESP-IDF v5.4.1 2nd stage bootloader
I (31) boot: compile time May  5 2025 16:13:50
I (31) boot: Multicore bootloader
I (32) boot: chip revision: v1.0
I (35) boot.esp32: SPI Speed      : 40MHz
I (39) boot.esp32: SPI Mode       : DIO
I (42) boot.esp32: SPI Flash Size : 2MB
I (46) boot: Enabling RNG early entropy source...
I (50) boot: Partition Table:
I (53) boot: ## Label            Usage          Type ST Offset   Length
I (59) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (66) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (72) boot:  2 factory          factory app      00 00 00010000 00100000
I (79) boot: End of partition table
I (82) esp_image: segment 0: paddr=00010020 vaddr=3f400020 size=0a03ch ( 41020) map
I (104) esp_image: segment 1: paddr=0001a064 vaddr=3ff80000 size=0001ch (    28) load
I (104) esp_image: segment 2: paddr=0001a088 vaddr=3ffb0000 size=02314h (  8980) load
I (111) esp_image: segment 3: paddr=0001c3a4 vaddr=40080000 size=03c74h ( 15476) load
I (121) esp_image: segment 4: paddr=00020020 vaddr=400d0020 size=1460ch ( 83468) map
I (151) esp_image: segment 5: paddr=00034634 vaddr=40083c74 size=08fc0h ( 36800) load
I (172) boot: Loaded app from partition at offset 0x10000
I (172) boot: Disabling RNG early entropy source...
I (182) cpu_start: Multicore app
I (191) cpu_start: Pro cpu start user code
I (191) cpu_start: cpu freq: 160000000 Hz
I (191) app_init: Application information:
I (191) app_init: Project name:     blink
I (194) app_init: App version:      1
I (198) app_init: Compile time:     May  5 2025 16:14:32
I (203) app_init: ELF file SHA256:  147a9dc35...
I (207) app_init: ESP-IDF:          v5.4.1
I (211) efuse_init: Min chip rev:     v0.0
I (215) efuse_init: Max chip rev:     v3.99 
I (219) efuse_init: Chip rev:         v1.0
I (223) heap_init: Initializing. RAM available for dynamic allocation:
I (229) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (234) heap_init: At 3FFB2BD8 len 0002D428 (181 KiB): DRAM
I (239) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (245) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (250) heap_init: At 4008CC34 len 000133CC (76 KiB): IRAM
I (257) spi_flash: detected chip: generic
I (259) spi_flash: flash io: dio
W (262) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (275) main_task: Started on CPU0
I (285) main_task: Calling app_main()
I (285) example: Example configured to blink GPIO LED!
I (285) gpio: GPIO[2]| InputEn: 0| OutputEn: 0| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:0 
I (285) example: Turning the LED OFF!
I (1295) example: Turning the LED ON!
I (2295) example: Turning the LED OFF!
I (3295) example: Turning the LED ON!
I (4295) example: Turning the LED OFF!
I (5295) example: Turning the LED ON!
I (6295) example: Turning the LED OFF!
I (7295) example: Turning the LED ON!
```

## 2025-05-05 add WiFi

Copy WiFi files from <https://github.com/HankB/ESP32-ESP-IDF-PlatformIO-start>.

