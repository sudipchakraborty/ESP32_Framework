#include "UDPDebug.h"
#include <stdarg.h>

UDPDebug::UDPDebug() {}



// #include <WiFi.h>
// #include "UDPDebug.h"

// const char* ssid = "YOUR_WIFI";
// const char* pass = "YOUR_PASS";

// UDPDebug debug;

// void setup()
// {
//     Serial.begin(115200);

//     WiFi.begin(ssid, pass);
//     while (WiFi.status() != WL_CONNECTED)
//         delay(500);

//     Serial.println("WiFi Connected");

//     // PC IP where C# app is running
//     debug.begin(IPAddress(192,168,1,100), 5005);

//     debug.setPrefix("[ESP32] ");

//     debug.println("System started");
// }

// void loop()
// {
//     debug.printf("Millis: %lu\n", millis());
//     delay(1000);
// }















// ---------------- Init ----------------
void UDPDebug::begin(IPAddress ip, uint16_t port)
{
    remoteIP = ip;
    remotePort = port;
    udp.begin(0); // random local port
}

// ---------------- Enable/Disable ----------------
void UDPDebug::setEnabled(bool en)
{
    enabled = en;
}

// ---------------- Prefix ----------------
void UDPDebug::setPrefix(const String &p)
{
    prefix = p;
}

// ---------------- Write ----------------
size_t UDPDebug::write(uint8_t c)
{
    if (!enabled) return 1;

    buffer += (char)c;

    // Send on newline or buffer full
    if (c == '\n' || buffer.length() > 200)
    {
        sendBuffer();
    }

    return 1;
}

// ---------------- Flush ----------------
void UDPDebug::flush()
{
    sendBuffer();
}

// ---------------- Send ----------------
void UDPDebug::sendBuffer()
{
    if (buffer.length() == 0) return;

    udp.beginPacket(remoteIP, remotePort);

    if (prefix.length())
        udp.print(prefix);

    udp.print(buffer);
    udp.endPacket();

    buffer = "";
}

// ---------------- printf ----------------
void UDPDebug::printf(const char *format, ...)
{
    char temp[256];

    va_list args;
    va_start(args, format);
    vsnprintf(temp, sizeof(temp), format, args);
    va_end(args);

    print(temp);
}