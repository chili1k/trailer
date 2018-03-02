#ifndef CONFIG_H
#define CONFIG_H

#include "Leg.h"
#include "Arduino.h"

/*

      roll
       -  A            B 
pitch  -  +------------+  + pitch
          |o  vcc      |
          |            |
          +------------+
          C            D
       +
      roll
*/

// Refresh display interval
#define DISPLAY_REFRESH_MS 3000

// How many amps are flowing when leg is on ground.
#define LEG_ON_GROUND_AMPS 10.0

// Relay state controlls motor direction.
#define MOTOR_FORWARD_STATE false
// Relay "Open mode" = PIN true
#define RELAY_OPEN_PIN_MODE true

#define GYRO_INTERRUPT_PIN 19

// Zero means top position.

const LegConfig legConfigA = {
  "LEG_A", // name
  35, // pinMotorDirection
  37, // pinMotorPower
  24, // pinZeroPos
  22, // pinFinalPos
  A1 // pinPowerMeter
};

const LegConfig legConfigB = {
  "LEG_B", // name
  27, // pinMotorDirection
  29, // pinMotorPower
  28, // pinZeroPos
  26, // pinFinalPos
  A2 // pinPowerMeter
};

const LegConfig legConfigC = {
  "LEG_C", // name
  31, // pinMotorDirection
  33, // pinMotorPower
  32, // pinZeroPos
  30, // pinFinalPos
  A3 // pinPowerMeter
};

const LegConfig legConfigD = {
  "LEG_D", // name
  23, // pinMotorDirection
  25, // pinMotorPower
  36, // pinZeroPos
  34, // pinFinalPos
  A4 // pinPowerMeter
};

/*
// Leg pin configuration UNO
const LegConfig legConfigA = {
  "LEG_A", // name
  4, // pinMotorDirection
  5, // pinMotorPower
  8, // pinZeroPos
  9, // pinFinalPos
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
  8, // pinMotorDirection
  9, // pinMotorPower
  6, // pinZeroPos
  7, // pinFinalPos
  17 // pinPowerMeter
};

*/

// How long to wait after a leg motor is shut down in milliseconds.
#define STOP_MOTORS_DLY 100
// Minimum time leg expands in milliseconds.
#define EXPAND_LEG_DLY 1000

#endif
