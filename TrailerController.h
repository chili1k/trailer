#ifndef TrailerController_h
#define TrailerController_h

#include "Leg.h"
#include "Balancer.h"

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

};

#endif
