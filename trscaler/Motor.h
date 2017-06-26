/*
  Motor.h - Motor controlled by relays
  Created by Mitja Bezensek, Jun 20, 2017
*/

#ifndef Motor_h
#define Motor_h

enum MotorState { Stopped, Forward, Reverse };

class Motor {
  public:
    Motor(int forwardPin, int reversePin);
    void setState(MotorState newState);
    void stop();
    MotorState getState();
  private:
    int forwardPin;
    int reversePin;
    MotorState oldState;
    MotorState currentState;
    void setup();
};

#endif
