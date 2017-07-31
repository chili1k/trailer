#ifndef CONFIG_H
#define CONFIG_H

#include "Leg.h"

// How many amps are flowing when leg is on ground
#define LEG_ON_GROUND_AMPS 10.0

// Leg pin configuration
#define LEG_A_NAME "LegA"
#define LEG_A_PIN_MOTOR_FORWARD 22
#define LEG_A_PIN_MOTOR_REVERSE 23
#define LEG_A_PIN_MOTOR_ZEROPOS 24
#define LEG_A_PIN_MOTOR_FINALPOS 25
#define LEG_A_PIN_MOTOR_POWERMETER 0

#define LEG_B_NAME "LegB"
#define LEG_B_PIN_MOTOR_FORWARD 26
#define LEG_B_PIN_MOTOR_REVERSE 27
#define LEG_B_PIN_MOTOR_ZEROPOS 28
#define LEG_B_PIN_MOTOR_FINALPOS 29
#define LEG_B_PIN_MOTOR_POWERMETER 1

#define LEG_C_NAME "LegC"
#define LEG_C_PIN_MOTOR_FORWARD 30
#define LEG_C_PIN_MOTOR_REVERSE 31
#define LEG_C_PIN_MOTOR_ZEROPOS 32
#define LEG_C_PIN_MOTOR_FINALPOS 33
#define LEG_C_PIN_MOTOR_POWERMETER 2

#define LEG_D_NAME "LegD"
#define LEG_D_PIN_MOTOR_FORWARD 34
#define LEG_D_PIN_MOTOR_REVERSE 35
#define LEG_D_PIN_MOTOR_ZEROPOS 36
#define LEG_D_PIN_MOTOR_FINALPOS 37
#define LEG_D_PIN_MOTOR_POWERMETER 3

// How long to wait after a leg motor is shut down in milliseconds.
#define STOP_MOTORS_DLY 100
// Minimum time leg expands in milliseconds.
#define EXPAND_LEG_DLY 1000

#endif
