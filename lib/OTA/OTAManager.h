#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include <Arduino.h>

class OTAManager
{
public:
    OTAManager();

    void begin(const String& currentVersion);

    void setURLs(const String& versionURL, const String& firmwareURL);

    void setRootCA(const char* cert); // optional for TLS

    void setDebugStream(Stream &out);

    void setAutoReboot(bool enable);

    bool checkAndUpdate();

    String getCurrentVersion();
    String getAvailableVersion();

private:
    String currentVersion;
    String availableVersion;

    String versionURL;
    String firmwareURL;

    const char* rootCA = nullptr;

    Stream* debug = nullptr;

    bool autoReboot = true;

    bool fetchVersion();
    bool performOTA();

    void log(const String& msg);
};

#endif