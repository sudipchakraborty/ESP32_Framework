#ifndef MODBUS_RTU_CLIENT_H
#define MODBUS_RTU_CLIENT_H

#include <Arduino.h>
#include <HardwareSerial.h>

class ModbusRTUClientCustom {
public:
    bool begin(HardwareSerial& serial, uint32_t baudrate);
    bool readHoldingRegisters(
        uint8_t slaveId,
        uint16_t startAddress,
        uint16_t quantity,
        uint16_t* out
    );
    bool writeSingleRegister(
        uint8_t slaveId,
        uint16_t address,
        uint16_t value
    );

private:
    HardwareSerial* _serial;
    uint16_t crc16(uint8_t* buf, uint16_t len);
    void preTransmit();
    void postTransmit();
};

#endif
