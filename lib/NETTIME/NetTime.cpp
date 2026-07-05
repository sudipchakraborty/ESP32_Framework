#include "NetTime.h"
#include <WiFi.h>
#include "time.h"

static bool timeInitialized = false;

bool NetTime::begin(long gmtOffset_sec, int daylightOffset_sec)
{
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");

    struct tm timeinfo;
    int retry = 0;

    while (!getLocalTime(&timeinfo) && retry < 10) {
        delay(500);
        retry++;
    }

    timeInitialized = (retry < 10);
    return timeInitialized;
}

bool NetTime::isTimeValid()
{
    struct tm timeinfo;
    return getLocalTime(&timeinfo);
}

String NetTime::getDateTimeString()
{
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) {
        return String(); // return empty if failed
    }

    char buffer[25];

    snprintf(buffer, sizeof(buffer),
             "%02d-%02d-%04d %02d:%02d:%02d",
             timeinfo.tm_mday,
             timeinfo.tm_mon + 1,
             timeinfo.tm_year + 1900,
             timeinfo.tm_hour,
             timeinfo.tm_min,
             timeinfo.tm_sec);

    return String(buffer);
}