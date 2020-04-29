#ifndef Leg_h
#define Leg_h

#include "Motor.h"
#include "Smooth.h"

#include <Bounce2.h>

#define MAX_LEGS 4
#define LEG_A 0
#define LEG_B 1
#define LEG_C 2
#define LEG_D 3

enum class LegPosition { Unknown, Expanding, Collapsing, Zero, Final };
enum class MotorState { Forward, Reverse, Stopped };

struct LegConfig {
    const char *name;
    Motor *motor;
    // 0-255
    int pwm;
    int pinMotorDirection1;
    int pinMotorDirection2;
    int pinMotorPower;
    int pinZeroPos;
    int pinFinalPos;
    int pinPowerMeter;
};

class Leg {
    public:
        Leg(LegConfig legConfig);
        void setup();
        void loop();
        const char *getName();

        // Actions
        void expand();
        void collapse();
        void stop();
        void setPWM(int pwm);

        bool isOnGround();
        bool isMotorRunning();
        bool isMotorStopped();
        bool isHighAmperage();
        bool isZero();
        bool isFinal();
        LegPosition getPosition();
        int getPWM();
        float getAmpers();
        void legStateToString(char *&state);

    private:
        Motor *motor;
        MotorConfig motorConfig;

        MotorState motorState;
        bool _isOnGround;
        bool _isHighAmperage;
        LegConfig legConfig;
        Smooth smoother;
        Bounce debounceZero;
        Bounce debounceFinal;

        void loopCheckIfOnGround();
};

#endif
