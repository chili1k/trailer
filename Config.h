#ifndef CONFIG_H
#define CONFIG_H

#include "Leg.h"

// How many amps are flowing when leg is on ground
#define LEG_ON_GROUND_AMPS 10.0

// Leg pin configuration
const LegConfig legConfigA = {
  "LEG_A", // name
  22, // pinMotorForward
  23, // pinMotorReverse
  24, // pinZeroPos
  25, // pinFinalPos
  0 // pinPowerMeter
};

const LegConfig legConfigB = {
  "LEG_B", // name
  26, // pinMotorForward
  27, // pinMotorReverse
  28, // pinZeroPos
  29, // pinFinalPos
  1 // pinPowerMeter
};

const LegConfig legConfigC = {
  "LEG_C", // name
  30, // pinMotorForward
  31, // pinMotorReverse
  32, // pinZeroPos
  33, // pinFinalPos
  2 // pinPowerMeter
};

const LegConfig legConfigD = {
  "LEG_D", // name
  34, // pinMotorForward
  35, // pinMotorReverse
  36, // pinZeroPos
  37, // pinFinalPos
  3 // pinPowerMeter
};

// How long to wait after a leg motor is shut down in milliseconds.
#define STOP_MOTORS_DLY 100
// Minimum time leg expands in milliseconds.
#define EXPAND_LEG_DLY 1000

#endif
