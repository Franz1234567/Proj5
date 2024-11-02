#ifndef P_CONTROLLER_H
#define P_CONTROLLER_H

class P_controller{
    public:
        P_controller();
        virtual double update(double ref, double actual);
        virtual void init(double Kp);
    private:
        double Kp_priv;
};

#endif