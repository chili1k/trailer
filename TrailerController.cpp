#include <string.h>

#include "TrailerController.h"
#include "Leg.h"
#include "Gyro.h"
#include "Display.h"
#include "Balancer.h"

TrailerController::TrailerController() {
  LegConfig legConfigs[] = {
    { .name = "LegA",
      .pinMotorForward = 22,
      .pinMotorReverse = 23,
      .pinZeroPos = 24,
      .pinFinalPos = 25,
      .pinPowerMeter = 0
    },
    { .name = "LegB",
      .pinMotorForward = 26,
      .pinMotorReverse = 27,
      .pinZeroPos = 28,
      .pinFinalPos = 29,
      .pinPowerMeter = 1
    },
    { .name = "LegC",
      .pinMotorForward = 30,
      .pinMotorReverse = 31,
      .pinZeroPos = 32,
      .pinFinalPos = 33,
      .pinPowerMeter = 2
    },
    { .name = "LegD",
      .pinMotorForward = 34,
      .pinMotorReverse = 35,
      .pinZeroPos = 36,
      .pinFinalPos = 37,
      .pinPowerMeter = 3
    }
  };

  Leg legA(&legConfigs[0]);
  Leg legB(&legConfigs[1]);
  Leg legC(&legConfigs[2]);
  Leg legD(&legConfigs[3]);
 
  Leg *legs[] = { &legA, &legB, &legC, &legD };
  Gyro gyro;
  Display display;

  Balancer balancer(legs, &gyro, &display); 
  this->balancer = &balancer;
}

void TrailerController::setup() {
  balancer->setup();
}

void TrailerController::loop() {
  balancer->loop();
}
