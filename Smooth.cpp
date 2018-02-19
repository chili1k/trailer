/**
Based on https://www.arduino.cc/en/Tutorial/Smoothing

Smoothing

  Reads repeatedly from an analog input, calculating a running average and
  printing it to the computer. Keeps twenty readings in an array and continually
  averages them.
*/

#include "Smooth.h"

Smooth::Smooth() {
  for (int r = 0; r < MAX_READINGS; r++) {
    readings[r] = 0;
  }
}

float Smooth::putReading(int reading) {
  total = total - readings[readIndex];
  readings[readIndex] = reading;
  total = total + reading;
  readIndex = readIndex + 1;

  if (readIndex >= MAX_READINGS) {
    readIndex = 0;
  }

  average = (float) total / (float) MAX_READINGS;
  return average;
}

float Smooth::getAverage() {
  return average;
}
