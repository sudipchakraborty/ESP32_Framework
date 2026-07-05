#ifndef CANBUS_H
#define CANBUS_H

#include "Arduino.h"
#include "Enum.h"

#define MAX_DATA_LEN 256
#define BUFFER_SIZE 256

class CANBus
{
public:
    CANBus(HardwareSerial *serialPort);   // constructor with serial
    void begin(uint32_t baud);
    void send_string(String msg);
    // CAN TX/RX
    void sendMessage(uint32_t id, uint8_t* data, uint8_t len);
    bool receiveMessage(uint32_t &id, uint8_t* data, uint8_t &len);
  

    // Packet structure
    typedef struct
    {
        uint8_t preamble[2];   // 66 55
        uint8_t length;
        uint8_t transtype;
        uint8_t packetType;
        PIC_Type_t address;
        uint8_t rw;
        uint8_t command;
        uint8_t data[MAX_DATA_LEN];
        uint8_t dataLen;
        uint16_t crc;
        uint8_t postamble[2];  // 77 88
        bool valid;
    } Packet_t;

    // Parser
    bool parsePacket(uint8_t *buf, size_t len, Packet_t &pkt);
    size_t buildResponsePacket(Packet_t &pkt, uint8_t *outBuf);
    void receiveNonBlocking();
    void printPacket(const Packet_t &pkt);

    bool isPacketAvailable();
    bool getPacket(Packet_t &pkt);
private:
    HardwareSerial *serial;   // 🔥 injected serial object
    uint8_t rxBuffer[BUFFER_SIZE];
    size_t rxIndex = 0;
    unsigned long lastReceiveTime = 0;
    const unsigned long timeoutMs = 10;

    void printBufferHex(uint8_t *buf, size_t len);
    uint16_t calculateCRC(uint8_t *data, size_t len);
    const char* getPacketTypeName(uint8_t type);
    const char* getRWName(uint8_t rw);
    // const char* getPICName(uint16_t addr);
    const char* getPICName(PIC_Type_t addr);
 
    Packet_t lastPacket;
    bool packetAvailable = false;

  
};

#endif