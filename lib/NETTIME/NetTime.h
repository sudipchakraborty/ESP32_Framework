#ifndef NET_TIME_H
#define NET_TIME_H

#include <Arduino.h>

class NetTime {
public:
    bool begin(long gmtOffset_sec = 19800, int daylightOffset_sec = 0);

    // Returns "dd-mm-yyyy hh:mm:ss"
    String getDateTimeString();

    // Check if time is valid
    bool isTimeValid();
};

#endif