#ifndef MQTT_H
#define MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>

class MQTTModule
{
private:
    WiFiClient espClient;
    PubSubClient client;

    const char* ssid;
    const char* password;
    const char* mqttServer;
    int mqttPort;

    String subTopic;
    String pubTopic;

    String rxBuffer;
    bool newData;

    static MQTTModule* instance;

    static void callback(char* topic, byte* payload, unsigned int length);

    void connectWiFi();
    void reconnect();

public:
    MQTTModule(const char* ssid,
               const char* password,
               const char* mqttServer,
               int mqttPort);

    void begin(String subscribeTopic, String publishTopic);
    void loop();

    bool available();
    String read();

    void send(String data);
};

#endif