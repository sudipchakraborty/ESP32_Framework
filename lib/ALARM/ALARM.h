#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>
#include <Wire.h>

#define MAX_ALARMS 10
#define EEPROM_I2C_ADDR 0x50   // AT24C512 default

// Days bitmask
// bit0 = Sunday ... bit6 = Saturday
typedef struct
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} AlarmTime;

typedef struct
{
    uint8_t id;
    char name[20];

    AlarmTime onTime;
    AlarmTime offTime;

    uint8_t repeatMask;   // bitmask for days
    bool enabled;

} Alarm_t;

class AlarmManager
{
private:
    Alarm_t alarms[MAX_ALARMS];
    uint8_t alarmCount;

    void writeEEPROM(uint16_t addr, uint8_t *data, uint16_t len);
    void readEEPROM(uint16_t addr, uint8_t *data, uint16_t len);

    int getDayOfWeek(int d, int m, int y); // 0=Sunday

public:
    AlarmManager();
    String getAlarmListString();
    void printAlarms(Stream &out);
    void begin();
    bool isWithinActiveWindow(Alarm_t &alarm, String dt);
    bool isWithinActiveWindow(Alarm_t &alarm, int hh, int mm, int ss);

    bool addOrUpdateAlarm(Alarm_t alarm);
    bool deleteAlarm(uint8_t id);

    Alarm_t* getAlarmById(uint8_t id);
    Alarm_t* checkAlarmMatch(String currentDateTime);

    void saveToEEPROM();
    void loadFromEEPROM();

    uint8_t getCount();
};

#endif