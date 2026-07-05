#ifndef MODBUS_CLIENT_H
#define MODBUS_CLIENT_H

#include <Arduino.h>

bool modbusInit(const char* serverIp, uint16_t port);
bool modbusConnected();

bool modbusReadHolding(
    uint8_t unitId,
    uint16_t startAddress,
    uint16_t quantity,
    uint16_t* buffer
);

bool modbusWriteSingleHolding(
    uint8_t unitId,
    uint16_t address,
    uint16_t value
);

void modbusTask();   // keep-alive / reconnect

#endif
