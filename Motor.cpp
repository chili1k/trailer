#include "Motor.h"
#include "Arduino.h"

void Motor::setup(int forwardPin, int reversePin) {
  this->forwardPin = forwardPin;
  this->reversePin = reversePin;
  this->currentDirection = Stopped;
  this->setupDone = false;

  if (setupDone) {
    return;
  }

  pinMode(forwardPin, OUTPUT);
  pinMode(reversePin, OUTPUT);
  setupDone = true;
}

void Motor::start(Direction newDirection) {
  if (currentDirection == newDirection) {
    return;
  }

  // Make sure motor is stopped before changing direction
  digitalWrite(forwardPin, false);
  digitalWrite(reversePin, false);
  
  if (newDirection == Forward) {
    digitalWrite(forwardPin, true);
  } else if (newDirection == Reverse) {
    digitalWrite(reversePin, true);
  }

  currentDirection = newDirection;
}

bool Motor::isStopped() {
  return currentDirection == Stopped;
}

bool Motor::isRunning() {
  return !isStopped();
}
Direction Motor::getDirection() {
  return currentDirection;
}

void Motor::stop() {
  start(Stopped);
}

