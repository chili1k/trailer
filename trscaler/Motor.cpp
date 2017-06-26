#include "Motor.h"
#include "Arduino.h"

Motor::Motor(int forwardPin, int reversePin) {
  this->forwardPin = forwardPin;
  this->reversePin = reversePin;
  this->currentState = Stopped;
  this->oldState = Stopped;
  this->oldState = false;
  this->
  setup();
}

void Motor::setup() {
  pinMode(forwardPin, OUTPUT);
  pinMode(reversePin, OUTPUT);
}

void Motor::setState(MotorState newState) {
  if (currentState == newState) {
    return;  
  }
  
  // Make sure motor is stopped first
  stop();

  switch (newState) {
    case Forward:
      digitalWrite(forwardPin, true);
      break;
    case Reverse:
      digitalWrite(reversePin, true);
      break;
    case Stopped:
      digitalWrite(forwardPin, false);
      digitalWrite(reversePin, false);
      break;
  }

  oldState = currentState;
  currentState = newState;
}

MotorState Motor:getCurrentState() {
  return currentState;
}

MotorState Motor:getOldState() {
  return currentState;
}

void Motor::stop() {
  setState(Stopped);
}

