#ifndef HBridgeMotor_h
#define HBridgeMotor_h

#include "Motor.h"

// Pin modes:
#define ONE_PIN 1
#define TWO_PIN 2

#define HBRIDGE_MOTOR_2K  0
#define HBRIDGE_MOTOR_1KH 1
#define HBRIDGE_MOTOR_1K  2
#define HBRIDGE_MOTOR_2KH 3

struct HBridgeConfig {
    // ONE_PIN or TWO_PIN
    int pinMode;
    // Configuration for directions as bits.
    // pin positions:
    // 0 0 0 0 0 0 0 0
    //             | |_ pin1
    //             |___ pin2
    unsigned short bitsForward;
    unsigned short bitsReverse;
    unsigned short bitsBrake;
    unsigned short bitsStop;
};

const HBridgeConfig hBConfigs[] = {
    // HBRIDGE_MOTOR_2K
    {ONE_PIN,0x0,0x1,0x2,0x3},
    // HBRIDGE_MOTOR_1KH
    {TWO_PIN,0x1,0x2,0x0,0x0},
    // HBRIDGE_MOTOR_1K
    // For this motor the PWM logic is reverse, so 0 is max power
    {TWO_PIN,0x2,0x1,0x0,0x0},
    // HBRIDGE_MOTOR_2KH
    {TWO_PIN,0x1,0x2,0x0,0x0}
};

class HBridgeMotor : public Motor {
    public:
        HBridgeMotor(int motorType);

        void setup(MotorConfig motorConfig);

        void forward();
        void reverse();
        void brake();
        void stop();
        void setPWM(int pwm);

    private:
        MotorConfig motorConfig;
        HBridgeConfig hBConfig;

        void writeBits(short bits);
        int pwm;

        void enable(bool on);
};

#endif
