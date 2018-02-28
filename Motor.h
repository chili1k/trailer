#ifndef Motor_h
#define Motor_h

enum Direction { Forward, Reverse, Stopped };

class Motor {
  public:
    void setup(int directionPin, int powerPin);

    void start(Direction newDirection);
    void stop();
    bool isStopped();
    bool isRunning();
    Direction getDirection();
  private:
    int directionPin;
    int powerPin;
    Direction currentDirection = Stopped;
};

#endif
