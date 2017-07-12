#ifndef Leg_h
#define Leg_h

#include "Motor.h"

enum LegPosition { Unknown, Expanding, Collapsing, Zero, Final };

struct LegConfig {
  char *name;
  int pinMotorForward;
  int pinMotorReverse;
  int pinZeroPos;
  int pinFinalPos;
  int pinPowerMeter;
};

class Leg {
  public:
    Leg(LegConfig *legConfig);
    void loop();
    void setup();
    const char *getName();

    // Actions
    void expand(); 
    void collapse();
    void stop();

    bool isOnGround();
    bool isMotorRunning();
    bool isMotorStopped();
    bool isHighAmperage();
    LegPosition getPosition();
    
  private:
    Motor *motor;
    bool _isOnGround;
    LegConfig *legConfig;
};

#endif
