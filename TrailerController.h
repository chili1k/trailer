#ifndef TrailerController_h
#define TrailerController_h

#include "Leg.h"
#include "Balancer.h"
#include "Display.h"

/**
 Controls trailer based on user input.
 */
class TrailerController {
  public:
    TrailerController();
    void setup();
    void loop();

  private:
    Balancer *balancer;
    Display display;
    void printHeader();
    void printLeg(Leg *leg);
    void printGyro(Gyro *gyro);
    void printAmpers(Leg *leg);
    void printLegPosition(Leg *leg);
    void printTitle();
};

#endif
