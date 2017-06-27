/*
  Level.h - Levelling logic.
            IMPORTANT: device must be in zero position in order to be levelled
  Created by Mitja Bezensek, Jun 20, 2017
*/

#ifndef Level_h
#define Level_h

#include "Motor.h"
#include "Gyro.h"
#include "Leg.h"
#include "Display.h"

/*
A .            B
|--------------|
|*.            |
| .            |
|--------------| 
C              D
*/
struct Motors {
  Motor motorA;
  Motor motorB;
  Motor motorC;
  Motor motorD;
};

struct Legs {
  Leg legA;
  Leg legB;
  Leg legC;
  Leg legD;
};

enum LevelState { Start, Levelled, None };

class Level {
  public:
    Level(Gyro *gyro, Motors *motors, Legs *legs, Display *display);
    LevelState Level::getState();
    void loop();
    void moveToStart();
    void startLevelling();
    void startStop();
  private:
    Gyro *gyro;
    Motors *motors;
    Legs *legs;
    Display *display;
    LevelState desiredState;
    void reverseMotorIfNotStart(Leg *leg, Motor *motor);
    void loopStartOrFinal();
    void loopLevel();
    bool anyMotorStopped();
    void startSingleMotor(Motor *motor);
    bool allLegsOnGround();
    void stopMotorIfZeroOrFinal(Leg *leg, Motor *motor);
    void stopAllMotors();
    
    
};

#endif
