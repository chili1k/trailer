#ifndef WinchMotor_h
#define WinchMotor_h

#include "Motor.h"

class WinchMotor : public Motor {
    public:
        void setup(MotorConfig motorConfig);
        void forward();
        void reverse();
        void brake();
        void stop();
        // Not used
        void setPWM(int pwm);

    private:
        int directionPin;
        int powerPin;
};

#endif
