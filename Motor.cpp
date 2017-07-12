#include "Motor.h"
#include "Arduino.h"

Motor::Motor(char *name, int forwardPin, int reversePin) {
  this->forwardPin = forwardPin;
  this->reversePin = reversePin;
  this->currentDirection = Stopped;
  this->name = name;
  setup();
}

char* Motor::getName() {
  return name;
}

void Motor::setup() {
  pinMode(forwardPin, OUTPUT);
  pinMode(reversePin, OUTPUT);
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

Direction Motor::getDirection() {
  return currentDirection;
}

void Motor::stop() {
  start(Stopped);
}

