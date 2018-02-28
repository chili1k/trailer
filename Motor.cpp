#include "Motor.h"
#include "Arduino.h"
#include "Config.h"

void Motor::setup(int directionPin, int powerPin) {
  this->directionPin = directionPin;
  this->powerPin = powerPin;

  pinMode(directionPin, OUTPUT);
  pinMode(powerPin, OUTPUT);
}

void Motor::start(Direction newDirection) {
  if (currentDirection == newDirection) {
    return;
  }

  if (newDirection == Forward) {
    digitalWrite(directionPin, MOTOR_FORWARD_STATE);
  } else if (newDirection == Reverse) {
    digitalWrite(directionPin, !MOTOR_FORWARD_STATE);
  }

  bool shouldStart = newDirection != Stopped;
  digitalWrite(powerPin, shouldStart);

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

