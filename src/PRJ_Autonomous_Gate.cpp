
////Default 
#include "rs485.h"
#include "modbusASCII.h"
#include "modbusResponseBuilder.h"
#include "memoryMap.h"
#include "DEBUG.h"
#include "LED.h"
#include "RELAY.h"
#include "Buzzer.h"
#include "StatusBlink.h"
#include "Rtc1307.h"
#include "AppTime.h"
#include "Utils.h"
#include "SimulatedClock.h"
#include "eepromStorage.h"
#include "IO.h"
////////////////////////////////////

//// Project specific include //////
#include <PRJ_Autonomous_Gate.h>
#define ST_LED_PIN 2
///////////////////////////////////

/// default declaration ///////////
StatusBlink st;


IO calling_Bell(14,IO_OUTPUT,ACTIVE_LOW);


//__________________________________________________________________________________________________________________________
void PRJ_Autonomous_Gate_SetUp()
{
    st.begin(ST_LED_PIN, 5000);
    calling_Bell.begin();



}
//_______________________________________________________________________________
void PRJ_Autonomous_Gate_Loop()
{
      st.blink();
      
    // calling_Bell.on();


}
//_______________________________________________________________________________
