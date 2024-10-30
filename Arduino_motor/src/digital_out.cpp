#include <digital_out.h>
#include <avr/io.h>

Digital_out::Digital_out(int pin) {
    pinMask = (1 << pin);
}

void Digital_out::init() {
    DDRB |= pinMask; // Set pin as output
}

void Digital_out::set_hi() {
    PORTB |= pinMask; // Set pin high
}

void Digital_out::set_lo() {
    PORTB &= ~pinMask; // Set pin low
}

void Digital_out::toggle() {
    PORTB ^= pinMask; // change pin state
}
