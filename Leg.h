#ifndef Leg_h
#define Leg_h

#include "Motor.h"
#include "Smooth.h"

#define MAX_LEGS 4
#define LEG_A 0
#define LEG_B 1
#define LEG_C 2
#define LEG_D 3

enum LegPosition { Unknown, Expanding, Collapsing, Zero, Final };

struct LegConfig {
  const char *name;
  int pinMotorForward;
  int pinMotorReverse;
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
    LegConfig legConfig;
    Smooth smoother;
};

#endif
