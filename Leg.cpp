#include "Leg.h"
#include "Arduino.h"

Leg::Leg(int pinZeroPos, int pinFinalPos, int pinPowerMeter) {
  this->pinZeroPos = pinZeroPos;
  this->pinFinalPos = pinFinalPos;
  this->pinPowerMeter = pinPowerMeter;
  setup();
}

void Leg::setup() {
  pinMode(pinZeroPos, INPUT);
  pinMode(pinFinalPos, INPUT);

  // TODO
  pinMode(pinPowerMeter, INPUT);
}

LegPosition Leg::getPosition() {
  if (digitalRead(pinZeroPos) == HIGH) {
    return Zero;
  }

  if (digitalRead(pinFinalPos) == HIGH) {
    return Final;
  }

  if (isHighAmperage()) {
    return Ground;
  }

  return Expanding;
}

bool Leg::isHighAmperage() {
  // TODO
  return false;
}

