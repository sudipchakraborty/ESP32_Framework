#include <Arduino.h>
#include <StatusBlink.h>
#include "rs485.h"
#include "modbusASCII.h"
#include "modbusResponseBuilder.h"
#include "at24c32N.h"
#include "Rtc1307.h"
#include "memoryMap.h"
#include "eepromStorage.h"
#include "Utils.h"
#include "LED.h"
#include "RELAY.h"
#include "Buzzer.h"
#include "Time.h"
#include "CANBus.h"
#include "Enum.h"

#include <PRJ_Dispenser.h> 
// #include "MQTTManager.h"
//////////////////////////
StatusBlink st1;
CANBus can(&Serial);
CANBus::Packet_t pkt;
 
uint8_t buffer[256];

// void mqttCallback(char* topic, byte* payload, unsigned int length);
// MQTTManager mqtt("RTIH","rtih@9849","178.238.231.143",1883);
//___________________________________________________________________________________________________________
void DispenserModule_setup()
{
    Serial2.begin(38400);
    st1.begin(LED_PIN, 10000);
    can.begin(38400);                                   // communication
    Serial2.println("Dispenser Module Started...");

    // mqtt.begin();
    // mqtt.setCallback(mqttCallback);
}
//__________________________________________________________________________________________________________
void DispenserModule_loop()
{
    // mqtt.loop();
    // can.receiveNonBlocking();
      
    // if (can.getPacket(pkt))
    // {
    //     // ✅ PACKET RECEIVED HERE
    //     processPacket();
    // }

    st1.toggle();
}
//__________________________________________________________________________________________________________
void processPacket()
{
    if(pkt.address)
    {
            if(pkt.address==DISP_6X6_GRAINS){
                 Serial2.println("My packet received..");
            }
            else
            {
                 Serial2.println("The packet is someone Else..");
            }
    }
}
//__________________________________________________________________________________________________________
// void mqttCallback(char* topic, byte* payload, unsigned int length)
// {
//     String msg = "";

//     for (int i = 0; i < length; i++)
//     {
//         msg += (char)payload[i];
//     }

//     Serial.println("MQTT Message:");
//     Serial.println(msg);

//     //--------------------------------------
//     // TEST COMMANDS
//     //--------------------------------------

//     if(msg == "LED_ON")
//     {
//         digitalWrite(LED_PIN, HIGH);
//         Serial.println("LED ON");
//     }

//     if(msg == "LED_OFF")
//     {
//         digitalWrite(LED_PIN, LOW);
//         Serial.println("LED OFF");
//     }

//     if(msg == "DISPENSE")
//     {
//         Serial2.println("DISPENSER START");
//     }

//     if(msg == "BUZZER")
//     {
//         Serial.println("BUZZER COMMAND");
//     }
// }
// //__________________________________________________________________________________________________________



















