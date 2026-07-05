// #include "modbus.h"
// // #include <ArduinoModbus.h>
// #include <WiFi.h>


// static const char* _serverIp;
// static uint16_t _port;
// static unsigned long lastReconnect = 0;

// bool modbusInit(const char* serverIp, uint16_t port)
// {
//     _serverIp = serverIp;
//     _port = port;

//     if (!ModbusTCPClient.begin(serverIp, port))
//     {
//         Serial.println("Modbus TCP connect failed");
//         return false;
//     }

//     Serial.println("Modbus TCP connected");
//     return true;
// }

// bool modbusConnected()
// {
//     return ModbusTCPClient.connected();
// }

// bool modbusReadHolding(
//     uint8_t unitId,
//     uint16_t startAddress,
//     uint16_t quantity,
//     uint16_t* buffer
// )
// {
//     if (!ModbusTCPClient.connected())
//         return false;

//     if (!ModbusTCPClient.requestFrom(
//             unitId,
//             HOLDING_REGISTERS,
//             startAddress,
//             quantity))
//     {
//         return false;
//     }

//     for (uint16_t i = 0; i < quantity; i++)
//     {
//         buffer[i] = ModbusTCPClient.read();
//     }

//     return true;
// }

// bool modbusWriteSingleHolding(
//     uint8_t unitId,
//     uint16_t address,
//     uint16_t value
// )
// {
//     if (!ModbusTCPClient.connected())
//         return false;

//     return ModbusTCPClient.holdingRegisterWrite(
//         unitId,
//         address,
//         value
//     );
// }

// void modbusTask()
// {
//     // Auto reconnect every 5 seconds
//     if (!ModbusTCPClient.connected())
//     {
//         if (millis() - lastReconnect > 5000)
//         {
//             lastReconnect = millis();
//             ModbusTCPClient.begin(_serverIp, _port);
//         }
//     }
// }
