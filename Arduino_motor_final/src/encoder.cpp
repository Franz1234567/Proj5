#include <encoder.h>
#include <digital_in.h>
#include <avr/io.h>

Encoder::Encoder(int  pin_number):encoder_pin(pin_number){
    encoder_pin.init();
}

bool Encoder::is_low(){
    return (encoder_pin.is_lo());
}

int Encoder::position(){
    return count;
}