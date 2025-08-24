#ifndef PROJ_DHT_H
#define PROJ_DHT_H

// Fungsi untuk inisialisasi sensor DHT22
void init_dht(void);

// Fungsi untuk membaca suhu
float get_temperature(void);

// Fungsi untuk membaca kelembaban
float get_humidity(void);

#endif // PROJ_DHT_H
