#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "DEBUG.h"

class MQTTManager {
public:
    MQTTManager(const char* ssid, const char* password,
                const char* mqtt_server, int mqtt_port,debug* debugObj);

    void begin();
    void loop();

    bool publish(const char* topic, const char* message);
    void subscribe(const char* topic);

    void setCallback(void (*callback)(char*, byte*, unsigned int));

private:
    const char* _ssid;
    const char* _password;
    const char* _mqtt_server;
    int _mqtt_port;

    WiFiClient espClient;
    PubSubClient client;

    unsigned long lastReconnectAttempt;

     debug* dbg;   // <-- add this

    void connectWiFi();
    bool reconnectMQTT();
};

#endif