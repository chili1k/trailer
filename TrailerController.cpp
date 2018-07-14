#include <Arduino.h>
#include <Bounce2.h>
#include <avr/wdt.h>

#include "TrailerController.h"
#include "Leg.h"
#include "Gyro.h"
#include "Balancer.h"
#include "Config.h"
#include "Ver.h"
#include "Debug.h"

bool wasEmergencyButtonPushed;
bool blinkLedState;

void TrailerController::setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  balancer.setup();
  printTitle();
  
  pinMode(BLINK_LED_PIN, OUTPUT);
  pinMode(EMERGENCY_STOP_PIN, INPUT);
  emergencyButton.attach(EMERGENCY_STOP_PIN);
  emergencyButton.interval(20);
  
  //wdt_enable(WDTO_4S);
}

void TrailerController::loop() {
  handleEmergencyButton();
  balancer.loop();
  //wdt_reset();

  // Main loop
  refreshDisplay();
  handleInput();
}

void TrailerController::blinkLed() {
  blinkLedState = !blinkLedState;
  digitalWrite(BLINK_LED_PIN, blinkLedState);
}

void TrailerController::handleEmergencyButton() {
  emergencyButton.update();

  bool pressDetected = emergencyButton.read();
  if (!wasEmergencyButtonPushed) {
    if (pressDetected) {
      balancer.stopAllLegs();
      wasEmergencyButtonPushed = true;
    }
  } else {
    if (!pressDetected) {
      wasEmergencyButtonPushed = false;
    }
  }
}

void TrailerController::printCommands() {
  Serial.println(F("0: Stop all motors"));
  Serial.println(F("1: Balance trailer"));
  Serial.println(F("2: Return to zero"));
  Serial.println(F("3: Return to ground"));
  Serial.println(F("4: Move to final"));
  Serial.println(F("5: Start single motor"));
}

void TrailerController::handleInput() {
  if (Serial.available() > 0) {
    int b = Serial.read();

    switch (b) {
      case '0':
        Serial.println(F("Stopping all motors"));
        balancer.stopAllLegs();
        break;
      case '1':
        Serial.println(F("Starting BALANCE operation"));
        balancer.balance();
        break;
      case '2':
        Serial.println(F("To ZERO position"));
        balancer.toZero();
        break;
      case '3':
        Serial.println(F("To GROUND position"));
        balancer.toGround();
        break;
      case '4':
        Serial.println(F("To FINAL position"));
        balancer.toFinal();
        break;
      case '5':
        startSingleMotor();
        break;
      default:
        Serial.println(F("Unknown command"));
    }
  }
}

void TrailerController::startSingleMotor() {
  // Same for all states.
  Serial.print(F("Choose motor [A-D]: "));
  while (!Serial.available()) { }
  int motorId = Serial.read()-'a';
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

/*  Serial.println();
  Serial.println(F("Press any key to stop"));
  while (!Serial.available()) { }
  Serial.read();
  balancer.stopAllLegs();
  */
}

void TrailerController::refreshDisplay() {
  unsigned long now = millis();

  // Do not refresh display often
  if ((now - lastRefreshTime) < DISPLAY_REFRESH_MS) {
    return;
  }

  blinkLed();
  
  timestamp++;
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
  Serial.print(F("#"));
  Serial.println(timestamp);
  Serial.println(F("Motor\tState\t\tCurrent"));
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
  Serial.print(F("\t\t"));
  printAmpers(leg);
  Serial.println();
}

void TrailerController::printGyro() {
  Gyro *gyro = balancer.getGyro();

  float *ypr = gyro->getYPR();
  Serial.print(F("Gyro pitch, roll: ["));
  Serial.print(ypr[1], 4);
  Serial.print(F(", "));
  Serial.print(ypr[2], 4);
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
  if (leg->isOnGround()) {
    Serial.print(F(" Ground"));
  }
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
