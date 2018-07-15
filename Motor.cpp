#include "Motor.h"
#include "Arduino.h"
#include "Config.h"
#include "Debug.h"

void Motor::setup(int directionPin, int powerPin) {
  this->directionPin = directionPin;
  this->powerPin = powerPin;

  pinMode(directionPin, OUTPUT);
  pinMode(powerPin, OUTPUT);
  digitalWrite(directionPin, MOTOR_OFF_PIN_MODE);
  digitalWrite(powerPin, MOTOR_OFF_PIN_MODE);
}

bool getPinMode(Direction direction) {
  return (direction != Stopped ? MOTOR_ON_PIN_MODE : MOTOR_OFF_PIN_MODE);
}

void Motor::start(Direction newDirection) {
  if (newDirection == Forward) {
    digitalWrite(directionPin, MOTOR_FORWARD_STATE);
  } else if (newDirection == Reverse) {
    digitalWrite(directionPin, MOTOR_REVERSE_STATE);
  }

  digitalWrite(powerPin, getPinMode(newDirection));
  // also turn off direction pin (#savetheplanet)
  if (newDirection == Stopped) {
    digitalWrite(directionPin, MOTOR_OFF_PIN_MODE);
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
