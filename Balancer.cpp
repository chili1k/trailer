#include "Balancer.h"
#include "Gyro.h"
#include "Leg.h"
#include "LegUtil.h"
#include "Debug.h"

#include "Arduino.h"


Balancer::Balancer(Leg **legs, Gyro *gyro, Display *display) {
  this->legs = legs;
  this->gyro = gyro;
  this->display = display;
  this->state = NoState;
}

void Balancer::setup() {
  for (int i = 0; i < MAX_LEGS; i++) {
    legs[i]->setup();
  }

  gyro->setup();
  display->setup();
}

void Balancer::loop() {
  switch (state) {
    case (NoState):
      stateNoStateLoop();
      break;
    case (ZeroState):
      stateZeroLoop();      
      break;
    case (ToZeroState):
      stateToZeroLoop();
      break;
    case (BalancingState):
      stateBalancingLoop();
      break;
    case (BalancedState):
      stateBalancedLoop();
      break;
    case (FinalState):
      stateFinalLoop();
      break;
    case (ErrorState):
      stateErrorLoop();
      break;
    default:
      display->println("Unknown state.");
  }
}

void Balancer::toZero() {
  switch (state) {
    case (NoState):
      stateNoStateCmdToZero();
      break;
    case (ZeroState):
      stateZeroCmdToZero();
      break;
    case (ToZeroState):
      stateToZeroCmdToZero();
      break;
    case (BalancingState):
      stateBalancingCmdToZero();
      break;
    case (BalancedState):
      stateBalancedCmdToZero();
      break;
    case (FinalState):
      stateFinalCmdToZero();
      break;
    case (ErrorState):
      stateErrorCmdToZero();
      break;
    default:
      display->println("Unknown state.");
  }
}

void Balancer::balance() {
  switch (state) {
    case (NoState):
      stateNoStateCmdBalance();
      break;
    case (ZeroState):
      stateZeroCmdBalance();
      break;
    case (ToZeroState):
      stateToZeroCmdBalance();
      break;
    case (BalancingState):
      stateBalancingCmdBalance();
      break;
    case (BalancedState):
      stateBalancedCmdBalance();
      break;
    case (FinalState):
      stateFinalCmdBalance();
      break;
    case (ErrorState):
      stateErrorCmdBalance();
      break;
    default:
      display->println("Unknown state.");
  }
}

void Balancer::setState(State newState) {
  state = newState;
}

/////// NoState

void Balancer::stateNoStateLoop() {
  // Nothing to do
}

void Balancer::stateNoStateCmdToZero() {
  setState(ZeroState);
}

void Balancer::stateNoStateCmdBalance() {
  setState(BalancingState); 
}

/////// Zero state

void Balancer::stateZeroLoop() {
  // Nothing to do
}

void Balancer::stateZeroCmdToZero() {
  display->println("Already at zero.");
}

void Balancer::stateZeroCmdBalance() {
  setState(BalancingState); 
}

/////// ToZero state

void Balancer::stateToZeroLoop() {
  int c = 0;

  for (int i = 0; i < MAX_LEGS; i++) {
    Leg *leg = legs[i];

    if (leg->getPosition() == Zero) {
      c++;
      leg->stop();
    } else if (leg->getPosition() != Unknown && leg->getPosition() != Zero) {
      leg->collapse();
    }
  }

  if (c == MAX_LEGS) {
    setState(ZeroState);
  }
}

void Balancer::stateToZeroCmdToZero() {
  display->println("Already at zero.");
}

void Balancer::stateToZeroCmdBalance() {
  display->println("Trailer is not at zero state yet.");
}

/////// Balancing state

void Balancer::stateBalancingLoop() {
  // Trailer can only be balanced if all legs are on ground
  if (gyro->isBalanced() && LegUtil::allLegsOnGround(legs)) {
    display->println("Trailed BALANCED!");
    LegUtil::stopAllMotors(legs);
    setState(BalancedState);
    return;
  }

  // If all legs in final position
  if (LegUtil::allLegsInPosition(legs, Final)) {
    display->println("Cannot balance. All legs reached final position.");
    LegUtil::stopAllMotors(legs);
    setState(ErrorState);
    return;
  }

  // Could also be A and C at the same time
  if (gyro->getPitchPosition() == UnderBalanced && gyro->getRollPosition() == UnderBalanced) {
    expandLeg(legs[LEG_A]);
  } else if (gyro->getPitchPosition() == UnderBalanced && gyro->getRollPosition() == OverBalanced) {
    expandLeg(legs[LEG_B]);
  // Could also be B and D at the same time
  } else if (gyro->getPitchPosition() == OverBalanced && gyro->getRollPosition() == OverBalanced) {
    expandLeg(legs[LEG_C]);
  } else if (gyro->getPitchPosition() == OverBalanced && gyro->getRollPosition() == UnderBalanced) {
    expandLeg(legs[LEG_D]);
  }
}

void Balancer::expandLeg(Leg *leg) {
  // Start only if not already started. Stop any other motors.
  if (leg->isMotorStopped()) {
    LegUtil::stopAllMotors(legs);
    // We don't want to be restarting too fast
    delay(STOP_MOTORS_DLY);
    leg->expand();
    // Expand leg at least one second before trying to expand another leg
    delay(EXPAND_LEG_DLY);
  }
}

void Balancer::stateBalancingCmdToZero() {
  LegUtil::stopAllMotors(legs);
  setState(ZeroState);
}

void Balancer::stateBalancingCmdBalance() {
  display->println("Already balancing.");
}

/////// Balanced state

void Balancer::stateBalancedLoop() {
  // Nothing to do
}

void Balancer::stateBalancedCmdToZero() {
  setState(ToZeroState);
}

void Balancer::stateBalancedCmdBalance() {
  display->println("Already balanced.");
}

/////// Final state

void Balancer::stateFinalLoop() {
  // Nothing to do
}

void Balancer::stateFinalCmdToZero() {
  setState(ToZeroState);
}

void Balancer::stateFinalCmdBalance() {
  display->println("Move to zero position first.");
}

/////// Error state

void Balancer::stateErrorLoop() {
  // Nothing to do
}

void Balancer::stateErrorCmdToZero() {
  setState(ToZeroState);
}

void Balancer::stateErrorCmdBalance() {
  // Nothing to do
  display->println("Move to zero position first.");
}
