#ifndef UDP_DEBUG_H
#define UDP_DEBUG_H

#include <Arduino.h>
#include <WiFiUdp.h>

class UDPDebug : public Stream
{
public:
    UDPDebug();

    void begin(IPAddress remoteIP, uint16_t port);
    void setEnabled(bool en);
    void setPrefix(const String &p);

    // Stream overrides
    virtual size_t write(uint8_t c);
    virtual int available() { return 0; }
    virtual int read() { return -1; }
    virtual int peek() { return -1; }
    virtual void flush();

    // printf support
    void printf(const char *format, ...);

private:
    WiFiUDP udp;
    IPAddress remoteIP;
    uint16_t remotePort;

    String buffer;
    bool enabled = true;
    String prefix = "";

    void sendBuffer();
};

#endif