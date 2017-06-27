/*
  Gyro.h - Gyro handling.
  Created by Mitja Bezensek, Jun 20, 2017
*/

#ifndef Gyro_h
#define Gyro_h

enum Position { UnderBalanced, Balanced, OverBalanced };

class Gyro {
  public:
    Gyro();
    Position getPitchPosition();
    Position getRollPosition();
    bool isBalanced();
  private:
//    MPU6050 mpu;
    void setup();
};

#endif
