#include "CANBus.h"
#include <string.h>
#include <HardwareSerial.h>
#include "Enum.h"

/////////////////////////////////////////
CANBus::CANBus(HardwareSerial *serialPort)
{
    serial = serialPort;
}

//__________________________________________________________________________________________________________
void CANBus::begin(uint32_t baud)
{
    serial->begin(baud);
}

//__________________________________________________________________________________________________________
void CANBus::send_string(String msg)
{
    serial->println(msg);   // use injected serial
}

//__________________________________________________________________________________________________________
void CANBus::sendMessage(uint32_t id, uint8_t* data, uint8_t len)
{
    // TODO: Implement CAN send
}

bool CANBus::receiveMessage(uint32_t &id, uint8_t* data, uint8_t &len)
{
    // TODO: Implement CAN receive
    return false;
}

// =========================
// 🔥 PACKET PARSER (XMODEM CRC)
// =========================
bool CANBus::parsePacket(uint8_t *buf, size_t len, Packet_t &pkt)
{
    pkt.valid = false;

    if (len < 13)
    {
        Serial2.println("❌ Packet too small");
        return false;
    }

    // 🔹 Preamble
    if (buf[0] != 0x66 || buf[1] != 0x55)
    {
        Serial2.println("❌ Invalid Preamble");
        return false;
    }

    pkt.preamble[0] = buf[0];
    pkt.preamble[1] = buf[1];

    pkt.length = buf[2];

    // 🔥 Length = TOTAL packet length
    if (len != pkt.length)
    {
        Serial2.printf("❌ Length mismatch: RX=%d LEN=%d\r\n", len, pkt.length);
        return false;
    }

    // 🔹 Header
    pkt.transtype = buf[3];
    pkt.packetType = buf[4];
    pkt.address = (PIC_Type_t)((buf[5] << 8) | buf[6]);
    pkt.rw = buf[7];
    pkt.command = buf[8];

    // 🔹 Data length
    pkt.dataLen = pkt.length - (2 + 1 + 6 + 2 + 2);

    if (pkt.dataLen > MAX_DATA_LEN)
    {
        Serial2.println("❌ Data too large");
        return false;
    }

    if (pkt.dataLen > 0)
    {
        memcpy(pkt.data, &buf[9], pkt.dataLen);
    }

    // 🔹 CRC (LOW → HIGH from C#)
    int crcIndex = 9 + pkt.dataLen;
    pkt.crc = (buf[crcIndex] << 8) | buf[crcIndex + 1];

    // 🔹 Postamble
    pkt.postamble[0] = buf[crcIndex + 2];
    pkt.postamble[1] = buf[crcIndex + 3];

    if (pkt.postamble[0] != 0x77 || pkt.postamble[1] != 0x88)
    {
        Serial2.println("❌ Invalid Postamble");
        return false;
    }

    // 🔥 CRC CHECK (XMODEM)
    uint16_t calculatedCRC = calculateCRC(&buf[0], crcIndex);

    Serial2.printf("CRC RX: %04X | CALC: %04X\r\n", pkt.crc, calculatedCRC);

    if (calculatedCRC != pkt.crc)
    {
        Serial2.println("❌ CRC MISMATCH");
        return false;
    }

    pkt.valid = true;
    Serial2.println("✅ Packet OK");

    return true;
}

//__________________________________________________________________________________________________________
void CANBus::printBufferHex(uint8_t *buf, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        Serial2.printf("%02X ", buf[i]);
    }
    Serial2.println();
}

//__________________________________________________________________________________________________________
void CANBus::receiveNonBlocking()
{
    uint8_t byteIn;

    while (serial->available() > 0)
    {
        byteIn = serial->read();
        if (rxIndex < BUFFER_SIZE)
        {
            rxBuffer[rxIndex++] = byteIn;
        }
        lastReceiveTime = millis();
    }
    if (rxIndex > 0 && (millis() - lastReceiveTime >= timeoutMs))
    {
        printBufferHex(rxBuffer, rxIndex);
        Packet_t pkt;
        if (parsePacket(rxBuffer, rxIndex, pkt))
        {
            lastPacket = pkt;
            packetAvailable = true;
            printPacket(pkt); // optional
        }
        rxIndex = 0;
    }
}

//__________________________________________________________________________________________________________
size_t CANBus::buildResponsePacket(Packet_t &pkt, uint8_t *outBuf)
{
    size_t index = 0;

    // 🔹 Preamble
    outBuf[index++] = 0x66;
    outBuf[index++] = 0x55;

    // Length placeholder
    size_t lengthIndex = index;
    outBuf[index++] = 0;

    // Header
    outBuf[index++] = pkt.transtype;
    outBuf[index++] = pkt.packetType;

    outBuf[index++] = (pkt.address >> 8) & 0xFF;
    outBuf[index++] = pkt.address & 0xFF;

    outBuf[index++] = pkt.rw;
    outBuf[index++] = pkt.command;

    // Data
    for (int i = 0; i < pkt.dataLen; i++)
    {
        outBuf[index++] = pkt.data[i];
    }

    // 🔹 Length = TOTAL packet
    uint8_t totalLength = index + 2 + 2; // CRC + postamble
    outBuf[lengthIndex] = totalLength;

    // 🔹 CRC (XMODEM)
    uint16_t crc = calculateCRC(outBuf, index);

    // LOW → HIGH (match C#)
    outBuf[index++] = (crc >> 8) & 0xFF;    // HIGH
    outBuf[index++] = crc & 0xFF;           // LOW

    // Postamble
    outBuf[index++] = 0x77;
    outBuf[index++] = 0x88;

    return index;
}

//__________________________________________________________________________________________________________
// 🔥 CRC16-XMODEM
uint16_t CANBus::calculateCRC(uint8_t *data, size_t len)
{
    uint16_t crc = 0x0000;

    for (size_t i = 0; i < len; i++)
    {
        crc ^= (uint16_t)(data[i] << 8);

        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }

    return crc;
}

//__________________________________________________________________________________________________________
void CANBus::printPacket(const Packet_t &pkt)
{
    if (!pkt.valid)
    {
        Serial2.print("Invalid Packet\r\n");
        return;
    }

    Serial2.print("========== PACKET ==========\r\n");

    Serial2.printf("Preamble     : %02X %02X\r\n", pkt.preamble[0], pkt.preamble[1]);
    Serial2.printf("Length       : %02X\r\n", pkt.length);
    Serial2.printf("TransType    : %02X\r\n", pkt.transtype);
    // Serial2.printf("PacketType   : %02X\r\n", pkt.packetType);
    Serial2.printf("PacketType   : %02X (%s)\r\n", 
                pkt.packetType, getPacketTypeName(pkt.packetType));

    // Serial2.printf("Address      : %04X\r\n", pkt.address);
    Serial2.printf("Address      : %04X (%s)\r\n", 
                pkt.address, getPICName(pkt.address));

    // Serial2.printf("RW           : %02X\r\n", pkt.rw);

    Serial2.printf("RW           : %02X (%s)\r\n", 
                pkt.rw, getRWName(pkt.rw));

    Serial2.printf("Command      : %02X\r\n", pkt.command);

    Serial2.printf("DataLen      : %d\r\n", pkt.dataLen);

    Serial2.print("Data         : ");
    for (int i = 0; i < pkt.dataLen; i++)
    {
        Serial2.printf("%02X ", pkt.data[i]);
    }
    Serial2.print("\r\n");

    Serial2.printf("CRC          : %04X\r\n", pkt.crc);
    Serial2.printf("Postamble    : %02X %02X\r\n", pkt.postamble[0], pkt.postamble[1]);

    Serial2.print("============================\r\n");
}
//__________________________________________________________________________________________________________
const char* CANBus::getPacketTypeName(uint8_t type)
{
    switch (type)
    {
    case 0: return "Unicast";
    case 1: return "Multicast";
    default: return "Unknown";
    }
}
//__________________________________________________________________________________________________________
const char* CANBus::getRWName(uint8_t rw)
{
    switch (rw)
    {
    case 0: return "Read";
    case 1: return "Write";
    case 2: return "Execute";
    default: return "Unknown";
    }
}
//__________________________________________________________________________________________________________
const char* CANBus::getPICName(PIC_Type_t addr)
{
    switch (addr)
    {
    case DISP_3X3_GRAINS:   return "PIC_DISP_3X3_GRAINS";
    case DISP_3X3_LIQUID:   return "PIC_DISP_3X3_LIQUID";
    case DISP_3X3_PUREE:    return "PIC_DISP_3X3_PUREE";

    case DISP_6X6_GRAINS:   return "PIC_DISP_6X6_GRAINS";
    case DISP_6X6_LIQUID:   return "PIC_DISP_6X6_LIQUID";
    case DISP_6X6_PUREE:    return "PIC_DISP_6X6_PUREE";

    case MOTOR_DRIVER_1:    return "PIC_MOTOR_DRIVER1";

    case MOTOR_X: return "SM_PIC_MOTOR_DRIVER_X";
    case MOTOR_Y: return "SM_PIC_MOTOR_DRIVER_Y";

    case MOTOR_R1: return "SM_PIC_MOTOR_DRIVER_R1";
    case MOTOR_R2: return "SM_PIC_MOTOR_DRIVER_R2";
    case MOTOR_R3: return "SM_PIC_MOTOR_DRIVER_R3";
    case MOTOR_R4: return "SM_PIC_MOTOR_DRIVER_R4";
    case MOTOR_R5: return "SM_PIC_MOTOR_DRIVER_R5";

    case MOTOR_L1: return "SM_PIC_MOTOR_DRIVER_L1";
    case MOTOR_L2: return "SM_PIC_MOTOR_DRIVER_L2";
    case MOTOR_L3: return "SM_PIC_MOTOR_DRIVER_L3";
    case MOTOR_L4: return "SM_PIC_MOTOR_DRIVER_L4";

    default: return "UNKNOWN_PIC";
    }
}
//__________________________________________________________________________________________________________
bool CANBus::isPacketAvailable()
{
    return packetAvailable;
}
//__________________________________________________________________________________________________________
bool CANBus::getPacket(Packet_t &pkt)
{
    if (!packetAvailable)
        return false;
    pkt = lastPacket;           // copy
    packetAvailable = false;    // consume
    return true;
}
//__________________________________________________________________________________________________________
