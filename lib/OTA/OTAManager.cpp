#include "OTAManager.h"

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>

// ---------------- Constructor ----------------
OTAManager::OTAManager() {}

// ---------------- Init ----------------
void OTAManager::begin(const String& version)
{
    currentVersion = version;
}

// ---------------- Set URLs ----------------
void OTAManager::setURLs(const String& vURL, const String& fURL)
{
    versionURL = vURL;
    firmwareURL = fURL;
}

// ---------------- Root CA ----------------
void OTAManager::setRootCA(const char* cert)
{
    rootCA = cert;
}

// ---------------- Debug ----------------
void OTAManager::setDebugStream(Stream &out)
{
    debug = &out;
}

// ---------------- Auto Reboot ----------------
void OTAManager::setAutoReboot(bool enable)
{
    autoReboot = enable;
}

// ---------------- Logging ----------------
void OTAManager::log(const String& msg)
{
    if (debug)
    {
        debug->println("[OTA] " + msg);
    }
}

// ---------------- Get Versions ----------------
String OTAManager::getCurrentVersion()
{
    return currentVersion;
}

String OTAManager::getAvailableVersion()
{
    return availableVersion;
}

// ---------------- Check & Update ----------------
bool OTAManager::checkAndUpdate()
{
    if (!fetchVersion())
        return false;

    if (availableVersion == currentVersion)
    {
        log("No update required");
        return false;
    }

    log("New version available: " + availableVersion);

    return performOTA();
}

// ---------------- Fetch Version ----------------
bool OTAManager::fetchVersion()
{
    WiFiClientSecure client;

    if (rootCA)
        client.setCACert(rootCA);
    else
        client.setInsecure(); // fallback (not recommended)

    HTTPClient http;

    if (!http.begin(client, versionURL))
    {
        log("HTTP begin failed");
        return false;
    }

    int code = http.GET();

    if (code != 200)
    {
        log("Version fetch failed");
        http.end();
        return false;
    }

    String payload = http.getString();

    DynamicJsonDocument doc(512);
    deserializeJson(doc, payload);

    availableVersion = doc["version"].as<String>();

    if (doc.containsKey("url"))
        firmwareURL = doc["url"].as<String>();

    http.end();
    return true;
}

// ---------------- Perform OTA ----------------
bool OTAManager::performOTA()
{
    WiFiClientSecure client;

    if (rootCA)
        client.setCACert(rootCA);
    else
        client.setInsecure();

    HTTPClient http;

    if (!http.begin(client, firmwareURL))
    {
        log("Firmware HTTP begin failed");
        return false;
    }

    int code = http.GET();

    if (code != 200)
    {
        log("Firmware download failed");
        http.end();
        return false;
    }

    int contentLength = http.getSize();
    WiFiClient *stream = http.getStreamPtr();

    if (!Update.begin(contentLength))
    {
        log("Not enough space");
        http.end();
        return false;
    }

    log("OTA started");

    size_t written = Update.writeStream(*stream);

    if (written != contentLength)
    {
        log("Write mismatch");
        http.end();
        return false;
    }

    if (!Update.end())
    {
        log("Update failed");
        http.end();
        return false;
    }

    if (Update.isFinished())
    {
        log("OTA success");

        if (autoReboot)
        {
            delay(1000);
            ESP.restart();
        }
    }

    http.end();
    return true;
}