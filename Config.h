#ifndef CONFIG_H
#define CONFIG_H

#include "Leg.h"
#include "Arduino.h"
#include "HBridgeMotor.h"
#include "WinchMotor.h"

//#define MODEL_UNO
#define MODEL_MEGA

#define BLINK_LED_PIN 13

#define SERIAL_BAUD_RATE 115200

// Refresh display interval
#define DISPLAY_REFRESH_MS 1000

// Serial DEBUG START
// ----------------------------------------------------------
#define SERIAL_DEBUG_BAUD_RATE 115200
// Comment to disable serial debug.
#define SERIAL_DEBUG_ENABLE
// How often data is sent to debug serial port.
#define SERIAL_DEBUG_REFRESH_MS 10
// ----------------------------------------------------------
// Serial DEBUG END

// How many amps are flowing when leg is on ground.
#define LEG_ON_GROUND_AMPS 10.0

#define EMERGENCY_STOP_PIN -1 //3//2

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

// NOTE: Check Gyro.cpp for calibarion instructions.
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

//static HBridgeMotor motorA(HBRIDGE_MOTOR_2KH);
static WinchMotor motorA;
static WinchMotor motorB;
static WinchMotor motorC;
static WinchMotor motorD;
//static HBridgeMotor motorA(HBRIDGE_MOTOR_2K);
//static HBridgeMotor motorB(HBRIDGE_MOTOR_2K);
//static HBridgeMotor motorC(HBRIDGE_MOTOR_1K);
//static HBridgeMotor motorD(HBRIDGE_MOTOR_2KH); //HBRIDGE_MOTOR_1KH

// PWM pins 2 - 13 and 44 - 46
// https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/

// Zero means all legs are in air.
/*//PWM
const LegConfig legConfigA = {
    "LEG_A", // name
    &motorA, // motorType
    80, // pwm
    7, // pinMotorDirection1
    -1, // pinMotorDirection2
    5, // pinMotorPower
    24, // pinZeroPos
    22, // pinFinalPos
    A1 // pinPowerMeter
};
*///winch
const LegConfig legConfigA = {
    "LEG_A", // name
    &motorA, // motorType
    255, // pwm
    35, // pinMotorDirection1
    -1, // pinMotorDirection2
    37, // pinMotorPower
    24, // pinZeroPos
    22, // pinFinalPos
    A1 // pinPowerMeter
};

/*//PWM
const LegConfig legConfigB = {
    "LEG_B", // name
    &motorB, // motorType
    80, // pwm
    8, // pinMotorDirection1
    -1, // pinMotorDirection2
    6, // pinMotorPower
    28, // pinZeroPos
    26, // pinFinalPos
    A2 // pinPowerMeter
};
*///winch
const LegConfig legConfigB = {
    "LEG_B", // name
    &motorB, // motorType
    255, // pwm
    27, // pinMotorDirection1
    -1, // pinMotorDirection2
    29, // pinMotorPower
    28, // pinZeroPos
    26, // pinFinalPos
    A2 // pinPowerMeter
};


/*//PWM
const LegConfig legConfigC = {
    "LEG_C", // name
    &motorC, // motorType
    // For this motor the PWM logic is reverse, so 0 is max power
    0, // pwm 60 //0
    12, // pinMotorDirection1
    4, // pinMotorDirection2
    3, // pinMotorPower
    32, // pinZeroPos
    30, // pinFinalPos
    A3 // pinPowerMeter
};
*/

//winch
const LegConfig legConfigC = {
    "LEG_C", // name
    &motorC, // motorType
    255, // pwm
    31, // pinMotorDirection1
    -1, // pinMotorDirection2
    33, // pinMotorPower
    32, // pinZeroPos
    30, // pinFinalPos
    A3 // pinPowerMeter
};

/*//PWM
const LegConfig legConfigD = {
    "LEG_D", // name
    &motorD, // motorType
    254, // pwm
    10, // pinMotorDirection1
    11, // pinMotorDirection2
    9, // pinMotorPower
    36, // pinZeroPos
    34, // pinFinalPos
    A4 // pinPowerMeter
};
*/
//winch
const LegConfig legConfigD = {
    "LEG_D", // name
    &motorD, // motorType
    255, // pwm
    23, // pinMotorDirection1
    -1, // pinMotorDirection2
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
