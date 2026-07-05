#ifndef RS485_H
#define RS485_H

#include <Arduino.h>
#include <HardwareSerial.h>
////////////////////////////////////////
class RS485 
{
public:
    // Initialize RS485 interface
    void DefaultSetUp();
    
    bool begin(
        HardwareSerial& serial,
        uint32_t baudrate,
        int rxPin,
        int txPin,
        int deRePin,
        uint32_t serialConfig = SERIAL_8N1
    );
   

    // LED config
    void configLED(int txLedPin, int rxLedPin);

    // (optional) manual control if needed
    void setTxLED(bool state);
    void setRxLED(bool state);

    // Data handling
    bool available();          // Is any data received?
    int  read();               // Read one byte
    size_t read(uint8_t* buf, size_t len);
    int test_count=0;

    // Transmission
    bool send(const uint8_t* data, size_t len);
    bool send(const char* str);
    void Test_Send(void);
    void Echo();

    // Control / maintenance
    void flush();
    void reset();              // Clear RX buffer
    void setTransmitMode();
    void setReceiveMode();
    size_t receivePacket(uint8_t* buf, size_t maxLen, uint16_t waitMs);
    void Test_RE_DE(void);

private:
    HardwareSerial* _serial;
    int _deRePin;
    int _txLedPin = -1;
    int _rxLedPin = -1;

    void blinkTxLED();
    void blinkRxLED();

    void preTransmit();
    void postTransmit();
};
//////////////////////////////////////////
#endif
