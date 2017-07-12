#include "Gyro.h"
#include "Leg.h"
#include "Display.h"
#include "Arduino.h"

#include "TrailerStateManager.h"

TrailerStateManager::TrailerStateManager(Gyro *gyro, Legs *legs) {
  this->gyro = gyro;
  this->legA = &legs->legA;
  this->legB = &legs->legB;
  this->legC = &legs->legC;
  this->legD = &legs->legD;
}

void TrailerStateManager::setup() {
  legA->setup();
  legB->setup();
  legC->setup();
  legD->setup();

  gyro->setup();
  display.setup();
}

DesiredState TrailerStateManager::getState() {
  // Trailer can only be balanced if all legs are on ground
  if (gyro->isBalanced() && allLegsOnGround()) {
    return Levelled;
  }

  if ((legA->getPosition() == Zero) && (legB->getPosition() == Zero) &&
      (legC->getPosition() == Zero) && (legD->getPosition() == Zero)) {
    return Start;
  }

  return None;
}

void TrailerStateManager::toZero() {
  desiredState = Start;
  
  collapseLegIfNotZero(legA);
  collapseLegIfNotZero(legB);
  collapseLegIfNotZero(legC);
  collapseLegIfNotZero(legD);
}

void TrailerStateManager::toBalanced() {
  if (getState() != Start) {
    display.println("Error: Move legs to START first");
    return;
  }

  desiredState = Levelled;

  legA->expand();
  legB->expand();
  legC->expand();
  legD->expand();
}

void TrailerStateManager::collapseLegIfNotZero(Leg *leg) {
  if (leg->getPosition() != Zero) {
    leg->collapse();
  }
}

void TrailerStateManager::loop() {
  legA->loop();
  legB->loop();
  legC->loop();
  legD->loop();

  // Always check zero or final state for safety
  stopMotorIfZeroOrFinal(legA);
  stopMotorIfZeroOrFinal(legB);
  stopMotorIfZeroOrFinal(legC);
  stopMotorIfZeroOrFinal(legD);
 
  if (desiredState == None) {
    return;
  }

  // Stop everything if any motors stopped during levelling
  if (desiredState == Levelled && anyMotorStopped()) {
    stopAllMotors();
    display.println("Error levelling. All motors stopped.");
    desiredState = None;
  }

  if (desiredState == Levelled && allLegsOnGround()) {
    loopLevel();
  }
}

bool TrailerStateManager::anyMotorStopped() {
  return legA->isMotorStopped() || legB->isMotorStopped() || legC->isMotorStopped() || legD->isMotorStopped();
}

void TrailerStateManager::loopLevel() {  
  if (getState() == Levelled) {
    stopAllMotors();
    desiredState = None;
  }

  // Could also be A and C at the same time
  if (gyro->getPitchPosition() == UnderBalanced && gyro->getRollPosition() == UnderBalanced) {
    expandLeg(legA);
  } else if (gyro->getPitchPosition() == UnderBalanced && gyro->getRollPosition() == OverBalanced) {
    expandLeg(legB);
  // Could also be B and D at the same time
  } else if (gyro->getPitchPosition() == OverBalanced && gyro->getRollPosition() == OverBalanced) {
    expandLeg(legC);
  } else if (gyro->getPitchPosition() == OverBalanced && gyro->getRollPosition() == UnderBalanced) {
    expandLeg(legD);
  }
}

void TrailerStateManager::expandLeg(Leg *leg) {
  // Check if any other motor is already running
  if (leg->isMotorStopped()) {
    stopAllMotors();
    leg->expand();
  }
}

/**
 * Determines if all legs are on ground
 */
bool TrailerStateManager::allLegsOnGround() {
  return legA->isOnGround() && legB->isOnGround() && legC->isOnGround() && legD->isOnGround();
}

void TrailerStateManager::stopMotorIfZeroOrFinal(Leg *leg) {
  if ( (leg->getPosition() == Zero && leg->isMotorRunning()) ||
       (leg->getPosition() == Final && leg->isMotorRunning()) ) {
    leg->stop();
    
    display.print("Leg ");
    display.print(leg->getName());
    display.print(" stopped.");
    
    if (leg->getPosition() == Zero) {
      display.print("Motor reached ZERO position.");
    } else {
      display.print("Motor reached FINAL position.");
    }
  }
}

void TrailerStateManager::stopAllMotors() {
  legA->stop();
  legB->stop();
  legC->stop();
  legD->stop();
}

