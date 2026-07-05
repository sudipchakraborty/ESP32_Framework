
#include "PRJ_Server.h"

// #include "PRJ_LPTM.h"
// #include "rs485.h"
// #include "modbusASCII.h"
// #include "modbusResponseBuilder.h"
// #include "memoryMap.h"
#include "DEBUG.h"
// //////////////////////////////////
// #include "LED.h"
// #include "RELAY.h"
// #include "Buzzer.h"
#include "StatusBlink.h"
// #include "Rtc1307.h"
// #include "AppTime.h"
// #include "Utils.h"
// #include "SimulatedClock.h"
#include "MQTTManager.h"
#include <ArduinoJson.h>

// #include "eepromStorage.h"
// /////////////////////////////////////////////////////////////////////////////////////////
// uint8_t MY_SLAVE_ID = 1;   // change per device   1
// RS485 rs485;
// char txBuf[255];
// uint8_t rxBuf[255];
// ModbusASCII mb;
// ModbusASCIIFrame modbusFrame;

extern debug dbg;
extern StatusBlink st;
// Rtc1307 rtc;
// AppTime SecondTick;

// AppTime RL1_Time;
// AppTime RL2_Time;
// ////////////////////////////
// led ledTx(LED_TX,0);
// led ledRx(LED_RX,0);
// relay rl1(RL1);
// relay rl2(RL2);
// Buzzer bzr(BZR, 0);

// int FSMState;
// SimulatedClock simClock;

void mqttCallback(char* topic, byte* payload, unsigned int length);
MQTTManager mqtt("RTIH","rtih@9849","178.238.231.143",1883,&dbg);
// // MQTTManager mqtt("Dr_Sudip_Chakraborty","Anindita@01092016","178.238.231.143",1883,&dbg);

// EEPROMStorage eeprom;

// int countValue = 0;
// unsigned long lastPublish = 0;
// uint16_t temp[2];

// PC mqtt path: C:\Users\Dr. S Chakraborty>cd C:\Program Files\Mosquitto

// test code from PC....
// C:\Program Files\Mosquitto>mosquitto_pub.exe -h 178.238.231.143 -t esp32/request -m "{\"device\":\"ESP32\",\"command\":\"LED_ON\",\"time\":\"10:30:00\",\"value\":1}" -u sudip -P "Sudip@123"

// to receive response from PC side
// C:\Program Files\Mosquitto>mosquitto_sub.exe -h 178.238.231.143 -t esp32/response -u sudip -P "Sudip@123"


// ////////////////////////////////////////////////////////////////////////////////////
 void Server_setup()
 {
       st.begin(LED_PIN, 5000);
       dbg.begin(Serial,115200);
//     rtc.begin();
//     dbg.DefaultSetUp();
       dbg.println("system started..");
//     rs485.DefaultSetUp();
       dbg.println("system Initialized..");

//     SecondTick.set_time(1000);
//     SecondTick.start();

//     eeprom.begin(21, 22);
//     /////////////////////////////
//     // Test_Data_Save_To_Modbus();
//     // modbusSaveToEEPROM();

//     LoadModbusConfigFromEEPROM();
//     Update_Time_From_Modbus();
    
//     RL1_Time.set_time(10000);
//     RL1_Time.reset();
//     RL1_Time.start();

//     RL2_Time.set_time(10000);
//     RL2_Time.reset();
//     RL2_Time.start();

//     FSMState= IDLE;
//     ///////////////////////////
//     // // Example: start from RTC time
//      String s = rtc.readDT_As_ddmmyyyyhhmmss();
//      dbg.println("RTC Time: ", s);

//     //  simClock.set(s);
//     //  uint32_t startSec = Utils::DT_String_To_Seconds_From_TimePart(s.c_str());
//     //  dbg.print("Time In second: ", startSec);
  
    mqtt.begin();
    mqtt.setCallback(mqttCallback);
//     ///////////////////////////////  
 }
// //__________________________________________________________________________________________
void Server_loop()
{  
        st.blink();
        mqtt.loop();

    //     StateMachine(); 
    //     Modbus_Handler();
}
// //__________________________________________________________________________________________
// void StateMachine(void)
// {  
//     switch (FSMState) 
//     {
//         case IDLE:                       
//              FSMState=READING_RTC;
//             break;
//         ///////////////////
//         case READING_RTC:       
//             if(SecondTick.timeOut())
//             {
//                 // simClock.updateMin(30);
//                 dbg.println("==============");
//                 String s=rtc.readDT_As_ddmmyyyyhhmmss();
//                 // String s=simClock.readDT_As_ddmmyyyyhhmmss();

//                 dbg.println("TIME:",s);
//                 // dbg.println(Utils::DT_String_To_Seconds_From_TimePart(s.c_str()));

//                 SecondTick.start();
  
//                 dbg.print("RL1: ");
//                 dbg.print("ON:",Utils::secondsToHHMMSS(RL1_Time.onTime)," ");
//                 dbg.println("OFF:",Utils::secondsToHHMMSS(RL1_Time.offTime));
                
//                 dbg.print("RL2: ");
//                 dbg.print("ON:",Utils::secondsToHHMMSS(RL2_Time.onTime)," ");
//                 dbg.println("OFF:",Utils::secondsToHHMMSS(RL2_Time.offTime));
                
//                 uint32_t tt = Utils::DT_String_To_Seconds_From_TimePart(s.c_str());
//                 if(RL1_Time.isWithinWindow(tt))
//                 {
//                     rl1.on(); 
//                     dbg.print("RL1:ON ");
//                 }else
//                 {
//                     rl1.off();
//                     dbg.print("RL1:OFF ");
//                 }

//                  if(RL2_Time.isWithinWindow(tt))
//                  {
//                     rl2.on();
//                     dbg.println("RL2:ON"); 
//                 }
//                 else
//                 {
//                     rl2.off();
//                     dbg.println("RL2:OFF");
//                 }

//                 dbg.println("==============");
//             }
//             // FSMState=CHECK_RL1_TIME;
//             break;
//         ////////////////////
//         case CHECK_RL1_TIME:
            
//             // if(RL1_Time.Within_Window()){
//             //     rl1.on();
//             // }
//             // else{
//             //     rl1.off();
//             // }
//             FSMState=CHECK_RL2_TIME;
//             break;
//         /////////////////////
//          case CHECK_RL2_TIME:
//              FSMState = IDLE;
//             break;
//         /////////////////////
//         default:
//             FSMState = IDLE;
//     }
// }
// //__________________________________________________________________________________________
// void Modbus_Handler()
//  {
//      if (rs485.available()){
//         dbg.println("Received data on RS485");
//         size_t rxLen = rs485.receivePacket(rxBuf, sizeof(rxBuf) - 1, 20);

//         if (rxLen == 0) return;
//         rxBuf[rxLen] = '\0';
//         if (!mb.parseModbusASCII((char*)rxBuf, modbusFrame)) return;
        
//         // ✅ Slave ID filtering (IMPORTANT)
//         if (modbusFrame.slaveId != MY_SLAVE_ID && modbusFrame.slaveId != 0)
//         {
//             dbg.println("Packet not for me, or Not Broadcast...ignored");
//             return;
//         }
//         if (!mb.isModbusPacketHealthy(modbusFrame)) return;
//         mb.PacketAvailable=true;
//         mb.debugPrintModbusFrame(modbusFrame, dbg);

//         char txLen = Modbus_BuildResponse(&modbusFrame,txBuf,sizeof(txBuf));
//         if (txLen > 0){
//             rs485.send((uint8_t*)txBuf, txLen);
//             // dbg.print("Sent Response: ");
//             // dbg.printHex((uint8_t*)txBuf, txLen);
//             ModbusActionHandler();
//         }
//     }
//  }
// //__________________________________________________________________________________________
// void ModbusActionHandler(void)
// {
//     if (!mb.PacketAvailable) return;
//      mb.PacketAvailable=false;

//          if(modbusFrame.function == 0x06) { // Write Single Register
//             switch(modbusFrame.address) 
//             {
//             case ModbusAddr_Output:
//                   if(modbusFrame.value & Modbus_RL1) 
//                   {
//                     rl1.on();
//                     dbg.println("Relay 1 ON");
//                     } else {
//                         rl1.off();
//                         dbg.println("Relay 1 OFF");
//                     }
//                     //////////////////////////
//                     if(modbusFrame.value & Modbus_RL2) {
//                     rl2.on();
//                     dbg.println("Relay 2 ON");
//                     } else {
//                         rl2.off();
//                         dbg.println("Relay 2 OFF");
//                     }
//                     ////////////////////////////////
//                     if(modbusFrame.value & Modbus_BZR) {
//                     bzr.on();
//                     dbg.println("Buzzer ON");
//                     } else {
//                         // No direct way to turn off buzzer if using buzz(duration, frequency)
//                         // You can implement a method in Buzzer class to stop buzzing if needed
//                         bzr.off();
//                         dbg.println("Buzzer OFF");
//                     }                
//                 break;
//                 ////////////////////////////////////
//                 case 1:
//                 case 2:
//                 case 3:
//                 case 4:
//                 case 5:
//                 case 6:
//                 case 7:
//                 case 8:
//                     modbusSaveToEEPROM();
//                     LoadModbusConfigFromEEPROM();
//                     Update_Time_From_Modbus();
//                 break;
//                 /////////////////////////////////////
//                 default:
//                 dbg.print("Unhandled Register Address: ");
//                 break;
//             }
//         } else {
//             dbg.print("Unhandled Modbus Function: ");
//             // dbg.println(modbusFrame.function, HEX);
//         }   
//     }
// //__________________________________________________________________________________________
void mqttCallback(char* topic, byte* payload, unsigned int length)
{
    String json = "";

    for (int i = 0; i < length; i++)
    {
        json += (char)payload[i];
    }

    dbg.println("Received:");
    dbg.println(json);

    JsonDocument doc;

    DeserializationError err = deserializeJson(doc, json);

    if (err)
    {
        dbg.println("JSON Parse Failed");
        return;
    }

    String command = doc["command"];

    if(command == "LED_ON")
    {
        digitalWrite(2, HIGH);

        JsonDocument response;

        response["status"] = "OK";
        response["device"] = "ESP32";
        response["message"] = "LED TURNED ON";

        String out;

        serializeJson(response, out);

        mqtt.publish("esp32/response", out.c_str());

        dbg.println("Response Sent:");
        dbg.println(out);
    }

    if(command == "LED_OFF")
    {
        digitalWrite(2, LOW);

        JsonDocument response;

        response["status"] = "OK";
        response["device"] = "ESP32";
        response["message"] = "LED TURNED OFF";

        String out;

        serializeJson(response, out);

        mqtt.publish("esp32/response", out.c_str());

        dbg.println("Response Sent:");
        dbg.println(out);
    }
}
// //__________________________________________________________________________________________________________
// void modbusSaveToEEPROM(void)
// {
//     uint32_t readVal;  

//     readVal= Utils::readUint32FromModbus(modbusMemory,ModbusAddr_RL1_StartTime);
//     eeprom.writeUint32(EEPROM_Addr_RL1_OnTime, readVal);

//     readVal= Utils::readUint32FromModbus(modbusMemory,ModbusAddr_RL1_EndTime);
//     eeprom.writeUint32(EEPROM_Addr_RL1_OffTime, readVal);

//     readVal= Utils::readUint32FromModbus(modbusMemory,ModbusAddr_RL2_StartTime);
//     eeprom.writeUint32(EEPROM_Addr_RL2_OnTime, readVal);

//     readVal= Utils::readUint32FromModbus(modbusMemory,ModbusAddr_RL2_EndTime);
//     eeprom.writeUint32(EEPROM_Addr_RL2_OffTime, readVal);
// } 
// //__________________________________________________________________________________________
// void LoadModbusConfigFromEEPROM(void)
// {
//     uint32_t readVal;

//     eeprom.readUint32(EEPROM_Addr_RL1_OnTime, readVal);
//     Utils::storeUint32ToModbus(modbusMemory,ModbusAddr_RL1_StartTime,readVal);

//     eeprom.readUint32(EEPROM_Addr_RL1_OffTime, readVal);
//     Utils::storeUint32ToModbus(modbusMemory,ModbusAddr_RL1_EndTime,readVal);

//     eeprom.readUint32(EEPROM_Addr_RL2_OnTime, readVal);
//     Utils::storeUint32ToModbus(modbusMemory,ModbusAddr_RL2_StartTime,readVal);

//     eeprom.readUint32(EEPROM_Addr_RL2_OffTime, readVal);
//     Utils::storeUint32ToModbus(modbusMemory,ModbusAddr_RL2_EndTime,readVal);
// }
// //__________________________________________________________________________________________
// void Test_Data_Save_To_Modbus(void)
// {
//     uint32_t val;

//     val=Utils::timeStringToSeconds("17:00:00");
//     Utils::storeUint32ToModbus(modbusMemory,ModbusAddr_RL1_StartTime,val);

//     val=Utils::timeStringToSeconds("19:00:00");
//     Utils::storeUint32ToModbus(modbusMemory,ModbusAddr_RL1_EndTime,val);
    
//     val=Utils::timeStringToSeconds("20:00:00");
//     Utils::storeUint32ToModbus(modbusMemory,ModbusAddr_RL2_StartTime,val);

//     val=Utils::timeStringToSeconds("23:00:00");
//     Utils::storeUint32ToModbus(modbusMemory,ModbusAddr_RL2_EndTime,val);
// }
// //__________________________________________________________________________________________
// void Update_Time_From_Modbus(void)
// {
//     uint32_t val;

//     val=Utils::readUint32FromModbus(modbusMemory,ModbusAddr_RL1_StartTime);
//     RL1_Time.set_on_time(val);

//     val=Utils::readUint32FromModbus(modbusMemory,ModbusAddr_RL1_EndTime);
//     RL1_Time.set_off_time(val);

//     val=Utils::readUint32FromModbus(modbusMemory,ModbusAddr_RL2_StartTime);
//     RL2_Time.set_on_time(val);

//     val=Utils::readUint32FromModbus(modbusMemory,ModbusAddr_RL2_EndTime);
//     RL2_Time.set_off_time(val);
// }
// //__________________________________________________________________________________________
