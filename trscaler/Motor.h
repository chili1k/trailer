/*
  Motor.h - Motor controlled by relays
  Created by Mitja Bezensek, Jun 20, 2017
*/

#ifndef Motor_h
#define Motor_h

enum Direction { Forward, Reverse, Stopped };

class Motor {
  public:
    Motor(char *name, int forwardPin, int reversePin);
    char* getName();
    void start(Direction newDirection);
    void stop();
    bool isStopped();
    Direction getDirection();
  private:
    int forwardPin;
    int reversePin;
    Direction currentDirection;
    char *name;
    void setup();
};

#endif
