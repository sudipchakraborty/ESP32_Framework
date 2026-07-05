#include "MQTT.h"

MQTTModule* MQTTModule::instance = nullptr;

MQTTModule::MQTTModule(const char* ssid,
                       const char* password,
                       const char* mqttServer,
                       int mqttPort)
    : client(espClient)
{
    this->ssid = ssid;
    this->password = password;
    this->mqttServer = mqttServer;
    this->mqttPort = mqttPort;

    newData = false;
    instance = this;
}

void MQTTModule::begin(String subscribeTopic, String publishTopic)
{
    subTopic = subscribeTopic;
    pubTopic = publishTopic;

    connectWiFi();

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
}

void MQTTModule::connectWiFi()
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
}

void MQTTModule::reconnect()
{
    while (!client.connected())
    {
        String clientId = "ESP32-" + String(random(0xffff), HEX);

        if (client.connect(clientId.c_str()))
        {
            client.subscribe(subTopic.c_str());
        }
        else
        {
            delay(2000);
        }
    }
}

void MQTTModule::loop()
{
    if (!client.connected())
    {
        reconnect();
    }

    client.loop();
}

void MQTTModule::callback(char* topic, byte* payload, unsigned int length)
{
    if (instance == nullptr) return;

    instance->rxBuffer = "";

    for (unsigned int i = 0; i < length; i++)
    {
        instance->rxBuffer += (char)payload[i];
    }

    instance->newData = true;
}

bool MQTTModule::available()
{
    return newData;
}

String MQTTModule::read()
{
    newData = false;
    return rxBuffer;
}

void MQTTModule::send(String data)
{
    client.publish(pubTopic.c_str(), data.c_str());
}