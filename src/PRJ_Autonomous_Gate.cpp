
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
#include "PWM.h"
///////////////////////////////////

/// default declaration ///////////
extern StatusBlink st;
extern Autonomous_Gate gate;  
///////////////////////////////////


////specific object ///////////////
PWM Motor(19,1000,10);          // 1kHz, 10-bit
///////////////////////////////////

// assign for input 
IO home(PIN_SEN_HOME,IO_INPUT,ACTIVE_LOW);
IO end(PIN_SEN_TERMINAL,IO_INPUT,ACTIVE_LOW);
IO outdoor(PIN_SW_OUTDOOR,IO_INPUT,ACTIVE_LOW);
IO indoor(PIN_SW_INDOOR,IO_INPUT,ACTIVE_LOW);

// input assignment
IO calling_Bell(PIN_CALLING_BELL,IO_OUTPUT,ACTIVE_LOW);
IO Lpwm(PIN_L_PWM,IO_OUTPUT,ACTIVE_LOW);
IO Len(PIN_L_EN,IO_OUTPUT,ACTIVE_LOW);
IO Rpwm(PIN_R_PWM,IO_OUTPUT,ACTIVE_LOW);
IO Ren(PIN_R_EN,IO_OUTPUT,ACTIVE_LOW);
IO fault(PIN_LED_FAULT,IO_OUTPUT,ACTIVE_LOW);
IO dir(PIN_RE_DE,IO_OUTPUT,ACTIVE_LOW);
IO bzr(PIN_BZR,IO_OUTPUT,ACTIVE_LOW);
////////////////////////////////


//__________________________________________________________________________________________________________________________
void Autonomous_Gate::PRJ_Autonomous_Gate_SetUp()
{
    st.begin(PIN_ST_LED, 50000);
    calling_Bell.begin();
    Lpwm.begin(); 
    Len.begin(); 
    Rpwm.begin(); 
    Ren.begin(); 
    fault.begin(); 
    dir.begin(); 
    bzr.begin();
}
//_________________________________________________________________________________________________________
void Autonomous_Gate::PRJ_Autonomous_Gate_Loop()
{
    st.blink();
    FSM_Handler();
}
//_________________________________________________________________________________________________________
void Autonomous_Gate::FSM_Handler()
{
    switch (gate.FSM)
    {
    case FSM_init:
         
        break;
    
    default:
        break;
    }
}
//_________________________________________________________________________________________________________

