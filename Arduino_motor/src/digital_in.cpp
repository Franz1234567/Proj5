#include <avr/io.h>
#include <digital_in.h> 

Digital_in::Digital_in(int pin) {
    pinMask = (1 << pin);
}

void Digital_in::init() {
    DDRB &= ~pinMask; // set IOB_2 to be an input
    PORTB |= pinMask; // enable internal pull-up resistor
}

bool Digital_in::is_hi() {
    return (PINB & pinMask) == 1;
}

bool Digital_in::is_lo() {
    return (PINB & pinMask) == 0;
} 