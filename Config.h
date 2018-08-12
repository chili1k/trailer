#ifndef CONFIG_H
#define CONFIG_H

#include "Leg.h"
#include "Arduino.h"

/*
A-------------------------------------C
|                                     |
|            A-------------C -        |
|            |             |          |
|            |             | pitch    |
|            |             |          |--->
|            |xy           |          |
|            B-------------D +        |
|             gyro facing up          |
|            -    roll     +          |
|                                     |
B-------------------------------------D
*/

//#define MODEL_UNO
#define MODEL_MEGA

#define BLINK_LED_PIN 13

#define SERIAL_BAUD_RATE 115200

// Refresh display interval
#define DISPLAY_REFRESH_MS 1000

// How many amps are flowing when leg is on ground.
#define LEG_ON_GROUND_AMPS 10.0

#define EMERGENCY_STOP_PIN 3

// Relay state controlls motor direction.
#define MOTOR_FORWARD_STATE false
#define MOTOR_REVERSE_STATE !MOTOR_FORWARD_STATE
// Pin configurations for motor ON and OFF.
#define MOTOR_ON_PIN_MODE false
#define MOTOR_OFF_PIN_MODE !MOTOR_ON_PIN_MODE

#ifdef MODEL_MEGA
  #define GYRO_INTERRUPT_PIN 19
#else
  #define GYRO_INTERRUPT_PIN 2
#endif

// At which trailer leg is gyro xy positioned:
#define GYRO_XY_ORIENTATION LEG_B
// true: xy sign looking up to the sky.
// false: xy sign looking down to the ground.
#define GYRO_XY_FACE_UP false

// all possible gyro rotations
const int gyroLegToTrailerLeg[4][4] = {
  // gyro xy at Leg A
  {LEG_C, LEG_A, LEG_D, LEG_B},
  // gyro xy at Leg B (main position)
  {LEG_A, LEG_B, LEG_C, LEG_D},
  // gyro xy at Leg C
  {LEG_D, LEG_C, LEG_B, LEG_A},
  // gyro xy at Leg D
  {LEG_B, LEG_D, LEG_A, LEG_C}
};

#define GYRO_LEG_TO_TRAILER_LEG(GYRO_LEG) (gyroLegToTrailerLeg[GYRO_XY_ORIENTATION][GYRO_LEG])

// Zero means all legs are in air.
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
#define STOP_MOTORS_DLY 200
// Minimum time leg expands in milliseconds.
#define EXPAND_LEG_DLY 1000

#endif
