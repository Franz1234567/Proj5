#ifndef TEST_P_CONTROLLER_H
#define TEST_P_CONTROLLER_H

class P_controller{
    public:
        P_controller(double Kp);
        virtual double update(double ref, double actual);
    private:
        double Kp_priv;
};

#endif