#ifndef CONFIG_H
#define CONFIG_H

#include "Leg.h"

// How many amps are flowing when leg is on ground.
#define LEG_ON_GROUND_AMPS 10.0

// Relay state controlls motor direction.
#define MOTOR_FORWARD_STATE true

// Leg pin configuration
const LegConfig legConfigA = {
  "LEG_A", // name
  8, // pinMotorDirection
  3, // pinMotorPower
  4, // pinZeroPos
  5, // pinFinalPos
  14 // pinPowerMeter
};

const LegConfig legConfigB = {
  "LEG_B", // name
  6, // pinMotorDirection
  7, // pinMotorPower
  8, // pinZeroPos
  9, // pinFinalPos
  15 // pinPowerMeter
};

const LegConfig legConfigC = {
  "LEG_C", // name
  10, // pinMotorDirection
  11, // pinMotorPower
  12, // pinZeroPos
  13, // pinFinalPos
  16 // pinPowerMeter
};

const LegConfig legConfigD = {
  "LEG_D", // name
  4, // pinMotorDirection
  5, // pinMotorPower
  6, // pinZeroPos
  7, // pinFinalPos
  17 // pinPowerMeter
};
/*
const LegConfig legConfigA = {
  "LEG_A", // name
  22, // pinMotorDirection
  23, // pinMotorPower
  24, // pinZeroPos
  25, // pinFinalPos
  0 // pinPowerMeter
};

const LegConfig legConfigB = {
  "LEG_B", // name
  26, // pinMotorDirection
  27, // pinMotorPower
  28, // pinZeroPos
  29, // pinFinalPos
  1 // pinPowerMeter
};

const LegConfig legConfigC = {
  "LEG_C", // name
  30, // pinMotorDirection
  31, // pinMotorPower
  32, // pinZeroPos
  33, // pinFinalPos
  2 // pinPowerMeter
};

const LegConfig legConfigD = {
  "LEG_D", // name
  34, // pinMotorDirection
  35, // pinMotorPower
  36, // pinZeroPos
  37, // pinFinalPos
  3 // pinPowerMeter
};
*/
// How long to wait after a leg motor is shut down in milliseconds.
#define STOP_MOTORS_DLY 100
// Minimum time leg expands in milliseconds.
#define EXPAND_LEG_DLY 1000

#endif
