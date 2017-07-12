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
  // Trailer can only be levelled if all legs are on ground
  if (gyro->isBalanced() && allLegsOnGround()) {
    return Levelled;
  }

  if (legs->legA.getPosition() == legs->legB.getPosition() ==
      legs->legC.getPosition() == legs->legD.getPosition() == Start) {
    return Start;
  }
}

void Level::moveToStart() {
  desiredState = Start;
  
  reverseMotorIfNotStart(&(legs->legA), &(motors->motorA));
  reverseMotorIfNotStart(&(legs->legB), &(motors->motorB));
  reverseMotorIfNotStart(&(legs->legC), &(motors->motorC));
  reverseMotorIfNotStart(&(legs->legD), &(motors->motorD));
}

void Level::startLevelling() {
  if (getState() != Start) {
    display->println("Error: Move legs to START first");
    return;
  }

  desiredState = Levelled;

  motors->motorA.start(Forward);
  motors->motorB.start(Forward);
  motors->motorC.start(Forward);
  motors->motorD.start(Forward);
}

void Level::reverseMotorIfNotStart(Leg *leg, Motor *motor) {
  if (leg->getPosition() != Zero) {
    motor->start(Reverse);
  }
}

void Level::loop() {
  // Always check zero or final state for safety
  loopStartOrFinal();
  
  if (desiredState == None) {
    return;
  }

  // Stop everything if any motors stopped during levelling
  if (desiredState == Levelled && anyMotorStopped()) {
    stopAllMotors();
    display->println("Error levelling. All motors stopped.");
    desiredState = None;
  }

  if (desiredState == Levelled && allLegsOnGround()) {
    loopLevel();
  }
}

void Level::loopStartOrFinal() {
  stopMotorIfZeroOrFinal(&(legs->legA), &(motors->motorA));
  stopMotorIfZeroOrFinal(&(legs->legB), &(motors->motorB));
  stopMotorIfZeroOrFinal(&(legs->legC), &(motors->motorC));
  stopMotorIfZeroOrFinal(&(legs->legD), &(motors->motorD));
}

bool Level::anyMotorStopped() {
  return (motors->motorA.isStopped() || motors->motorB.isStopped() || motors->motorC.isStopped() || motors->motorD.isStopped());
}

void Level::loopLevel() {  
  if (getState() == Levelled) {
    stopAllMotors();
    desiredState = None;
  }

  // Could also be A and C at the same time
  if (gyro->getPitchPosition() == UnderBalanced && gyro->getRollPosition() == UnderBalanced) {
    startSingleMotor(&(motors->motorA));
  } else if (gyro->getPitchPosition() == UnderBalanced && gyro->getRollPosition() == OverBalanced) {
    startSingleMotor(&(motors->motorC));
  // Could also be B and D at the same time
  } else if (gyro->getPitchPosition() == OverBalanced && gyro->getRollPosition() == OverBalanced) {
    startSingleMotor(&(motors->motorB));
  } else if (gyro->getPitchPosition() == OverBalanced && gyro->getRollPosition() == UnderBalanced) {
    startSingleMotor(&(motors->motorD));
  }
}

void Level::startSingleMotor(Motor *motor) {
  // Check if any other motor is already running
  if (motor->isStopped()) {
    stopAllMotors();
    motor->start(Forward);
  }
}

/**
 * Determines if all legs are on ground
 */
bool Level::allLegsOnGround() {
  return legs->legA.getPosition() == legs->legB.getPosition() ==
         legs->legC.getPosition() == legs->legD.getPosition() == Ground;
}

void Level::stopMotorIfZeroOrFinal(Leg *leg, Motor *motor) {
  if ( (leg->getPosition() == Zero && motor->isStopped()) ||
       (leg->getPosition() == Final && motor->isStopped()) ) {
    motor->stop();
    
    display->print("Motor ");
    display->print(motor->getName());
    display->print(" stopped.");
    
    if (leg->getPosition() == Zero) {
      display->print("Motor reached ZERO position.");
    } else {
      display->print("Motor reached FINAL position.");
    }
  }
}

void Level::stopAllMotors() {
  motors->motorA.stop();
  motors->motorB.stop();
  motors->motorC.stop();
  motors->motorD.stop();  
}

