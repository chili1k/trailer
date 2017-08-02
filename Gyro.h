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
    void loop();
    bool setup();
    float *getYPR();

  private:
    float pitch;
    float roll;
    void printPitchRoll();
    // yaw, pitch, roll
    float ypr[3];

};

#endif
