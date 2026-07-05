#include "DeviceID.h"
#include <WiFi.h>
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_flash.h"

#include "mbedtls/md5.h"
#include "mbedtls/sha256.h"

// ---------------- Constructor ----------------
DeviceID::DeviceID()
{
    chipid = 0;
}

// ---------------- Init ----------------
void DeviceID::begin()
{
    chipid = ESP.getEfuseMac();
}

// ---------------- Print Info ----------------
void DeviceID::printDeviceInfo(Stream &out)
{
    out.println("==== DEVICE ID INFO ====");

    out.print("eFuse Chip ID: ");
    out.println(getEfuseChipIDString());

    out.print("Base MAC: ");
    out.println(getBaseMAC());

    out.print("WiFi MAC: ");
    out.println(getWiFiMAC());

    out.print("BT MAC: ");
    out.println(getBTMAC());

    out.print("Flash Size (bytes): ");
    out.println(getFlashChipSize());

    out.print("Flash JEDEC ID: ");
    out.println(getFlashChipJEDECID(), HEX);

    out.print("MD5 UID: ");
    out.println(getMD5UID());

    out.print("SHA256 UID: ");
    out.println(getSHA256UID());

    out.print("Final Device ID: ");
    out.println(getDeviceID());

    out.println("========================");
}

// ---------------- eFuse Chip ID ----------------
uint64_t DeviceID::getEfuseChipID()
{
    return chipid;
}

String DeviceID::getEfuseChipIDString()
{
    char buf[20];
    snprintf(buf, sizeof(buf), "%04X%08X",
             (uint16_t)(chipid >> 32),
             (uint32_t)chipid);
    return String(buf);
}

// ---------------- MAC Helpers ----------------
String DeviceID::macToString(uint8_t mac[6])
{
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2],
             mac[3], mac[4], mac[5]);
    return String(buf);
}

// ---------------- Base MAC ----------------
String DeviceID::getBaseMAC()
{
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    return macToString(mac);
}

// ---------------- WiFi MAC ----------------
String DeviceID::getWiFiMAC()
{
    return WiFi.macAddress();
}

// ---------------- BT MAC ----------------
String DeviceID::getBTMAC()
{
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_BT);
    return macToString(mac);
}

// ---------------- Flash Size ----------------
uint32_t DeviceID::getFlashChipSize()
{
    return ESP.getFlashChipSize();  // e.g., 4194304 (4MB)
}

// ---------------- Flash JEDEC ID ----------------
uint32_t DeviceID::getFlashChipJEDECID()
{
    uint32_t id = 0;
    esp_flash_read_id(NULL, &id);
    return id;
}

// ---------------- MD5 UID ----------------
String DeviceID::getMD5UID()
{
    char input[32];
    snprintf(input, sizeof(input), "%llu", chipid);

    unsigned char result[16];
    mbedtls_md5((unsigned char*)input, strlen(input), result);

    char output[33];
    for (int i = 0; i < 16; i++)
        sprintf(output + i * 2, "%02x", result[i]);

    return String(output);
}

// ---------------- SHA256 UID ----------------
String DeviceID::getSHA256UID()
{
    char input[32];
    snprintf(input, sizeof(input), "%llu", chipid);

    unsigned char result[32];
    mbedtls_sha256((unsigned char*)input, strlen(input), result, 0);

    char output[65];
    for (int i = 0; i < 32; i++)
        sprintf(output + i * 2, "%02x", result[i]);

    return String(output);
}

// ---------------- Recommended Device ID ----------------
String DeviceID::getDeviceID()
{
    return getSHA256UID();
}