#ifndef MODBUS_ASCII_H
#define MODBUS_ASCII_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include "DEBUG.h"

#define MODBUS_ASCII_MAX_FRAME 128

// ------------ Modbus ASCII Parsed Frame ------------
typedef struct {
    bool     valid;          // Frame valid or not
    uint8_t  slaveId;
    uint8_t  function;
    uint16_t address;
    uint16_t quantity;       // or value (depends on function)
    uint16_t value;          // for write
    uint8_t  rawData[64];    // raw PDU data
    uint8_t  rawLen;
} ModbusASCIIFrame;



// struct ModbusASCIIFrame {
//     uint8_t address;
//     uint8_t function;
//     uint8_t data[64];
//     uint8_t dataLen;
// };

class ModbusASCII {
public:
    bool isModbusPacketHealthy(const ModbusASCIIFrame& f);
    bool parseModbusASCII(const char* frame, ModbusASCIIFrame& outFrame);
    void debugPrintModbusFrame(const ModbusASCIIFrame& f, debug& dbg);
    bool PacketAvailable=false;
private:
    HardwareSerial* _serial;

    char rxBuf[MODBUS_ASCII_MAX_FRAME];
    uint16_t rxIndex;

    bool frameStarted;
    bool frameReady;

    ModbusASCIIFrame lastFrame;

    bool parseFrame();
    uint8_t lrc(const char* data, uint16_t len);
    uint8_t hexToByte(char hi, char lo);
    uint8_t calcLRC(const uint8_t* data, uint8_t len);
};

#endif
