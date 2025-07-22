#ifndef MQTT_H
#define MQTT_H
//Stuff shared between mqtt.c and other C files

//void mqtt_app_start(void);
void init_mqtt(void);
int proj_mqtt_publish(const char* topic, const char* payload, int len, int qos, int retain);

#endif // MQTT_H