#include "Gyro.h"
#include "Arduino.h"

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include <math.h>

#define OUTPUT_READABLE_YAWPITCHROLL
#define INTERRUPT_PIN 2

Gyro::Gyro() {
  setup();
}

Position Gyro::getPitchPosition() {
  return UnderBalanced;
}

Position Gyro::getRollPosition() {
  return UnderBalanced;
}

bool Gyro::isBalanced() {
  return false;
}

void Gyro::setup() {
  
}

