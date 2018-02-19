#include <Arduino.h>

#include "TrailerController.h"
#include "Display.h"
#include "Leg.h"
#include "Gyro.h"
#include "Display.h"
#include "Balancer.h"
#include "Config.h"
#include "Ver.h"

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

  Balancer balancer(legs, &gyro); 
  this->balancer = &balancer;
  lastRefreshTime = 0;
}

void TrailerController::setup() {
  balancer->setup();
  display.setup();

  printTitle();
  handleInput();
}

void TrailerController::loop() {
  //balancer->loop();

  // Main loop
  refreshDisplay();
  //handleInput();
}

void TrailerController::handleInput() {
  if (Serial.available() > 0) {
    int b = Serial.read();

    switch (b) {
      case '0':
        display.println("Stopping all motors");
        balancer->stopAllLegs();
      case '1':
        display.println("Starting BALANCE operation");
        balancer->balance();
      case '2':
        display.println("Returning trailer to ZERO position");
        balancer->toZero();
      default:
        display.println("Unkwnon command");
    }
  }
}

void TrailerController::refreshDisplay() {
  unsigned long now = millis();

  // Do not refresh display too often
  if ((now - lastRefreshTime) < DISPLAY_REFRESH_MS) {
    return;
  }

  lastRefreshTime = now;

  printHeader();

  display.println("Hello there");

  Leg **leg = balancer->getLegs();
  for (int i = 0; i < MAX_LEGS; i++) {
    printLeg(leg[i]);  
  }

  display.newline();
  printGyro();
  display.newline();
  printTrailerState();
  display.println("--------------------------------------");
  printCommands();
}

void TrailerController::printHeader() {
  display.println("Motor\tState\tCurrent");
}

void TrailerController::printTrailerState() {
  State state = balancer->getState();

  display.print("Trailer state: ");

  switch (state) {
    case NoState:
      display.println("No State");
    case ZeroState:
      display.println("At Zero Position");
    case ToZeroState:
      display.println("Moving to Zero Position ...");
    case BalancedState:
      display.println("Balanced");
    case BalancingState:
      display.println("Balancing ...");
    case FinalState:
      display.println("At final position");
    case ErrorState:
    default:
      display.println("Unknown trailer state!");
  }
}

void TrailerController::printLeg(Leg *leg) {
  display.print(leg->getName());
  display.print("\t");
  display.print(leg->getName());
  display.print("\t");
  printLegPosition(leg);
  display.print("\t");
  printAmpers(leg);
  display.newline();
}

void TrailerController::printGyro() {
  Gyro *gyro = balancer->getGyro();

  float *ypr = gyro->getYPR();
  display.print("Gyro pitch, roll: [");
  display.print(ypr[0]);
  display.print(", ");
  display.print(ypr[1]);
  display.print(", ");
  display.print(ypr[2]);
  display.print(" ]");

}

void TrailerController::printAmpers(Leg *leg) {
  float ampers = leg->getAmpers();
  display.print(ampers);
  display.print(" A");
}

void TrailerController::printLegPosition(Leg *leg) {
  const char *state;
  switch (leg->getPosition()) {
    case (Unknown):
      state = "Unknown";
    case (Expanding):
      state = "Expanding";
    case (Collapsing):
      state = "Collapsing";
    case (Zero):
      state = "Zero";
    case (Final):
      state = "Final";
    default:
      state = "ERROR STATE";
  }

  display.print(state);
}

void TrailerController::printCommands() {
  display.println("0: Stop all motors");
  display.println("1: Balance trailer");
  display.println("2: Return to zero");
  display.println("3: Start single motor");
}

void TrailerController::printTitle() {
  display.println("*************************************");
  display.print  ("** Trailer balancer ");
  display.print(VER);
  display.print(" ");
  display.println(RELEASE_DATE);
  display.println("Copyright bezensek.mitja@gmail.com");
  display.println("*************************************");
  display.newline();
}
