#include "Rtc1307.h"
//__________________________________________________________________________________________

Rtc1307::Rtc1307() : _initialized(false) {}
//__________________________________________________________________________________________
/**
 * @brief Initialize I2C and RTC.
 */
bool Rtc1307::begin() {
    Wire.begin();
    _initialized = rtc.begin();
    return _initialized;
}
//__________________________________________________________________________________________
/**
 * @brief Check whether RTC oscillator is running.
 */
bool Rtc1307::isRunning() {
    if (!_initialized) return false;
    return rtc.isrunning();
}
//__________________________________________________________________________________________
/**
 * @brief Read current DateTime from RTC.
 *
 * If RTC not initialized, returns DateTime((uint32_t)0) to avoid ambiguity.
 */
DateTime Rtc1307::now() {
    if (!_initialized) return DateTime((uint32_t)0);
    return rtc.now();
}
//__________________________________________________________________________________________
/**
 * @brief Read time as HH:MM:SS string.
 */
String Rtc1307::readTimeString() {
    if (!_initialized) return String();
    DateTime t = rtc.now();
    char buf[16];
    snprintf(buf, sizeof(buf), "%02u:%02u:%02u", t.hour(), t.minute(), t.second());
    return String(buf);
}
//__________________________________________________________________________________________
/**
 * @brief Read date and time as "YYYY-MM-DD HH:MM:SS".
 */
String Rtc1307::readDateTimeString() {
    if (!_initialized) return String();
    DateTime t = rtc.now();
    char buf[32];
    snprintf(buf, sizeof(buf), "%04u-%02u-%02u %02u:%02u:%02u",
             t.year(), t.month(), t.day(), t.hour(), t.minute(), t.second());
    return String(buf);
}
//__________________________________________________________________________________________
String Rtc1307::readDT_As_ddmmyyyyhhmmss() {
    if (!_initialized) return String();
    DateTime t = rtc.now();
    char buf[32];
    snprintf(buf, sizeof(buf), "%02u-%02u-%04u %02u:%02u:%02u",
             t.day(), t.month(), t.year(),
             t.hour(), t.minute(), t.second());
    return String(buf);
}
//__________________________________________________________________________________________
/**
 * @brief Set RTC using string format "dd-mm-yyyy hh:mm:ss"
 */
bool Rtc1307::setDateTimeFromString(const String &dateTimeStr) {
    if (!_initialized) return false;

    uint16_t year;
    uint8_t month, day, hour, minute, second;

    // Parse string
    int parsed = sscanf(dateTimeStr.c_str(), "%2hhu-%2hhu-%4hu %2hhu:%2hhu:%2hhu",
                        &day, &month, &year,
                        &hour, &minute, &second);

    if (parsed != 6) {
        return false; // invalid format
    }

    DateTime dt(year, month, day, hour, minute, second);
    rtc.adjust(dt);

    return true;
}
//__________________________________________________________________________________________
/**
 * @brief Set RTC using compact string "ddmmyyyyhhmmss"
 */
bool Rtc1307::setDateTimeFromCompactString(const String &str) {
    if (!_initialized) return false;

    if (str.length() != 14) return false;

    uint8_t day     = str.substring(0, 2).toInt();
    uint8_t month   = str.substring(2, 4).toInt();
    uint16_t year   = str.substring(4, 8).toInt();
    uint8_t hour    = str.substring(8,10).toInt();
    uint8_t minute  = str.substring(10,12).toInt();
    uint8_t second  = str.substring(12,14).toInt();

    DateTime dt(year, month, day, hour, minute, second);
    rtc.adjust(dt);

    return true;
}
//_________________________________________________________________________________________
/**
 * @brief Set RTC using DateTime object.
 */
bool Rtc1307::setDateTime(const DateTime &dt) {
    if (!_initialized) return false;
    rtc.adjust(dt);
    return true;
}
//__________________________________________________________________________________________
/**
 * @brief Set RTC using individual components.
 */
bool Rtc1307::setDateTime(uint16_t year, uint8_t month, uint8_t day,
                          uint8_t hour, uint8_t minute, uint8_t second) {
    if (!_initialized) return false;
    DateTime dt(year, month, day, hour, minute, second);
    rtc.adjust(dt);
    return true;
}
//__________________________________________________________________________________________
bool Rtc1307::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    if (!_initialized) return false;

    DateTime now = rtc.now();  // Read current date & time

    // Keep existing date, change only time
    DateTime newTime(
        now.year(),
        now.month(),
        now.day(),
        hour,
        minute,
        second
    );

    rtc.adjust(newTime);

    return true;
}
//__________________________________________________________________________________________
/**
 * @brief If RTC is stopped, set it to compile time.
 */
bool Rtc1307::updateIfStopped() {
    if (!_initialized) return false;
    if (!rtc.isrunning()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    return true;
}
//__________________________________________________________________________________________
