#ifndef MODBUS_RESPONSE_BUILDER_H
#define MODBUS_RESPONSE_BUILDER_H

#include <stdint.h>
#include <stddef.h>
#include "modbusASCII.h"

#define MODBUS_MAX_REGS   125
#define MODBUS_BIN_MAX    (3 + MODBUS_MAX_REGS * 2)   // 253 bytes
#define MODBUS_ASCII_MAX  520                         // safe ASCII buffer

char Modbus_BuildResponse(
    const ModbusASCIIFrame* frame,
    char* responseBuffer,
    size_t bufferSize
);

#endif