/*
  Gyro.h - Gyro handling.
  Created by Mitja Bezensek, Jun 20, 2017
*/

#ifndef GYRO_H
#define GYRO_H

enum Position { UnderBalanced, Balanced, OverBalanced };

class Gyro {
  public:
    Gyro();
    Position getPitchPosition();
    Position getRollPosition();
    bool isBalanced();
    void loop();
    void setup();
    float *getYPR();

  private:
    float ypr[3] = {-1.0, -1.0, -1.0};
    float pitch;
    float roll;
    void printPitchRoll();

};

#endif
