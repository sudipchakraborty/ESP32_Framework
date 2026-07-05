#include "StatusBlink.h"
#include <Arduino.h>
///////////////////////////
StatusBlink::StatusBlink()  // Constructor
{   
}
//__________________________________________________________________________________________________________________________________________________________________
/**
 * @brief initialize function
 * @param pin to set the output
  * @param speed to set the blink speed
 * @return void
 */
void StatusBlink::begin(char pin, long speed)
{
    pin_no=pin;
    speed_val=speed;
    pinMode(pin, OUTPUT);
    state=true;
    delay_reg=0;
}
//__________________________________________________________________________________________________________________________________________________________________
/**
 * @brief just on the status LED
 * @param void
 * @return void
 */
void StatusBlink::on(void)
{
  digitalWrite(pin_no, HIGH);
}
//__________________________________________________________________________________________________________________________________________________________________
/**
 * @brief just instant off the LED
 * @param void
 * @return void
 */
void StatusBlink::off(void)
{
  digitalWrite(pin_no, LOW);
}
//__________________________________________________________________________________________________________________________________________________________________
void StatusBlink::blink()
{
    if(state==true)
    {
      delay_reg++;
      if(delay_reg>=speed_val)
      {
        state=false;
        digitalWrite(pin_no, LOW);    
        delay_reg=0;
      }
    }
    else
    {
      delay_reg++;
      if(delay_reg>=speed_val)
      {
        state=true;
        digitalWrite(pin_no, HIGH);
        delay_reg=0;
      }
    }
}
//__________________________________________________________________________________________________________________________________________________________________
 void StatusBlink::toggle()
{
    state = !state;
    digitalWrite(pin_no, state);      // apply state
}
//__________________________________________________________________________________________________________________________________________________________________
                
                
 
 
