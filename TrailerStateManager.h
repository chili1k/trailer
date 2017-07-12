#ifndef TrailerStateManager_h
#define TrailerStateManager_h

#include "Motor.h"
#include "Gyro.h"
#include "Leg.h"
#include "Display.h"

/*

      roll
       -  A            B 
pitch  -  +------------+  + pitch
          |o  vcc      |
          |            |
          +------------+
          C            D
       +
      roll
*/

struct Legs {
  Leg legA;
  Leg legB;
  Leg legC;
  Leg legD;
};

enum DesiredState { Start, Levelled, None };
//enum TrailerState { Zero, Expanding to ground, Ground, Balancing, Balanced, Final, Error };

class TrailerStateManager {
  public:
    TrailerStateManager(Gyro *gyro, Legs *legs);
    DesiredState getState();
    void setup();
    void loop();

    void toZero();
    //void toGround();
    void toBalanced();

  private:
    Gyro *gyro;
    Leg *legA, *legB, *legC, *legD;
    Display display;
    DesiredState desiredState;

    void collapseLegIfNotZero(Leg *leg);
    void loopLevel();
    bool anyMotorStopped();
    void expandLeg(Leg *leg);
    bool allLegsOnGround();
    void stopMotorIfZeroOrFinal(Leg *leg);
    void stopAllMotors();
    
    
};

#endif
