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

struct LegConfig {
  const char *name;
  int pinMotorDirection;
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

    bool isOnGround();
    bool isMotorRunning();
    bool isMotorStopped();
    bool isHighAmperage();
    bool isZero();
    bool isFinal();
    LegPosition getPosition();
    float getAmpers();
    
  private:
    Motor motor;
    bool _isOnGround;
    bool _isHighAmperage;
    LegConfig legConfig;
    Smooth smoother;
    Bounce debounceZero;
    Bounce debounceFinal;

    void loopCheckIfOnGround();
};

#endif
