#include "Buzzer.h"
//______________________________________________________________________________
Buzzer::Buzzer(int pin, bool logic) {
     _pin=pin;
    _logic=logic;
    pinMode(_pin, OUTPUT);
    off();
}
//______________________________________________________________________________
void Buzzer::begin() {
    pinMode(_pin, OUTPUT);
    off();
}
//______________________________________________________________________________
void Buzzer::on() {
   if(_logic==1){
        digitalWrite(_pin,HIGH);
    }
    else{
         digitalWrite(_pin,LOW);
    }
}
//______________________________________________________________________________
void Buzzer::off() {
    if(_logic==1)
    {
        digitalWrite(_pin,LOW);
    }
    else{
        digitalWrite(_pin,HIGH);
    }
}
//______________________________________________________________________________
void Buzzer::beep() {
    on();
    delay(100);
    off();
}
//______________________________________________________________________________
