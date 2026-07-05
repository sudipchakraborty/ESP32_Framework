#include "modbusASCII.h"
#include "arduino.h"  // For hexToByte and LRC functions
#include "DEBUG.h"
//________________________________________________________________________________________________________
bool ModbusASCII::isModbusPacketHealthy(const ModbusASCIIFrame& f)
{
    // 1️⃣ Basic validity
    if (!f.valid)
        return false;

    // 2️⃣ Slave ID sanity (Modbus spec: 1–247)
    if (f.slaveId == 0 || f.slaveId > 247)
        return false;

    // 3️⃣ Supported function codes
    switch (f.function)
    {
        case 0x03:  // Read Holding Registers
        case 0x04:  // Read Input Registers
            // Expected raw data: address(2) + quantity(2)
            if (f.rawLen != 4)
                return false;

            // Quantity must be >=1 and reasonable
            if (f.quantity == 0 || f.quantity > 125)
                return false;
            break;

        case 0x06:  // Write Single Register
            // Expected raw data: address(2) + value(2)
            if (f.rawLen != 4)
                return false;
            break;

        // Optional: add more supported functions here
        // case 0x10: // Write Multiple Registers

        default:
            // Unsupported / unknown function
            return false;
    }

    // 4️⃣ Address sanity (application-specific, adjust if needed)
    // Example: limit register range
    if (f.address > 0xFFFF)
        return false;

    // 5️⃣ Raw data buffer safety
    if (f.rawLen > sizeof(f.rawData))
        return false;

    // All checks passed
    return true;
} 
//_______________________________________________________________________________________________________
bool ModbusASCII::parseModbusASCII(const char* frame, ModbusASCIIFrame& outFrame)
{
    memset(&outFrame, 0, sizeof(outFrame));
    outFrame.valid = false;

    if (!frame || frame[0] != ':')
        return false;

    size_t len = strlen(frame);

    // Must have at least ":0103LRC"
    if (len < 9)
        return false;

    uint8_t bin[64];
    uint8_t binLen = 0;

    for (size_t i = 1; i + 1 < len; i += 2)
    {
        if (frame[i] == '\r' || frame[i] == '\n')
            break;

        if (!isxdigit(frame[i]) || !isxdigit(frame[i + 1]))
            return false;

        bin[binLen++] = hexToByte(frame[i], frame[i + 1]);

        if (binLen >= sizeof(bin))
            return false;
    }

    // Address + Function + LRC minimum
    if (binLen < 3)
        return false;

    uint8_t rxLRC = bin[binLen - 1];
    uint8_t calc  = calcLRC(bin, binLen - 1);

    if (rxLRC != calc)
        return false;

    outFrame.slaveId  = bin[0];
    outFrame.function = bin[1];

    // Payload excludes address, function, LRC
    uint8_t payloadLen = binLen - 3;

    if (payloadLen > sizeof(outFrame.rawData))
        return false;

    memcpy(outFrame.rawData, &bin[2], payloadLen);
    outFrame.rawLen = payloadLen;

    // ---- Function decode ----
    if (outFrame.function == 0x03 || outFrame.function == 0x04)
    {
        if (payloadLen != 4)
            return false;

        outFrame.address  = (bin[2] << 8) | bin[3];
        outFrame.quantity = (bin[4] << 8) | bin[5];
    }
    else if (outFrame.function == 0x06)
    {
        if (payloadLen != 4)
            return false;

        outFrame.address = (bin[2] << 8) | bin[3];
        outFrame.value   = (bin[4] << 8) | bin[5];
    }
    else
    {
        return false;
    }

    outFrame.valid = true;
    return true;
}
//_______________________________________________________________________________________________________
void ModbusASCII::debugPrintModbusFrame(const ModbusASCIIFrame& f, debug& dbg)
{
    dbg.println("===== MODBUS ASCII FRAME =====");

    dbg.println("Valid      : ", f.valid ? "YES" : "NO");

    if (!f.valid) {
        dbg.println("Frame invalid, skipping details");
        dbg.println("==============================");
        return;
    }

    dbg.println("Slave ID   : ", String(f.slaveId));

    char funcStr[10];
    sprintf(funcStr, "0x%02X", f.function);
    dbg.println("Function   : ", String(funcStr));

    dbg.println("Address    : ", String(f.address));

    if (f.function == 0x03 || f.function == 0x04) {
        dbg.println("Quantity   : ", String(f.quantity));
    }

    if (f.function == 0x06) {
        dbg.println("Value      : ", String(f.value));
    }

    dbg.println("Raw Length : ", String(f.rawLen));

    dbg.println("Raw Data   : ");
    dbg.printHex(f.rawData, f.rawLen);
    
    dbg.println("==============================");
}
//_______________________________________________________________________________________________________
uint8_t ModbusASCII::hexToByte(char hi, char lo)
{
    auto h = (hi <= '9') ? hi - '0' : (toupper(hi) - 'A' + 10);
    auto l = (lo <= '9') ? lo - '0' : (toupper(lo) - 'A' + 10);
    return (h << 4) | l;
}
//_______________________________________________________________________________________________________
uint8_t ModbusASCII::calcLRC(const uint8_t* data, uint8_t len)
{
    uint8_t lrc = 0;
    for (uint8_t i = 0; i < len; i++)
        lrc += data[i];
    return ((~lrc) + 1);
}
//_______________________________________________________________________________________________________