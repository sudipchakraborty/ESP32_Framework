#ifndef DEVICE_ID_H
#define DEVICE_ID_H

#include <Arduino.h>

class DeviceID
{
public:
    DeviceID();

    void begin();

    // Core IDs
    uint64_t getEfuseChipID();          // Best unique ID
    String getEfuseChipIDString();

    String getWiFiMAC();
    String getBTMAC();
    String getBaseMAC();

    uint32_t getFlashChipID();

    // Derived IDs
    String getMD5UID();
    String getSHA256UID();

    // Combined Device ID (recommended)
    String getDeviceID();
    void printDeviceInfo(Stream &out);
    uint32_t getFlashChipSize();
    uint32_t getFlashChipJEDECID();

private:
    uint64_t chipid;

    String macToString(uint8_t mac[6]);
};

#endif