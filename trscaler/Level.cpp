#include "Level.h"
#include "Gyro.h"
#include "Leg.h"
#include "Display.h"
#include "Arduino.h"

Level::Level(Gyro *gyro, Motors *motors, Legs *legs, Display *display) {
  this->gyro = gyro;
  this->motors = motors;
  this->legs = legs;
  this->display = display;
}

LevelState Level::getState() {
  if (gyro.isLevelled()) {
    return Levelled;
  }

  if (motors->motorA.getMotorState() != Stopped || 
      motors->motorB.getMotorState != Stopped ||
      motors->motorC.getMotorState != Stopped ||
      motors->motorD.getMotorState != Stopped) {
    return Moving;
  }

  if (legs->legA.getPosition() == Zero &&
    legs->legB.getPosition() == Zero &&
    legs->legC.getPosition() == Zero &&
    legs->legD.getPosition() == Zero) {
    return Zero;
  }
}

void Level::setState(LevelState desiredState) {  
  switch (desiredState) {
    case Stopped:
      startStop();
      break;
    case Zero:
      startZero();
      break;
    case Start:
      startStart();
      break;
    case Levelled:
      break;
    case Moving:
      break;
    case Stopped:
      break;
    case Error:
      break;
  }

  desiredStateFinal = desiredState;
}

void Level::startZero() {
  reverseMotorIfNotZero(legs->legA, motors->motorA);
  reverseMotorIfNotZero(legs->legB, motors->motorB);
  reverseMotorIfNotZero(legs->legC, motors->motorC);
  reverseMotorIfNotZero(legs->legD, motors->motorD);
}

void Level::startLevel() {
  if (getState() != Zero) {
    display->println("Error: Move legs to zero first");
    return;
  }

  motors->motorA.setState(Forward);
  motors->motorB.setState(Forward);
  motors->motorC.setState(Forward);
  motors->motorD.setState(Forward);
}

void Level::startStop() {
  motors->motorA.stop();
  motors->motorB.stop();
  motors->motorC.stop();
  motors->motorD.stop();
}

void Level::reverseMotorIfNotZero(Leg *leg, Motor *motor) {
  if (leg->getPosition() != Zero) {
    motor->setState(Reverse);
  }
}

void Level::loop() {
  switch (desiredFinalState): {
    case None:
      break;
    case Level:
      loopLevel();
      break;    
    case Zero:
      loopZero();
      break;
  }
}

void Level::loopZero() {
  stopMotorIfZero(legs->legA, motors->motorA);
  stopMotorIfZero(legs->legB, motors->motorB);
  stopMotorIfZero(legs->legC, motors->motorC);
  stopMotorIfZero(legs->legD, motors->motorD);
}

void Level::loopLevel() {
  // Don't do anything when not in motion
  if (getState != Moving) {
    return;
  }
  
  if (getState() == Levelled) {
    stop();
  }

  if (gyro->getPitchPosition() == UnderBalanced && gyro.getRollPosition() == UnderBalanced) {
    motors->motorC.setState(Forward);
  } else if (gyro->getPitchPosition() == UnderBalanced && gyro.getRollPosition() == OverBalanced) {
    motors->motorA.setState(Forward);
  } else if (gyro->getPitchPosition() == OverBalanced && gyro.getRollPosition() == OverBalanced) {
    motors->motorB.setState(Forward);
  } else if (gyro->getPitchPosition() == OverBalanced && gyro.getRollPosition() == UnderBalanced) {
    motors->motorD.setState(Forward);
  }
}

void Level::stopMotorIfZero(Leg *leg, Motor *motor) {
  if (leg->getPosition() == Zero && motor->getState() != Stopped) {
    motor->stop();
  }
}

void Level::stop() {
  setState(Stopped);
}

