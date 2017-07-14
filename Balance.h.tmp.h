#ifndef Balance_h
#define Balance_h

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

enum TrailerState { None, Zero, Balanced, Final, Error };

class Balance {
  public:
    Balance(Gyro *gyro, Legs *legs);
    TrailerState getState();
    void setup();
    void loop();

    void stopAllMotors();
    void balance();
    void toZero();
    void startMotor(char legId);

  private:
    Gyro *gyro;
    Leg *legA, *legB, *legC, *legD;
    Display display;
    TrailerState desiredState;

    void collapseLegIfNotZero(Leg *leg);
    void loopLevel();
    void expandLeg(Leg *leg);
    bool allLegsOnGround();
    void stopMotorIfZeroOrFinal(Leg *leg);
    bool anyMotorRunning();
    bool anyMotorStopped();
    void stopAllMotors();
};

#endif
