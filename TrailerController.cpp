#include <Arduino.h>

#include "TrailerController.h"
#include "Display.h"
#include "Leg.h"
#include "Gyro.h"
#include "Balancer.h"
#include "Config.h"
#include "Ver.h"
#include "Debug.h"

void TrailerController::setup() {
  // Display needs to be set up first, otherwise DPRINTLN won't work!
  Display::setup();
  balancer.setup();

  printTitle();
  handleInput();
}

void TrailerController::loop() {
  balancer.loop();

  // Main loop
  refreshDisplay();
  handleInput();
}

void TrailerController::handleInput() {
  if (Serial.available() > 0) {
    int b = Serial.read();

    switch (b) {
      case '0':
        Display::println("Stopping all motors");
        balancer.stopAllLegs();
      case '1':
        Display::println("Starting BALANCE operation");
        balancer.balance();
      case '2':
        Display::println("Returning trailer to ZERO position");
        balancer.toZero();
      default:
        Display::println("Unkwnon command");
    }
  }
}

void TrailerController::refreshDisplay() {
  unsigned long now = millis();

  // Do not refresh Display::too often
  if ((now - lastRefreshTime) < DISPLAY_REFRESH_MS) {
    return;
  }

  lastRefreshTime = now;

  printHeader();

  Leg *legs = balancer.getLegs();
  for (int i = 0; i < MAX_LEGS; i++) {
    printLeg(&legs[i]);  
  }

  Display::newline();
  printGyro();
  Display::newline();
  printTrailerState();
  Display::println("--------------------------------------");
  printCommands();
}

void TrailerController::printHeader() {
  Display::println("Motor\tState\tCurrent");
}

void TrailerController::printTrailerState() {
  State state = balancer.getState();

  Display::print("Trailer state: ");

  switch (state) {
    case NoState:
      Display::println("No State");
      break;
    case ZeroState:
      Display::println("At Zero Position");
      break;
    case ToZeroState:
      Display::println("Moving to Zero Position ...");
      break;
    case BalancedState:
      Display::println("Balanced");
      break;
    case BalancingState:
      Display::println("Balancing ...");
      break;
    case FinalState:
      Display::println("At final position");
      break;
    case ErrorState:
    default:
      Display::println("Unknown trailer state!");
  }
}

void TrailerController::printLeg(Leg *leg) {
  Display::print(leg->getName());
  Display::print("\t");
  printLegPosition(leg);
  Display::print("\t");
  printAmpers(leg);
  Display::newline();
}

void TrailerController::printGyro() {
  Gyro *gyro = balancer.getGyro();

  float *ypr = gyro->getYPR();
  Display::print("Gyro pitch, roll: [");
  Display::print(ypr[0]);
  Display::print(", ");
  Display::print(ypr[1]);
  Display::print(", ");
  Display::print(ypr[2]);
  Display::print(" ]");

}

void TrailerController::printAmpers(Leg *leg) {
  float ampers = leg->getAmpers();
  Display::print(ampers);
  Display::print(" A");
}

void TrailerController::printLegPosition(Leg *leg) {
  const char *state;
  switch (leg->getPosition()) {
    case (Unknown):
      state = "Unknown";
      break;
    case (Expanding):
      state = "Expanding";
      break;
    case (Collapsing):
      state = "Collapsing";
      break;
    case (Zero):
      state = "Zero";
      break;
    case (Final):
      state = "Final";
      break;
    default:
      state = "ERROR STATE";
  }

  Display::print(state);
}

void TrailerController::printCommands() {
  Display::println("0: Stop all motors");
  Display::println("1: Balance trailer");
  Display::println("2: Return to zero");
  Display::println("3: Start single motor");
}

void TrailerController::printTitle() {
  Display::println("*************************************");
  Display::print  ("** Trailer balancer ");
  Display::print(VER);
  Display::print(" ");
  Display::println(RELEASE_DATE);
  Display::println("Copyright bezensek.mitja@gmail.com");
  Display::println("*************************************");
  Display::newline();
}
