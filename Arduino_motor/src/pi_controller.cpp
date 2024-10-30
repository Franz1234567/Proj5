#include "global.h"

double error = 0;
double u = 0;

PI_Controller::PI_Controller():P_controller(){

}

void PI_Controller::init(double Kp, double Ti, double T){
    Kp_priv = Kp;
    Ti_priv = Ti;
    T_priv = T;
    sum_error_priv = 0;
}

double PI_Controller::update(double ref, double actual){
    error = ref - actual;
    sum_error_priv += error * T_priv;
    u =  Kp_priv * error + (Kp_priv/Ti_priv) * sum_error_priv;
    if (u >= max_speed){ 
        sum_error_priv -= error * T_priv; // If the output is saturated, we do not want to accumulate the error
    }
    return u;
}

double PI_Controller::get_sum_error(){
    return sum_error_priv;
}