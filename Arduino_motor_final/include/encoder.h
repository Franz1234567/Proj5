#ifndef ENCODER_H
#define ENCODER_H

#include <digital_in.h>

class Encoder{
    public: 
        Encoder(int pin);
        int position();
        bool is_low();
        int count = 0;
    private:
        Digital_in encoder_pin;
};

#endif