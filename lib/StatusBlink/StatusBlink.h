#ifndef STATUSBLINK_H
#define STATUSBLINK_H
//////////////////////
class StatusBlink
{
    public:
           StatusBlink();
      void begin(char pin,long speed);
      void blink();
      void on(void);
      void off(void);
      void toggle(void);
    private:
      unsigned int  delay_reg;
      unsigned int  speed_val;
      char pin_no;
      bool state=false;
};
///////////////////////











#endif