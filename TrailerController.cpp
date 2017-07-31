#include <string.h>

#include "TrailerController.h"
#include "Leg.h"
#include "Gyro.h"
#include "Display.h"
#include "Balancer.h"
#include "Config.h"

TrailerController::TrailerController() {
  LegConfig legConfigs[] = {
    { .name = LEG_A_NAME,
      .pinMotorForward = LEG_A_PIN_MOTOR_FORWARD,
      .pinMotorReverse = LEG_A_PIN_MOTOR_REVERSE,
      .pinZeroPos = LEG_A_PIN_MOTOR_ZEROPOS,
      .pinFinalPos = LEG_A_PIN_MOTOR_FINALPOS,
      .pinPowerMeter = LEG_A_PIN_MOTOR_POWERMETER
    },
    { .name = LEG_B_NAME,
      .pinMotorForward = LEG_B_PIN_MOTOR_FORWARD,
      .pinMotorReverse = LEG_B_PIN_MOTOR_REVERSE,
      .pinZeroPos = LEG_B_PIN_MOTOR_ZEROPOS,
      .pinFinalPos = LEG_B_PIN_MOTOR_FINALPOS,
      .pinPowerMeter = LEG_B_PIN_MOTOR_POWERMETER
    },
    { .name = LEG_C_NAME,
      .pinMotorForward = LEG_C_PIN_MOTOR_FORWARD,
      .pinMotorReverse = LEG_C_PIN_MOTOR_REVERSE,
      .pinZeroPos = LEG_C_PIN_MOTOR_ZEROPOS,
      .pinFinalPos = LEG_C_PIN_MOTOR_FINALPOS,
      .pinPowerMeter = LEG_C_PIN_MOTOR_POWERMETER
    },
    { .name = LEG_D_NAME,
      .pinMotorForward = LEG_D_PIN_MOTOR_FORWARD,
      .pinMotorReverse = LEG_D_PIN_MOTOR_REVERSE,
      .pinZeroPos = LEG_D_PIN_MOTOR_ZEROPOS,
      .pinFinalPos = LEG_D_PIN_MOTOR_FINALPOS,
      .pinPowerMeter = LEG_D_PIN_MOTOR_POWERMETER
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
