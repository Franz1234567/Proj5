#include "test_p_controller.h"

P_controller::P_controller(){
}

void P_controller::init(double Kp){
    Kp_priv = Kp;
}

double P_controller::update(double ref, double actual){
    double error = ref - actual;
    double u = Kp_priv * error;
    return u;
}