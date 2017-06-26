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

enum LevelState { Zero, Levelled, Moving, Error, None };

class Level {
  public:
    Level(Gyro *gyro, Motors *motors, Legs *legs, Display *display);
    LevelState getState();
    void setState(LevelState desiredState);
    void loop();
    void level();
    bool isMoving();
    void stop();
  private:
    Gyro *gyro;
    Motors *motors;
    Legs *legs;
    Display *display;
    LevelState desiredStateFinal;
    LevelState desiredStateCurrent;
    Level::stopMotors();
};

#endif
