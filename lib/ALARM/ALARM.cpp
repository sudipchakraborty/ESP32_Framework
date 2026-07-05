#include "ALARM.h"

// ---------------- Constructor ----------------
AlarmManager::AlarmManager()
{
    alarmCount = 0;
}
//_____________________________________________________________________________

// ---------------- Init ----------------
void AlarmManager::begin()
{
    Wire.begin();
    loadFromEEPROM();
}
//_____________________________________________________________________________
String AlarmManager::getAlarmListString()
{
    String output = "";

    output += "Total Alarms: " + String(alarmCount) + "\n";

    for (int i = 0; i < alarmCount; i++)
    {
        Alarm_t &a = alarms[i];

        output += "----------------------\n";
        output += "ID: " + String(a.id) + "\n";
        output += "Name: " + String(a.name) + "\n";

        char timeStr[20];

        // ON Time
        sprintf(timeStr, "%02d:%02d:%02d",
                a.onTime.hour,
                a.onTime.minute,
                a.onTime.second);
        output += "ON Time: " + String(timeStr) + "\n";

        // OFF Time
        sprintf(timeStr, "%02d:%02d:%02d",
                a.offTime.hour,
                a.offTime.minute,
                a.offTime.second);
        output += "OFF Time: " + String(timeStr) + "\n";

        // Enabled
        output += "Enabled: " + String(a.enabled ? "YES" : "NO") + "\n";

        // Repeat Days Decode
        String days = "";

        if (a.repeatMask & (1 << 0)) days += "Sun ";
        if (a.repeatMask & (1 << 1)) days += "Mon ";
        if (a.repeatMask & (1 << 2)) days += "Tue ";
        if (a.repeatMask & (1 << 3)) days += "Wed ";
        if (a.repeatMask & (1 << 4)) days += "Thu ";
        if (a.repeatMask & (1 << 5)) days += "Fri ";
        if (a.repeatMask & (1 << 6)) days += "Sat ";

        output += "Repeat: " + days + "\n";
    }

    output += "----------------------\n";

    return output;
}
//_____________________________________________________________________________
void AlarmManager::printAlarms(Stream &out)
{
    out.printf("Total Alarms: %d\r\n", alarmCount);

    for (int i = 0; i < alarmCount; i++)
    {
        Alarm_t &a = alarms[i];

        out.printf("----------------------\r\n");
        out.printf("ID: %d\r\n", a.id);
        out.printf("Name: %s\r\n", a.name);

        out.printf("ON Time: %02d:%02d:%02d\r\n",
                   a.onTime.hour, a.onTime.minute, a.onTime.second);

        out.printf("OFF Time: %02d:%02d:%02d\r\n",
                   a.offTime.hour, a.offTime.minute, a.offTime.second);

        out.printf("Enabled: %s\r\n", a.enabled ? "YES" : "NO");

        out.print("Repeat: ");

        if (a.repeatMask & (1 << 0)) out.print("Sun ");
        if (a.repeatMask & (1 << 1)) out.print("Mon ");
        if (a.repeatMask & (1 << 2)) out.print("Tue ");
        if (a.repeatMask & (1 << 3)) out.print("Wed ");
        if (a.repeatMask & (1 << 4)) out.print("Thu ");
        if (a.repeatMask & (1 << 5)) out.print("Fri ");
        if (a.repeatMask & (1 << 6)) out.print("Sat ");

        out.print("\r\n");
    }

    out.printf("----------------------\r\n");
}
//_____________________________________________________________________________

// ---------------- EEPROM Write ----------------
void AlarmManager::writeEEPROM(uint16_t addr, uint8_t *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        Wire.beginTransmission(EEPROM_I2C_ADDR);
        Wire.write((addr + i) >> 8);
        Wire.write((addr + i) & 0xFF);
        Wire.write(data[i]);
        Wire.endTransmission();
        delay(5); // EEPROM write delay
    }
}
//_____________________________________________________________________________

// ---------------- EEPROM Read ----------------
void AlarmManager::readEEPROM(uint16_t addr, uint8_t *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        Wire.beginTransmission(EEPROM_I2C_ADDR);
        Wire.write((addr + i) >> 8);
        Wire.write((addr + i) & 0xFF);
        Wire.endTransmission();

        Wire.requestFrom(EEPROM_I2C_ADDR, 1);
        if (Wire.available())
            data[i] = Wire.read();
    }
}
//_____________________________________________________________________________

// ---------------- Add / Update ----------------
bool AlarmManager::addOrUpdateAlarm(Alarm_t alarm)
{
    for (int i = 0; i < alarmCount; i++)
    {
        if (alarms[i].id == alarm.id)
        {
            alarms[i] = alarm;
            return true;
        }
    }

    if (alarmCount < MAX_ALARMS)
    {
        alarms[alarmCount++] = alarm;
        return true;
    }

    return false;
}
//_____________________________________________________________________________

// ---------------- Delete ----------------
bool AlarmManager::deleteAlarm(uint8_t id)
{
    for (int i = 0; i < alarmCount; i++)
    {
        if (alarms[i].id == id)
        {
            for (int j = i; j < alarmCount - 1; j++)
                alarms[j] = alarms[j + 1];

            alarmCount--;
            return true;
        }
    }
    return false;
}
//_____________________________________________________________________________

// ---------------- Get Alarm ----------------
Alarm_t* AlarmManager::getAlarmById(uint8_t id)
{
    for (int i = 0; i < alarmCount; i++)
    {
        if (alarms[i].id == id)
            return &alarms[i];
    }
    return nullptr;
}
//_____________________________________________________________________________

// ---------------- Day of Week (Zeller’s Congruence) ----------------
int AlarmManager::getDayOfWeek(int d, int m, int y)
{
    if (m < 3)
    {
        m += 12;
        y--;
    }

    int k = y % 100;
    int j = y / 100;

    int f = d + 13*(m+1)/5 + k + k/4 + j/4 + 5*j;
    return ((f + 6) % 7); // 0=Sunday
}
//_____________________________________________________________________________

// ---------------- Match Alarm ----------------
Alarm_t* AlarmManager::checkAlarmMatch(String dt)
{
    int d, m, y, hh, mm, ss;

    sscanf(dt.c_str(), "%d-%d-%d %d:%d:%d",
           &d, &m, &y, &hh, &mm, &ss);

    int dow = getDayOfWeek(d, m, y);

    for (int i = 0; i < alarmCount; i++)
    {
        if (!alarms[i].enabled) continue;

        if (!(alarms[i].repeatMask & (1 << dow)))
            continue;

        if (alarms[i].onTime.hour == hh &&
            alarms[i].onTime.minute == mm &&
            alarms[i].onTime.second == ss)
        {
            return &alarms[i];
        }
    }

    return nullptr;
}
//_____________________________________________________________________________
bool AlarmManager::isWithinActiveWindow(Alarm_t &alarm, String dt)
{
    if (!alarm.enabled) return false;

    int d, m, y, hh, mm, ss;

    // Parse input datetime
    if (sscanf(dt.c_str(), "%d-%d-%d %d:%d:%d",
               &d, &m, &y, &hh, &mm, &ss) != 6)
    {
        return false; // invalid format
    }

    // Check day-of-week
    int dow = getDayOfWeek(d, m, y);  // 0 = Sunday

    if (!(alarm.repeatMask & (1 << dow)))
        return false;

    // Convert to seconds
    int now = hh * 3600 + mm * 60 + ss;
    int on  = alarm.onTime.hour * 3600 + alarm.onTime.minute * 60 + alarm.onTime.second;
    int off = alarm.offTime.hour * 3600 + alarm.offTime.minute * 60 + alarm.offTime.second;

    // Case 1: Same-day window
    if (on < off)
    {
        return (now >= on && now < off);
    }
    // Case 2: Cross-midnight window
    else
    {
        return (now >= on || now < off);
    }
}
//_____________________________________________________________________________
bool AlarmManager::isWithinActiveWindow(Alarm_t &alarm, int hh, int mm, int ss)
{
    if (!alarm.enabled) return false;

    int now = hh * 3600 + mm * 60 + ss;
    int on  = alarm.onTime.hour * 3600 + alarm.onTime.minute * 60 + alarm.onTime.second;
    int off = alarm.offTime.hour * 3600 + alarm.offTime.minute * 60 + alarm.offTime.second;

    // Case 1: Normal (same day) → ON < OFF
    if (on < off)
    {
        return (now >= on && now < off);
    }
    // Case 2: Cross midnight → ON > OFF (e.g. 22:00 → 06:00)
    else
    {
        return (now >= on || now < off);
    }
}
//_____________________________________________________________________________
// ---------------- Save ----------------
void AlarmManager::saveToEEPROM()
{
    uint16_t addr = 0;

    writeEEPROM(addr, &alarmCount, 1);
    addr += 1;

    for (int i = 0; i < alarmCount; i++)
    {
        writeEEPROM(addr, (uint8_t*)&alarms[i], sizeof(Alarm_t));
        addr += sizeof(Alarm_t);
    }
}
//_____________________________________________________________________________

// ---------------- Load ----------------
void AlarmManager::loadFromEEPROM()
{
    uint16_t addr = 0;

    readEEPROM(addr, &alarmCount, 1);
    addr += 1;

    if (alarmCount > MAX_ALARMS)
        alarmCount = 0;

    for (int i = 0; i < alarmCount; i++)
    {
        readEEPROM(addr, (uint8_t*)&alarms[i], sizeof(Alarm_t));
        addr += sizeof(Alarm_t);
    }
}
//_____________________________________________________________________________

// ---------------- Count ----------------
uint8_t AlarmManager::getCount()
{
    return alarmCount;
}
//_____________________________________________________________________________
