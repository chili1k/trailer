#include "NPN.h"
#include "Arduino.h"

Leg::Leg(int pinZeroPos, int pinCollapsedPos, int pinPowerMeter) {
  this->pin = pin;
  this->pinZeroPos = pinZeroPos;
  this->pinCollapsedPos = pinCollapsedPos;
  this->pinPowerMeter = pinPowerMeter;
  setup();
}

void Leg::setup() {
  pinMode(pinZeroPos, INPUT);
  pinMode(pinCollapsedPos, INPUT);

  // TODO
  pinMode(pinPowerMeter, INPUT);
}

LegPosition Leg::getPosition() {
  if (digitalRead(pinCollapsedPos) == HIGH) {
    return Collapsed;
  }

  if (digitalRead(pinFinal) == HIGH) {
    return Collapsed;
  }

  isHighAmperage = isHighAmperage();

  if (isHighAmperage) {
    return Levelling;
  } else {
    return Expanding;
  }

  return Error;
}

bool Leg:isHighAmperage() {
  // TODO
  return false;
}

