#include <Arduino.h>
#include "DEBUG.h"

///////////////////////////////
debug::debug()
{
    dbgSerial = &Serial;
}
//////////////////////////////////////////////////////
void debug::begin(HardwareSerial &port,double speed)
{
    dbgSerial = &port;
    baud_rate=speed;
    dbgSerial->begin(baud_rate);
}
//////////////////////////////////////////////////////
debug::debug(HardwareSerial &serialPort, double BaudRate)
{
    dbgSerial = &serialPort;
    baud_rate = BaudRate;
}

debug::debug(HardwareSerial &serialPort)
{
    dbgSerial = &serialPort;
}
/////////////////////////////////////////////////////////////
void debug::begin(HardwareSerial &port,
                  uint32_t speed,
                  int8_t txPin,
                  int8_t rxPin)
{
    dbgSerial = &port;
    baud_rate = speed;

    dbgSerial->begin(baud_rate,
                     SERIAL_8N1,
                     rxPin,
                     txPin);
}
////////////////////////////////////////
void debug::DefaultSetUp()
{
    dbgSerial = &Serial1;
    dbgSerial->begin(115200, SERIAL_8N1, -1, 4);
}
/////////////////////////////////////////////////////////////

void debug::begin(void)
{
    dbgSerial->begin(baud_rate);
}

/////////////////////////////////////////////////////////////

void debug::print(const char *msg)
{
    if(enable) dbgSerial->print(msg);
}

void debug::println(const char *msg)
{
    if(enable) dbgSerial->println(msg);
}

void debug::print(String msg)
{
    if(enable)
        dbgSerial->print(msg);
}

void debug::println(String msg)
{
    if(enable)
        dbgSerial->println(msg);
}

void debug::print(int val)
{
    if(enable) dbgSerial->print(val);
}

void debug::println(int val)
{
    if(enable) dbgSerial->println(val);
}

void debug::print(uint32_t val)
{
    if(enable) dbgSerial->print(val);
}

void debug::println(uint32_t val)
{
    if(enable) dbgSerial->println(val);
}

void debug::print(long val)
{
    if(enable) dbgSerial->print(val);
}

void debug::println(long val)
{
    if(enable) dbgSerial->println(val);
}

void debug::print(String msg, String val)
{
    if(enable){
        dbgSerial->print(msg);
        dbgSerial->print(val);
    }
}

void debug::println(String msg, String val)
{
    if(enable){
        dbgSerial->print(msg);
        dbgSerial->println(val);
    }
}

void debug::print(String msg, uint32_t val)
{
    if(enable){
        dbgSerial->print(msg);
        dbgSerial->print(val);
    }
}

void debug::println(String msg, uint32_t val)
{
    if(enable){
        dbgSerial->print(msg);
        dbgSerial->println(val);
    }
}

void debug::print(String msg1, String msg2, String msg3)
{
    if(enable){
        dbgSerial->print(msg1);
        dbgSerial->print(msg2);
        dbgSerial->print(msg3);
    }
}

void debug::println(String msg1, String msg2, String msg3)
{
    if(enable){
        dbgSerial->print(msg1);
        dbgSerial->print(msg2);
        dbgSerial->println(msg3);
    }
}

/////////////////////////////////////////////////////////////

template <typename T>
void debug::printArray(const T arr[], size_t size)
{
    if(!enable) return;

    dbgSerial->print("[ ");
    for(size_t i = 0; i < size; i++) {
        dbgSerial->print(arr[i]);

        if(i < size - 1)
            dbgSerial->print(", ");
    }

    dbgSerial->println(" ]");
}

/////////////////////////////////////////////////////////////

void debug::Test()
{
    if (!enable) return;

    char buffer[32];
    sprintf(buffer, "COUNT=%04lu", counter++);

    dbgSerial->println(buffer);
}

/////////////////////////////////////////////////////////////

void debug::printHex(const uint8_t* data, size_t len)
{
    if (!enable) return;

    for (size_t i = 0; i < len; i++) {

        if (data[i] < 0x10)
            dbgSerial->print('0');

        dbgSerial->print(data[i], HEX);
        dbgSerial->print(' ');
    }

    dbgSerial->println();
}