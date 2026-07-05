#ifndef DEBUG_H
#define DEBUG_H
#include <Arduino.h>

class debug{

  private:
    HardwareSerial *dbgSerial = nullptr;
    double baud_rate;
    uint32_t counter = 0;
  
  public:
    debug();
    void begin(HardwareSerial &port,double speed);
    debug(HardwareSerial &serialPort);
    debug(HardwareSerial &serialPort, double BaudRate);
    void begin(HardwareSerial &port,uint32_t speed,int8_t txPin,int8_t rxPin = -1);

    void DefaultSetUp(int rxPin, int txPin);

    debug(double BaudRate);

    void DefaultSetUp();
    void begin(void);
  
   void print(const char *msg);
   void println(const char *msg);

   void print(String msg);
   void println(String msg);

    void print(int val);
    void println(int val);

    void print(uint32_t val);
    void println(uint32_t val);

    void print(long val);
    void println(long val);

    void print(String msg, String val);
    void println(String msg, String val);
    
    void print(String msg, uint32_t val); 
    void println(String msg, uint32_t val);

    void print(String msg1, String msg2,String msg3);
    void println(String msg1, String msg2,String msg3);


  
 
    /////////////////////////
    template <typename T>
    void printArray(const T arr[], size_t size);
    byte enable=true;
    void Test();
    void printHex(const uint8_t* data, size_t len);
  
};
#endif