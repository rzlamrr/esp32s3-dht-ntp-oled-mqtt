#ifndef MQTT_H
#define MQTT_H

void init_mqtt(void);
int proj_mqtt_publish(const char* topic, const char* payload, int len, int qos, int retain);

#endif // MQTT_H