#include <math.h>

#include "Arduino.h"

#include "Leg.h"
#include "Motor.h"
#include "Debug.h"
#include "Config.h"

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
    int powerReading = analogRead(legConfig->pinPowerMeter);
    smoother.putReading(powerReading);
    _isOnGround = isHighAmperage();
  }

  // Safety - do not allow leg over final position
  if (getPosition() == Final) {
    motor->stop();
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

float Leg::getAmpers() {
  //float mV = (raw / 1023.0) * 5000.0;
  //float amps = ((2500.0 - mV) / 66.0);
  float rawAverage = smoother.getAverage();
  return 37.87-(0.07405 * rawAverage);
}

bool Leg::isHighAmperage() {
  // We don't care how the current meter is turned, because
  // power is only high when leg is expanding
  return abs(getAmpers()) > LEG_ON_GROUND_AMPS;
}
