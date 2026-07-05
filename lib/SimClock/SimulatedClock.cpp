#include "SimulatedClock.h"

SimulatedClock::SimulatedClock()
{   
}
//__________________________________________________________________________________________
void SimulatedClock::begin()
{
}
//__________________________________________________________________________________________
void SimulatedClock::set(String strTime)
{
    // Expected format:
    // "07-05-2026 11:19:11"

    dd  = strTime.substring(0, 2).toInt();
    mon = strTime.substring(3, 5).toInt();
    yy  = strTime.substring(6, 10).toInt();

    hh  = strTime.substring(11, 13).toInt();
    min = strTime.substring(14, 16).toInt();
    ss  = strTime.substring(17, 19).toInt();
}
//__________________________________________________________________________________________
void SimulatedClock::update(uint8_t val)
{
    // Increment seconds
    ss += val;

    // Handle seconds overflow
    while (ss >= 60)
    {
        ss -= 60;
        min++;
    }

    // Handle minutes overflow
    while (min >= 60)
    {
        min -= 60;
        hh++;
    }

    // Handle hours overflow
    while (hh >= 24)
    {
        hh -= 24;
        dd++;
    }

    // Days in month
    uint8_t daysInMonth;

    switch(mon)
    {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:
            daysInMonth = 31;
            break;

        case 4: case 6: case 9: case 11:
            daysInMonth = 30;
            break;

        case 2:
            // Leap year check
            if ((yy % 4 == 0 && yy % 100 != 0) || (yy % 400 == 0))
                daysInMonth = 29;
            else
                daysInMonth = 28;
            break;

        default:
            daysInMonth = 30;
            break;
    }

    // Handle day overflow
    while (dd > daysInMonth)
    {
        dd = 1;
        mon++;

        // Month overflow
        if (mon > 12)
        {
            mon = 1;
            yy++;
        }

        // Recalculate days in new month
        switch(mon)
        {
            case 1: case 3: case 5: case 7:
            case 8: case 10: case 12:
                daysInMonth = 31;
                break;

            case 4: case 6: case 9: case 11:
                daysInMonth = 30;
                break;

            case 2:
                if ((yy % 4 == 0 && yy % 100 != 0) || (yy % 400 == 0))
                    daysInMonth = 29;
                else
                    daysInMonth = 28;
                break;
        }
    }
}
//__________________________________________________________________________________________
void SimulatedClock::update()
{
    // Increment seconds
    ss += 1;

    // Handle seconds overflow
    while (ss >= 60)
    {
        ss -= 60;
        min++;
    }

    // Handle minutes overflow
    while (min >= 60)
    {
        min -= 60;
        hh++;
    }

    // Handle hours overflow
    while (hh >= 24)
    {
        hh -= 24;
        dd++;
    }

    // Days in month
    uint8_t daysInMonth;

    switch(mon)
    {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:
            daysInMonth = 31;
            break;

        case 4: case 6: case 9: case 11:
            daysInMonth = 30;
            break;

        case 2:
            // Leap year check
            if ((yy % 4 == 0 && yy % 100 != 0) || (yy % 400 == 0))
                daysInMonth = 29;
            else
                daysInMonth = 28;
            break;

        default:
            daysInMonth = 30;
            break;
    }

    // Handle day overflow
    while (dd > daysInMonth)
    {
        dd = 1;
        mon++;

        // Month overflow
        if (mon > 12)
        {
            mon = 1;
            yy++;
        }

        // Recalculate days in new month
        switch(mon)
        {
            case 1: case 3: case 5: case 7:
            case 8: case 10: case 12:
                daysInMonth = 31;
                break;

            case 4: case 6: case 9: case 11:
                daysInMonth = 30;
                break;

            case 2:
                if ((yy % 4 == 0 && yy % 100 != 0) || (yy % 400 == 0))
                    daysInMonth = 29;
                else
                    daysInMonth = 28;
                break;
        }
    }
}
//__________________________________________________________________________________________
void SimulatedClock::updateMin(uint8_t val)
{
    // Increment seconds
    ss = 0;   
    min+=val;
     
    // Handle minutes overflow
    while (min >= 60)
    {
        min -= 60;
        hh++;
    }

    // Handle hours overflow
    while (hh >= 24)
    {
        hh -= 24;
        dd++;
    }

    // Days in month
    uint8_t daysInMonth;

    switch(mon)
    {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:
            daysInMonth = 31;
            break;

        case 4: case 6: case 9: case 11:
            daysInMonth = 30;
            break;

        case 2:
            // Leap year check
            if ((yy % 4 == 0 && yy % 100 != 0) || (yy % 400 == 0))
                daysInMonth = 29;
            else
                daysInMonth = 28;
            break;

        default:
            daysInMonth = 30;
            break;
    }

    // Handle day overflow
    while (dd > daysInMonth)
    {
        dd = 1;
        mon++;

        // Month overflow
        if (mon > 12)
        {
            mon = 1;
            yy++;
        }

        // Recalculate days in new month
        switch(mon)
        {
            case 1: case 3: case 5: case 7:
            case 8: case 10: case 12:
                daysInMonth = 31;
                break;

            case 4: case 6: case 9: case 11:
                daysInMonth = 30;
                break;

            case 2:
                if ((yy % 4 == 0 && yy % 100 != 0) || (yy % 400 == 0))
                    daysInMonth = 29;
                else
                    daysInMonth = 28;
                break;
        }
    }
}
//__________________________________________________________________________________________
String SimulatedClock::readDT_As_ddmmyyyyhhmmss()
{
    char buf[25];
    sprintf(buf,
            "%02d-%02d-%04d %02d:%02d:%02d",
            dd,
            mon,
            yy,
            hh,
            min,
            ss);
    return String(buf);
}
//__________________________________________________________________________________________


// uint32_t SimulatedClock::getEpoch()
// {
//     uint32_t elapsedMs = millis() - startMillis;
//     uint32_t simulatedSec = (elapsedMs / 1000.0) * speed;

//     return baseEpoch + simulatedSec;
// }

// String SimulatedClock::getDateTimeString()
// {
//     uint32_t t = getEpoch();

//     uint32_t sec = t % 60;
//     t /= 60;
//     uint32_t min = t % 60;
//     t /= 60;
//     uint32_t hour = t % 24;
//     uint32_t days = t / 24;

//     // Simple date calc (not full calendar accurate, but OK for simulation)
//     uint32_t year = 2026;
//     uint32_t month = 1;
//     uint32_t day = 1 + days;

//     char buffer[25];
//     sprintf(buffer, "%02lu-%02lu-%04lu %02lu:%02lu:%02lu",
//             day, month, year, hour, min, sec);

//     return String(buffer);
// }