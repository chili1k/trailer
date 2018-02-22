#ifndef Motor_h
#define Motor_h

enum Direction { Forward, Reverse, Stopped };

class Motor {
  public:
    void setup(int forwardPin, int reversePin);

    void start(Direction newDirection);
    void stop();
    bool isStopped();
    bool isRunning();
    Direction getDirection();
  private:
    int forwardPin;
    int reversePin;
    bool setupDone;
    Direction currentDirection;
};

#endif
