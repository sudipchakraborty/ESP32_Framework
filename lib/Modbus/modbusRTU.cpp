#include "modbusRTU.h"

#define RS485_DE_RE 4     // GPIO4
#define MODBUS_TIMEOUT 1000

bool ModbusRTUClientCustom::begin(HardwareSerial& serial, uint32_t baudrate)
{
    _serial = &serial;

    pinMode(RS485_DE_RE, OUTPUT);
    digitalWrite(RS485_DE_RE, LOW);

    _serial->begin(baudrate, SERIAL_8N1, 16, 17); // RX2=16, TX2=17
    return true;
}

void ModbusRTUClientCustom::preTransmit()
{
    digitalWrite(RS485_DE_RE, HIGH);
    delayMicroseconds(50);
}

void ModbusRTUClientCustom::postTransmit()
{
    delayMicroseconds(50);
    digitalWrite(RS485_DE_RE, LOW);
}

uint16_t ModbusRTUClientCustom::crc16(uint8_t* buf, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t pos = 0; pos < len; pos++) {
        crc ^= buf[pos];
        for (int i = 0; i < 8; i++) {
            if (crc & 1) crc = (crc >> 1) ^ 0xA001;
            else crc >>= 1;
        }
    }
    return crc;
}

bool ModbusRTUClientCustom::readHoldingRegisters(
    uint8_t slaveId,
    uint16_t startAddress,
    uint16_t quantity,
    uint16_t* out
)
{
    uint8_t frame[8];
    frame[0] = slaveId;
    frame[1] = 0x03;
    frame[2] = startAddress >> 8;
    frame[3] = startAddress & 0xFF;
    frame[4] = quantity >> 8;
    frame[5] = quantity & 0xFF;

    uint16_t crc = crc16(frame, 6);
    frame[6] = crc & 0xFF;
    frame[7] = crc >> 8;

    preTransmit();
    _serial->write(frame, 8);
    _serial->flush();
    postTransmit();

    uint8_t response[256];
    uint16_t idx = 0;
    unsigned long start = millis();

    while (millis() - start < MODBUS_TIMEOUT) {
        if (_serial->available()) {
            response[idx++] = _serial->read();
            if (idx >= 5 && idx >= response[2] + 5) break;
        }
    }

    if (idx < 5) return false;

    for (uint16_t i = 0; i < quantity; i++) {
        out[i] = (response[3 + i * 2] << 8) |
                  response[4 + i * 2];
    }

    return true;
}

bool ModbusRTUClientCustom::writeSingleRegister(
    uint8_t slaveId,
    uint16_t address,
    uint16_t value
)
{
    uint8_t frame[8];
    frame[0] = slaveId;
    frame[1] = 0x06;
    frame[2] = address >> 8;
    frame[3] = address & 0xFF;
    frame[4] = value >> 8;
    frame[5] = value & 0xFF;

    uint16_t crc = crc16(frame, 6);
    frame[6] = crc & 0xFF;
    frame[7] = crc >> 8;

    preTransmit();
    _serial->write(frame, 8);
    _serial->flush();
    postTransmit();

    return true;
}
