#include "MQTTManager.h"
//////////////////////////////////////////////////
MQTTManager::MQTTManager(const char* ssid,
                         const char* password,
                         const char* mqtt_server,
                         int mqtt_port,
                         debug* debugObj)
    : _ssid(ssid),
      _password(password),
      _mqtt_server(mqtt_server),
      _mqtt_port(mqtt_port),
      client(espClient)
{
    dbg = debugObj;   // store pointer
    lastReconnectAttempt = 0;
}
//__________________________________________________________________________________________________________
void MQTTManager::begin() 
{
    connectWiFi();
    client.setServer(_mqtt_server, _mqtt_port);
    client.setKeepAlive(30);
    client.setSocketTimeout(5);
}
//__________________________________________________________________________________________________________
void MQTTManager::connectWiFi() {
    dbg->println("Connecting to WiFi...");
    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        dbg->println(".");
    }

    dbg->println("\nWiFi connected");
}
//__________________________________________________________________________________________________________
bool MQTTManager::reconnectMQTT()
{
    WiFiClient testClient;

    dbg->println("Testing TCP...");

    if (!testClient.connect(_mqtt_server, _mqtt_port))
    {
        dbg->println("TCP FAILED");
        return false;
    }

    dbg->println("TCP SUCCESS");
    testClient.stop();

    delay(100);
    yield();

    dbg->println("Trying MQTT...");

    String clientId =
        "ESP32_" + String((uint32_t)ESP.getEfuseMac(), HEX);

    client.setKeepAlive(30);
    client.setSocketTimeout(5);

    bool ok = client.connect(
        clientId.c_str(),
        "esp32",
        "12345678"
    );

    if (ok)
    {
        dbg->println("MQTT connected");
        // client.subscribe("test/topic");
        client.subscribe("esp32/request");
        return true;
    }
    else
    {
        dbg->print("MQTT failed rc=", String(client.state()));
        return false;
    }
}
//__________________________________________________________________________________________________________
void MQTTManager::loop() 
{
    if (!client.connected()) {
        unsigned long now = millis();
        if (now - lastReconnectAttempt > 3000) {
            lastReconnectAttempt = now;
            if (reconnectMQTT()) {
                lastReconnectAttempt = 0;
            }
        }
    } else {
        client.loop();
    }
}
//__________________________________________________________________________________________________________
bool MQTTManager::publish(const char* topic, const char* message) {
    return client.publish(topic, message);
}
//__________________________________________________________________________________________________________
void MQTTManager::subscribe(const char* topic) {
    client.subscribe(topic);
}
//__________________________________________________________________________________________________________
void MQTTManager::setCallback(void (*callback)(char*, byte*, unsigned int)) {
    client.setCallback(callback);
}
//__________________________________________________________________________________________________________
