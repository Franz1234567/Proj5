#ifndef ANALOG_OUT_H
#define ANALOG_OUT_H

#include <timer_msec.h>
#include <digital_out.h>

class Analog_out
{
public:
    Analog_out(int pin);
    void init(double duty_cycle);
    void set(double duty_cycle);

private:
    Timer_msec timer;
public:    
    Digital_out pin_digi;
};

#endif 