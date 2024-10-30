#include <timer_msec.h>
#include <avr/io.h>
#include <stdint.h>


Timer_msec::Timer_msec(){}

void Timer_msec::init_pwm(double duty_cycle){
    TCCR1A = 0;  // set timer1 to normal operation (all bits in control registers A and B set to zero)
    TCCR1B = 0;
    TCNT1 = 0; // initialize counter value to 0
 
    OCR1A = (16000000.0/64) * (450/1000000.0) -1; 
    OCR1B = OCR1A * (duty_cycle / 100.0);

    TCCR1B |= (1 << WGM12);

    TIMSK1 |= (1 << OCIE1A);       // set interrupt on compare match A
    TIMSK1 |= (1 << OCIE1B);       // set interrupt on compare match B
  
    TCCR1B |= (1 << CS11) | (1 << CS10); // set prescaler to 64 and start the timer
}

void Timer_msec::set_duty_cycle(double duty_cycle){
    OCR1B = OCR1A * (duty_cycle / 100.0);
}

void Timer_msec::init_speed(){
    // timer of 1 second using timer0 (which has 8 bits)
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;

    OCR0A = (16000000.0/1024.0) * (8.0/1000.0) -1; //124 // see report for explanation about this target count
    TCCR0A |= (1 << WGM01);
    TIMSK0 |= (1 << OCIE0A); // set interrupt 
    TCCR0B |= (1 << CS00) | (1 << CS02); // prescaler of 1024
}


void Timer_msec::init_pulses_count(){
    // timer of waiting_time (280us) using timer2 (which has 8 bits)
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;

    OCR2A = (16000000.0/1024.0) * (280.0/1000000.0) -1; // see report for explanation 
    TCCR2A |= (1 << WGM21);
    TIMSK2 |= (1 << OCIE2A);
    TCCR2B |= (1 << CS20) | (1 << CS22);
}
