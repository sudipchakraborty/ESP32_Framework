#ifndef SIMULATED_CLOCK_H
#define SIMULATED_CLOCK_H

#include <Arduino.h>

class SimulatedClock
{
private:
    uint8_t hh, min, ss, dd, mon;
    uint16_t yy;

    // uint32_t baseEpoch;        // start time in seconds
    // uint32_t startMillis;      // when simulation started
    // float speed;               // speed factor

public:
    SimulatedClock();
    void begin();
    void set(String strTime);
    void update(uint8_t val);
    void update();
    String readDT_As_ddmmyyyyhhmmss();
    void updateMin(uint8_t val);

    // void setSpeed(float speedFactor);
    // uint32_t getEpoch();
    // String getDateTimeString();
};

#endif