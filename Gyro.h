/*
  Gyro.h - Gyro handling.
  Created by Mitja Bezensek, Jun 20, 2017
*/

#ifndef GYRO_H
#define GYRO_H

enum class Axe { Yaw, Pitch, Roll };

class Gyro {
  public:
    Gyro();
    float getPitch();
    float getRoll();
    bool isPitchBalanced();
    bool isRollBalanced();
    bool isBalanced();
    void loop();
    void setup();
    float *getYPR();

  private:
    float ypr[3] = {-1.0, -1.0, -1.0};
    float pitch;
    float roll;
    void readGyro();
};

#endif
