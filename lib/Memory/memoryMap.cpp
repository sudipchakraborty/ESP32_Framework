#include "memoryMap.h"



void ModbusMemory_LoadTestPattern()
{
    for (uint16_t i = 0; i < MODBUS_MEM_SIZE; i++)
    {
        uint8_t val = i % 100;   // keep it simple (0–99)

        // Pattern: 0x0011, 0x1122, 0x2233 ...
        modbusMemory[i] = (val << 8) | ((val + 1) % 100);
    }
}

/* -------------------------------------------------
 * Global Modbus Data (DEFINITION)
 * This must exist in exactly ONE .cpp file
 * ------------------------------------------------- */
// ModbusData_t modbusData = {0};

uint16_t modbusMemory[MODBUS_MEM_SIZE] = {0};

/* -------------------------------------------------
 * Read Holding Register
 * Address range: 40000+
 * ------------------------------------------------- */
// uint16_t modbusMemoryRead(uint16_t address)
// {
//     switch (address)
//     {
//         case 0: return modbusData.OUTPUTS;
//         case 1: return modbusData.START_HH_MM;
//         case 2: return modbusData.END_HH_MM;
//         default:
//             return 0;
//     }
// }

/* -------------------------------------------------
 * Write Holding Register
 * ------------------------------------------------- */
// void modbusMemoryWrite(uint16_t address, uint16_t value)
// {
//     switch (address)
//     {
//         case 0: modbusData.OUTPUTS   = value; break;
//         case 1: modbusData.START_HH_MM   = value; break;
//         case 2: modbusData.END_HH_MM      = value; break;
//         default:
//             break;
//     }
// }
// ///////////////////////////////////////////////////////////////