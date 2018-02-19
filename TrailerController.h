#ifndef TrailerController_h
#define TrailerController_h

#include "Leg.h"
#include "Balancer.h"
#include "Display.h"

// Refresh display interval
#define DISPLAY_REFRESH_MS 1000

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
    void handleInput();
    void refreshDisplay();
    void printHeader();
    void printTrailerState();
    void printLeg(Leg *leg);
    void printGyro();
    void printAmpers(Leg *leg);
    void printLegPosition(Leg *leg);
    void printCommands();
    void printTitle();

    unsigned long lastRefreshTime;
};

#endif
