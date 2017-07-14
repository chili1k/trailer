#include "TrailerContext.h"
#include "TrailerState.h"
#include "Display.h"
#include "Gyro.h"
#include "Leg.h"

#define MAX_LEGS 4
#define LEG_A 0
#define LEG_B 1
#define LEG_C 2
#define LEG_D 3

TrailerContext::TrailerContext(Gyro *gyro, Leg **legs) {
  this->gyro = gyro;
  this->legs = legs;
  
  this->zeroState = new ZeroState(this);
/*
  this->toZeroState = ;
  this->expandingToGroundState;
  this->balancedState;
  this->balancingState;
  this->errorState;
  */
}

void TrailerContext::setState(TrailerState *newState) {
  state = newState;
}

void TrailerContext::setup() {
  for (int i = 0; i < MAX_LEGS; i++) {
    legs[i]->setup();
  }

  gyro->setup();
}

void TrailerContext::loop() {
  state->loop();
}

void TrailerContext::stopAllMotors() {
  for (int i = 0; i < MAX_LEGS; i++) {
    legs[i]->stop();
  }
}

bool TrailerContext::allLegsInPosition(LegPosition position) {
  int count = 0;

  for (int i = 0; i < MAX_LEGS; i++) {
    if (legs[i]->getPosition() == position) {
      count++;
    }
  }

  return count == MAX_LEGS;
}

Gyro *TrailerContext::getGyro() {
  return gyro;
}

Leg **TrailerContext::getLegs() {
  return legs;
}
