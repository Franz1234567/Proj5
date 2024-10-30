#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

class PI_Controller : public P_controller {
private:
    double Kp_priv;
    double Ti_priv;
    double T_priv;
    double sum_error_priv;
public:
    PI_Controller();
    double update(double ref, double actual) override;
    double get_sum_error();
    void init(double Kp, double Ti, double T);
};

#endif
