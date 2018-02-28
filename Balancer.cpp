#include "Balancer.h"
#include "Gyro.h"
#include "Leg.h"
#include "LegUtil.h"
#include "Debug.h"
#include "Config.h"

#include <Arduino.h>

void Balancer::setup() {
  gyro.setup();

  for (int i = 0; i < MAX_LEGS; i++) {
    legs[i].setup();
  }
}

void Balancer::loop() {
  gyro.loop();

  switch (state) {
    case State::NoState:
      stateNoStateLoop();
      break;
    case State::ZeroState:
      stateZeroLoop();      
      break;
    case State::ToZeroState:
      stateToZeroLoop();
      break;
    case State::BalancingState:
      stateBalancingLoop();
      break;
    /*
    case State::BalancedState:
      stateBalancedLoop();
      break;
    case State::FinalState:
      stateFinalLoop();
      break;
    case State::ErrorState:
      stateErrorLoop();
      break;
      */
    default:
      DPRINTLN(F("Unknown state."));
  }
}

void Balancer::toZero() {
  switch (state) {
    case State::NoState:
      stateNoStateCmdToZero();
      break;
    case State::ZeroState:
      stateZeroCmdToZero();
      break;
    case State::ToZeroState:
      stateToZeroCmdToZero();
      break;
    case State::BalancingState:
      stateBalancingCmdToZero();
      break;
    case State::BalancedState:
      stateBalancedCmdToZero();
      break;
    case State::FinalState:
      stateFinalCmdToZero();
      break;
    case State::ErrorState:
      stateErrorCmdToZero();
      break;
    default:
      DPRINTLN(F("Unknown state."));;
  }
}

void Balancer::balance() {
  switch (state) {
    case State::NoState:
      stateNoStateCmdBalance();
      break;
    case State::ZeroState:
      stateZeroCmdBalance();
      break;
    case State::ToZeroState:
      stateToZeroCmdBalance();
      break;
    case State::BalancingState:
      stateBalancingCmdBalance();
      break;
    case State::BalancedState:
      stateBalancedCmdBalance();
      break;
    case State::FinalState:
      stateFinalCmdBalance();
      break;
    case State::ErrorState:
      stateErrorCmdBalance();
      break;
    default:
      DPRINTLN(F("Unknown state."));;
  }
}

void Balancer::stopAllLegs() {
  for (int i = 0; i < MAX_LEGS; i++) {
    legs[i].stop();
  }

  setState(State::ErrorState);
}

void Balancer::setState(State newState) {
  state = newState;
}

Leg *Balancer::getLegs() {
  return legs;
}

Gyro *Balancer::getGyro() {
  return &gyro;
}

State Balancer::getState() {
  return state;
}

/////// NoState

void Balancer::stateNoStateLoop() {
  // Nothing to do
}

void Balancer::stateNoStateCmdToZero() {
  setState(State::ZeroState);
}

void Balancer::stateNoStateCmdBalance() {
  setState(State::BalancingState); 
}

/////// Zero state

void Balancer::stateZeroLoop() {
  // Nothing to do
}

void Balancer::stateZeroCmdToZero() {
  DPRINTLN(F("Already at zero."));
}

void Balancer::stateZeroCmdBalance() {
  setState(State::BalancingState); 
}

/////// ToZero state

void Balancer::stateToZeroLoop() {
  int c = 0;

  for (int i = 0; i < MAX_LEGS; i++) {
    Leg leg = legs[i];

    if (leg.getPosition() == LegPosition::Zero) {
      c++;
      leg.stop();
    } else if (leg.getPosition() != LegPosition::Unknown && leg.getPosition() != LegPosition::Zero) {
      leg.collapse();
    }
  }

  if (c == MAX_LEGS) {
    setState(State::ZeroState);
  }
}

void Balancer::stateToZeroCmdToZero() {
  DPRINTLN(F("Already at zero."));
}

void Balancer::stateToZeroCmdBalance() {
  DPRINTLN(F("Trailer is not at zero state yet."));
}

/////// Balancing state

void Balancer::stateBalancingLoop() {
  // Trailer can only be balanced if all legs are on ground
  if (gyro.isBalanced() && LegUtil::allLegsOnGround(legs)) {
    DPRINTLN(F("Trailed BALANCED!"));
    LegUtil::stopAllMotors(legs);
    setState(State::BalancedState);
    return;
  }

  // If all legs in final position
  if (LegUtil::allLegsInPosition(legs, LegPosition::Final)) {
    DPRINTLN(F("Cannot balance. All legs reached final position."));
    LegUtil::stopAllMotors(legs);
    setState(State::ErrorState);
    return;
  }

  // Could also be A and C at the same time
  if (gyro.getPitchPosition() == UnderBalanced && gyro.getRollPosition() == UnderBalanced) {
    expandLeg(legs[LEG_A]);
  } else if (gyro.getPitchPosition() == UnderBalanced && gyro.getRollPosition() == OverBalanced) {
    expandLeg(legs[LEG_B]);
  // Could also be B and D at the same time
  } else if (gyro.getPitchPosition() == OverBalanced && gyro.getRollPosition() == OverBalanced) {
    expandLeg(legs[LEG_C]);
  } else if (gyro.getPitchPosition() == OverBalanced && gyro.getRollPosition() == UnderBalanced) {
    expandLeg(legs[LEG_D]);
  }
}

void Balancer::expandLeg(Leg &leg) {
  // Start only if not already started. Stop any other motors.
  if (leg.isMotorStopped()) {
    LegUtil::stopAllMotors(legs);
    // We don't want to be restarting too fast
    delay(STOP_MOTORS_DLY);
    leg.expand();
    // Expand leg at least one second before trying to expand another leg
    delay(EXPAND_LEG_DLY);
  }
}

void Balancer::stateBalancingCmdToZero() {
  LegUtil::stopAllMotors(legs);
  setState(State::ZeroState);
}

void Balancer::stateBalancingCmdBalance() {
  DPRINTLN(F("Already balancing."));
}

/////// Balanced state

void Balancer::stateBalancedLoop() {
  // Nothing to do
}

void Balancer::stateBalancedCmdToZero() {
  setState(State::ToZeroState);
}

void Balancer::stateBalancedCmdBalance() {
  DPRINTLN(F("Already balanced."));
}

/////// Final state

void Balancer::stateFinalLoop() {
  // Nothing to do
}

void Balancer::stateFinalCmdToZero() {
  setState(State::ToZeroState);
}

void Balancer::stateFinalCmdBalance() {
  DPRINTLN(F("Move to zero position first."));
}

/////// Error state

void Balancer::stateErrorLoop() {
  // Nothing to do
}

void Balancer::stateErrorCmdToZero() {
  setState(State::ToZeroState);
}

void Balancer::stateErrorCmdBalance() {
  // Nothing to do
  DPRINTLN(F("Move to zero position first."));
}

