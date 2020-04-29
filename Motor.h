#ifndef Motor_h
#define Motor_h

struct MotorConfig {
    // First direction pin
    int directionPin1;
    // Second direction pin. Is ignored if directionPinCount=1.
    int directionPin2;
    // PWM pin
    int enablePin;
    // PWM value. Used only for some motors.
    int pwm;
};

class Motor {
    public:
        virtual void setup(MotorConfig config) = 0;
        virtual void forward() = 0;
        virtual void reverse() = 0;
        virtual void brake() = 0;
        virtual void stop() = 0;
        virtual void setPWM(int pwm) = 0;
};

#endif
