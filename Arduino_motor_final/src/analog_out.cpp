# include <analog_out.h>
# include <digital_out.h>

Analog_out::Analog_out(int pin):pin_digi(pin){
    pin_digi.init();
}

void Analog_out::init(double duty_cycle){
    timer.init_pwm(duty_cycle);
}

void Analog_out::set(double duty_cycle){
    timer.set_duty_cycle(duty_cycle);
}