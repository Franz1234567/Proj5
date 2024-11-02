#pragma once

class Timer_msec
{
public:
    Timer_msec();
    void init_pwm(double duty_cycle);
    void init_speed();
    int count_speed;
    void set_duty_cycle(double duty_cycle);
    void init_pulses_count();
};