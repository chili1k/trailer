#include "Arduino.h"

#include "Leg.h"
#include "Motor.h"
#include "Debug.h"

Leg::Leg(LegConfig *legConfig) {
  this->legConfig = legConfig; 
}

void Leg::setup() {
  Motor m = Motor(legConfig->pinMotorForward, legConfig->pinMotorReverse);
  this->motor = &m;
  motor->setup();

  pinMode(legConfig->pinZeroPos, INPUT);
  pinMode(legConfig->pinFinalPos, INPUT);

  // TODO
  pinMode(legConfig->pinPowerMeter, INPUT);
}

const char *Leg::getName() {
  return legConfig->name;
}

void Leg::loop() {
  if (motor->isRunning()) {
    _isOnGround = isHighAmperage();
  }
}

void Leg::expand() {
  if (getPosition() == Final) {
    DPRINTLN("Cannot expand leg beyond final position.");
    return;
  }

  motor->start(Forward);
}

void Leg::collapse() {
  if (getPosition() == Zero) {
    DPRINTLN("Cannot collapse leg beyond zero position.");
    return;
  }

  motor->start(Reverse);
}

LegPosition Leg::getPosition() {
  if (digitalRead(legConfig->pinZeroPos) == HIGH) {
    return Zero;
  }

  if (digitalRead(legConfig->pinFinalPos) == HIGH) {
    return Final;
  }

  if (motor->getDirection() == Forward) {
    return Expanding;
  }

  if (motor->getDirection() == Reverse) {
    return Collapsing;
  }

  return Unknown;
}

bool Leg::isOnGround() {
  return _isOnGround;
}

bool Leg::isMotorRunning() {
  return !motor->isStopped();
}

bool Leg::isMotorStopped() {
  return !isMotorRunning();
}
