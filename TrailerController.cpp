#include <Arduino.h>

#include "TrailerController.h"
#include "Leg.h"
#include "Gyro.h"
#include "Balancer.h"
#include "Config.h"
#include "Ver.h"
#include "Debug.h"

void TrailerController::setup() {
  Serial.begin(115200);
  balancer.setup();
  printTitle();
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
        Serial.println(F("Stopping all motors"));
        balancer.stopAllLegs();
      case '1':
        Serial.println(F("Starting BALANCE operation"));
        balancer.balance();
      case '2':
        Serial.println(F("Returning trailer to ZERO position"));
        balancer.toZero();
      case '3':
        startSingleMotor();
      default:
        Serial.println(F("Unknown command"));
    }
  }
}

void TrailerController::startSingleMotor() {
  // Same for all states.
  Serial.print(F("Choose motor [0-3]: "));
  while (!Serial.available()) { }
  int motorId = Serial.read()-'0';
  Serial.println(motorId);
  if (motorId < 0 || motorId >= MAX_LEGS) {
    Serial.println(F("WARNING: motor must be between 0 and 3."));
    return;
  }


  Serial.print(F("Choose direction [0-Expand 1-Collapse]: "));
  while (!Serial.available()) { }
  int direction = Serial.read()-'0';
  Serial.println(direction);
  if (direction != 0 && direction != 1) {
    Serial.println(F("WARNING: direction must be 0 or 1."));
    return;
  }

  Serial.print(F("Running motor '"));
  Serial.print(motorId);

  if (direction == 0) {
    Serial.println(F("' in Expand direction..."));
    balancer.forceExpandLeg(motorId);
  } else {
    Serial.println(F("' in Collapse direction..."));
    balancer.forceCollapseLeg(motorId);
  }

  Serial.println();
  Serial.println(F("Press any key to stop"));
  while (!Serial.available()) { }
  Serial.read();
  balancer.stopAllLegs();
}

void TrailerController::refreshDisplay() {
  unsigned long now = millis();

  // Do not refresh display often
  if ((now - lastRefreshTime) < DISPLAY_REFRESH_MS) {
    return;
  }

  lastRefreshTime = now;

  printHeader();

  Leg *legs = balancer.getLegs();
  for (int i = 0; i < MAX_LEGS; i++) {
    printLeg(&legs[i]);  
  }

  Serial.println();
  printGyro();
  Serial.println();
  printTrailerState();
  Serial.println(F("--------------------------------------"));
  printCommands();
  Serial.println();
}

void TrailerController::printHeader() {
  Serial.println(F("Motor\tState\tCurrent"));
}

void TrailerController::printTrailerState() {
  State state = balancer.getState();

  Serial.print(F("Trailer state: "));

  switch (state) {
    case State::NoState:
      Serial.println(F("No State"));
      break;
    case State::ZeroState:
      Serial.println(F("At Zero Position"));
      break;
    case State::ToZeroState:
      Serial.println(F("Moving to Zero Position ..."));
      break;
    case State::BalancedState:
      Serial.println(F("Balanced"));
      break;
    case State::BalancingState:
      Serial.println(F("Balancing ..."));
      break;
    case State::FinalState:
      Serial.println(F("At final position"));
      break;
    case State::ErrorState:
      Serial.println(F("Position error."));
      break;
    default:
      Serial.println(F("Unknown trailer state!"));
  }
}

void TrailerController::printLeg(Leg *leg) {
  Serial.print(leg->getName());
  Serial.print(F("\t"));
  printLegPosition(leg);
  Serial.print(F("\t"));
  printAmpers(leg);
  Serial.println();
}

void TrailerController::printGyro() {
  Gyro *gyro = balancer.getGyro();

  float *ypr = gyro->getYPR();
  Serial.print(F("Gyro pitch, roll: ["));
  Serial.print(ypr[0]);
  Serial.print(F(", "));
  Serial.print(ypr[1]);
  Serial.print(F(", "));
  Serial.print(ypr[2]);
  Serial.print(F(" ]"));

}

void TrailerController::printAmpers(Leg *leg) {
  float ampers = leg->getAmpers();
  Serial.print(ampers);
  Serial.print(F(" A"));
}

void TrailerController::printLegPosition(Leg *leg) {
  const char *state;
  switch (leg->getPosition()) {
    case (LegPosition::Unknown):
      state = "Unknown";
      break;
    case (LegPosition::Expanding):
      state = "Expanding";
      break;
    case (LegPosition::Collapsing):
      state = "Collapsing";
      break;
    case (LegPosition::Zero):
      state = "Zero";
      break;
    case (LegPosition::Final):
      state = "Final";
      break;
    default:
      state = "ERROR STATE";
  }

  Serial.print(state);
}

void TrailerController::printCommands() {
  Serial.println(F("0: Stop all motors"));
  Serial.println(F("1: Balance trailer"));
  Serial.println(F("2: Return to zero"));
  Serial.println(F("3: Start single motor"));
}

void TrailerController::printTitle() {
  Serial.println(F("*************************************"));
  Serial.print(F("Trailer balancer "));
  Serial.print(F(VER));
  Serial.print(F(" "));
  Serial.println(F(RELEASE_DATE));
  Serial.println(F("Copyright bezensek.mitja@gmail.com"));
  Serial.println(F("*************************************"));
  Serial.println();
}
