/*
  Gyro.h - Gyro handling.
  Created by Mitja Bezensek, Jun 20, 2017
*/

#ifndef Gyro_h
#define Gyro_h

enum Position { UnderBalanced, Levelled, OverBalanced };

class Gyro {
  public:
    Gyro();
    Position getPitchPosition();
    Position getRollPosition();
    bool isLevelled();
    int getRoll();
    int getPitch();
  private:
//    MPU6050 mpu;
    void setup();
};

#endif
