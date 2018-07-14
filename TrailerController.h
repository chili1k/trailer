#ifndef TrailerController_h
#define TrailerController_h

#include <Bounce2.h>

#include "Leg.h"
#include "Balancer.h"

/**
 Controls trailer based on user input.
 */
class TrailerController {
  public:
    void setup();
    void loop();

  private:
    Balancer balancer;
    void handleInput();
    void handleEmergencyButton();
    void startSingleMotor();
    void refreshDisplay();
    void printHeader();
    void printTrailerState();
    void printLeg(Leg *leg);
    void printGyro();
    void printAmpers(Leg *leg);
    void printLegPosition(Leg *leg);
    void printCommands();
    void printTitle();

    unsigned long timestamp = 0;
    unsigned long lastRefreshTime = 0;
    Bounce emergencyButton;
};

#endif
