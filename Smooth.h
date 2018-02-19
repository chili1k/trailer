#ifndef SMOOTH_H
#define SMOOTH_H

#define MAX_READINGS 20

class Smooth {
  public:
    Smooth();
    float putReading(int reading);
    float getAverage();
  private:
    int readings[MAX_READINGS];
    int total = 0;
    int readIndex = 0;
    float average = 0.0;
};

#endif
