#include "rs485.h"

///// for Module test fucntion///
// #include "rs485.h"
// #include "modbusASCII.h"
// #include "modbusResponseBuilder.h"

// void LPTM_setup()
// {
//     rs485.DefaultSetUp();
// }

// void LPTM_loop()
// {
// //    rs485.Echo();
// }

// rs485.Echo();
// rs485.Test_RE_DE();
// rs485.Test_Send();
///////////////////////////////
void RS485::DefaultSetUp()
{
    _serial = &Serial2;
    _deRePin = 33;

    pinMode(_deRePin, OUTPUT);
    digitalWrite(_deRePin, LOW);   // RX mode

    _serial->begin(115200, SERIAL_8N1, 16, 17);
    configLED(32, 23);
}
//_______________________________________________________________________________________________________
bool RS485::begin(HardwareSerial& serial,uint32_t baudrate,int rxPin, int txPin, int deRePin,uint32_t serialConfig)
{
    _serial = &serial;
    _deRePin = deRePin;

    pinMode(_deRePin, OUTPUT);
    digitalWrite(_deRePin, LOW);   // RX mode

    _serial->begin(baudrate, serialConfig, rxPin, txPin);
    return true;
}
//_______________________________________________________________________________________________________
void RS485::configLED(int txLedPin, int rxLedPin)
{
    _txLedPin = txLedPin;
    _rxLedPin = rxLedPin;

    if (_txLedPin != -1) {
        pinMode(_txLedPin, OUTPUT);
        digitalWrite(_txLedPin, HIGH);
    }

    if (_rxLedPin != -1) {
        pinMode(_rxLedPin, OUTPUT);
        digitalWrite(_rxLedPin, HIGH);
    }
}
//_______________________________________________________________________________________________________
bool RS485::available()
{
    if (!_serial->available())
        return 0;
    else
        return 1;
}
//_______________________________________________________________________________________________________
size_t RS485::receivePacket(uint8_t* buf, size_t maxLen, uint16_t waitMs)
{
    if (!buf || maxLen == 0)
        return 0;

    // Wait for remaining bytes to arrive
    delay(waitMs);

    size_t count = 0;

    while (_serial->available() && count < maxLen) {
        buf[count++] = _serial->read();
    }

    // Clear any leftover garbage
    reset();
    return count;
}
//_______________________________________________________________________________________________________
int RS485::read()
{
    if (_serial->available()) {
        setRxLED(true);          // 🔥 RX ON
        int val = _serial->read();

        if (!_serial->available()) {
            setRxLED(false);     // 🔥 RX OFF when buffer empty
        }
        return val;
    }
    return -1;
}
//_______________________________________________________________________________________________________
size_t RS485::read(uint8_t* buf, size_t len)
{
    size_t i = 0;
    while (_serial->available() && i < len) {
        buf[i++] = _serial->read();
    }
    return i;
}
//_______________________________________________________________________________________________________
bool RS485::send(const uint8_t* data, size_t len)
{
    if (!data || len == 0) return false;

    setTxLED(true);        // 🔥 TX ON
    preTransmit();

    _serial->write(data, len);
    _serial->flush();      // wait until sent

    postTransmit();
    setTxLED(false);       // 🔥 TX OFF

    return true;
}
//_______________________________________________________________________________________________________
bool RS485::send(const char* str)
{
    if (!str) return false;
    return send((const uint8_t*)str, strlen(str));
}
//_______________________________________________________________________________________________
void RS485::flush()
{
    _serial->flush();
}
//_______________________________________________________________________________________________
void RS485::reset()
{
    while (_serial->available()) {
        _serial->read();
    }
}
//_______________________________________________________________________________________________
void RS485::setTransmitMode()
{
    digitalWrite(_deRePin, HIGH);
}
//_______________________________________________________________________________________________
void RS485::setReceiveMode()
{
    digitalWrite(_deRePin, LOW);
}
//_______________________________________________________________________________________________
void RS485::preTransmit()
{
    digitalWrite(_deRePin, HIGH);
    delayMicroseconds(50);
}
//_______________________________________________________________________________________________
void RS485::postTransmit()
{
    delayMicroseconds(50);
    digitalWrite(_deRePin, LOW);
}
//_______________________________________________________________________________________________
void RS485::Test_RE_DE(void)
{
    digitalWrite(_deRePin, HIGH);
        delayMicroseconds(1000);
    digitalWrite(_deRePin, LOW);
        delayMicroseconds(1000);
}
//_______________________________________________________________________________________________
void RS485::Test_Send()
{
    static uint32_t counter = 0;
    char buffer[32];

    // Create pattern: COUNT=0001
    sprintf(buffer, "COUNT=%04lu\r\n", counter++);

    preTransmit();
    _serial->write((uint8_t*)buffer, strlen(buffer));
    _serial->flush();
    postTransmit();
}
//_______________________________________________________________________________________________
void RS485::Echo()
{
    if (!_serial->available()) return;

    uint8_t buffer[128];
    size_t len = 0;

    setRxLED(true);   // 🔥 RX START

    delay(10);

    while (_serial->available() && len < sizeof(buffer)) {
        buffer[len++] = _serial->read();
    }

    setRxLED(false);  // 🔥 RX END

    if (len > 0) {
        send(buffer, len);
    }
}
//_______________________________________________________________________________________________
void RS485::setTxLED(bool state)
{
    if (_txLedPin != -1)
        digitalWrite(_txLedPin, state ? LOW : HIGH);
}
//_______________________________________________________________________________________________
void RS485::setRxLED(bool state)
{
    if (_rxLedPin != -1)
        digitalWrite(_rxLedPin, state ? LOW : HIGH);
}
//_______________________________________________________________________________________________
